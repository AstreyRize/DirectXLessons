#include <string>
#include <regex>
#include <fstream>
#include "pch.h"
#include "ModelHelper.h"

using namespace DirectX;
using namespace DX;
using namespace std;

// Список вершин модели.
vector<Vertex> result;

// Класс для работы с STL моделями.
ModelHelper::ModelHelper(string modelPath)
{
	// Хранимая процедура для получения координат вершин из файла STL.
	regex re("^[\\s]{1,}vertex{1}\\s(\\-*\\d+\\.\\d+e[\\+|\\-]\\d{2})\\s(\\-*\\d+\\.\\d+e[\\+|\\-]\\d{2})\\s(\\-*\\d+\\.\\d+e[\\+|\\-]\\d{2})");

	int lastId = 0;
	vector<string> fileLines;
	string::size_type sz;

	// Читаем все строки из файла с моделью.
	ifstream file(modelPath);
	string str;

	while (getline(file, str))
	{
		fileLines.push_back(str);
	}

	// Ищем в файле координаты вершин.
	for (int i = 0; i < fileLines.size(); i++)
	{
		if (fileLines[i].find("outer loop") != string::npos)
		{
			vector<Vertex> triangle;

			// Получаем вершины для треугольника.
			for (short t = 1; t <= 3; t++)
			{
				smatch sm;

				// Записываем полученные данные в массив структур Vertex.
				if (regex_search(fileLines[i + t], sm, re))
				{
					Vertex newVertex = { 0 };
					newVertex.ID = lastId;
					newVertex.X = stof(sm[1], &sz);
					newVertex.Y = stof(sm[2], &sz);
					newVertex.Z = stof(sm[3], &sz);

					triangle.push_back(newVertex);
					lastId++;
				}
			}

			// Устанавливаем связи между треугольниками.
			for (short vertex1 = 0; vertex1 < triangle.size(); vertex1++)
			{
				vector<Vertex> triangleList;

				for (short vertex2 = 0; vertex2 < triangle.size(); vertex2++)
				{
					triangleList.push_back(triangle[vertex2]);
				}

				triangle[vertex1].RelatedVertices.push_back(triangleList);
			}

			for (short item = 0; item < triangle.size(); item++) {
				result.push_back(triangle[item]);
			}
		}
	}

	return;
}

// Получаем список вершин в виде массива VertexPositionColor.
Lesson_1::VertexPositionColor* ModelHelper::GetVertices()
{
	vector<Lesson_1::VertexPositionColor> vertexList;

	// Берем каждую вершину и помещаем в массив.
	for (int i = 0; i < result.size(); i++)
	{
		vertexList.push_back({ XMFLOAT3(result[i].X, result[i].Y, result[i].Z), XMFLOAT3(1.0f, 1.0f, 1.0f) });
	}

	return &vertexList[0];
}

// Создаем массив связей из вершин. Каждые три вершины образуют треугольник.
unsigned short* ModelHelper::GetReletionships()
{
	vector<vector<unsigned short>> vertexReletionships;

	// Получаем каждую вершину из списка.
	for (auto& resultItem : result)
	{
		vector<unsigned short> relatedVerticeIds;

		// Для каждой вершины получаем связанные с ней вершины.
		for (auto& relatedVertices : resultItem.RelatedVertices)
		{
			// Из каждой связанной вершины получаем ее ID.
			for (auto& vertex : relatedVertices)
			{
				int id = vertex.ID;
				relatedVerticeIds.push_back(id);
			}

			// Сортируем ID связанных вершин, что бы обратить треугольники наружу.
			sort(relatedVerticeIds.begin(), relatedVerticeIds.end(), greater <>());

			// Далее проверяем на наличие дубликатов.
			if (vertexReletionships.size() == 0)
			{
				vertexReletionships.push_back(relatedVerticeIds);
				continue;
			}

			bool isUnique = true;

			for (auto& reletionships : vertexReletionships)
			{
				if (reletionships.size() < 3)
				{
					continue;
				}

				bool firstItemIsEqual = reletionships[0] == relatedVerticeIds[0];
				bool secondItemIsEqual = reletionships[1] == relatedVerticeIds[1];
				bool thirdItemIsEqual = reletionships[2] == relatedVerticeIds[2];

				if (firstItemIsEqual && secondItemIsEqual && thirdItemIsEqual)
				{
					isUnique = false;
				}
			}

			if (isUnique) {
				vertexReletionships.push_back(relatedVerticeIds);
			}
		}
	}

	vector<unsigned short> reletionshipsResult;

	// Преобразуем все в сплошной массив.
	for (auto& reletionships : vertexReletionships)
	{
		if (reletionships.size() < 3)
		{
			continue;
		}

		reletionshipsResult.push_back(reletionships[0]);
		reletionshipsResult.push_back(reletionships[1]);
		reletionshipsResult.push_back(reletionships[2]);
	}

	return &reletionshipsResult[0];
}