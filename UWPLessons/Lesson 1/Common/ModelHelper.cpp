#include <string>
#include <regex>
#include <fstream>
#include "pch.h"
#include "ModelHelper.h"

using namespace DirectX;
using namespace DX;
using namespace std;

// ������ ������ ������.
vector<Vertex> result;

// ����� ��� ������ � STL ��������.
ModelHelper::ModelHelper(string modelPath)
{
	// �������� ��������� ��� ��������� ��������� ������ �� ����� STL.
	regex re("^[\\s]{1,}vertex{1}\\s(\\-*\\d+\\.\\d+e[\\+|\\-]\\d{2})\\s(\\-*\\d+\\.\\d+e[\\+|\\-]\\d{2})\\s(\\-*\\d+\\.\\d+e[\\+|\\-]\\d{2})");

	int lastId = 0;
	vector<string> fileLines;
	string::size_type sz;

	// ������ ��� ������ �� ����� � �������.
	ifstream file(modelPath);
	string str;

	while (getline(file, str))
	{
		fileLines.push_back(str);
	}

	// ���� � ����� ���������� ������.
	for (int i = 0; i < fileLines.size(); i++)
	{
		if (fileLines[i].find("outer loop") != string::npos)
		{
			vector<Vertex> triangle;

			// �������� ������� ��� ������������.
			for (short t = 1; t <= 3; t++)
			{
				smatch sm;

				// ���������� ���������� ������ � ������ �������� Vertex.
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

			// ������������� ����� ����� ��������������.
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

// �������� ������ ������ � ���� ������� VertexPositionColor.
Lesson_1::VertexPositionColor* ModelHelper::GetVertices()
{
	vector<Lesson_1::VertexPositionColor> vertexList;

	// ����� ������ ������� � �������� � ������.
	for (int i = 0; i < result.size(); i++)
	{
		vertexList.push_back({ XMFLOAT3(result[i].X, result[i].Y, result[i].Z), XMFLOAT3(1.0f, 1.0f, 1.0f) });
	}

	return &vertexList[0];
}

// ������� ������ ������ �� ������. ������ ��� ������� �������� �����������.
unsigned short* ModelHelper::GetReletionships()
{
	vector<vector<unsigned short>> vertexReletionships;

	// �������� ������ ������� �� ������.
	for (auto& resultItem : result)
	{
		vector<unsigned short> relatedVerticeIds;

		// ��� ������ ������� �������� ��������� � ��� �������.
		for (auto& relatedVertices : resultItem.RelatedVertices)
		{
			// �� ������ ��������� ������� �������� �� ID.
			for (auto& vertex : relatedVertices)
			{
				int id = vertex.ID;
				relatedVerticeIds.push_back(id);
			}

			// ��������� ID ��������� ������, ��� �� �������� ������������ ������.
			sort(relatedVerticeIds.begin(), relatedVerticeIds.end(), greater <>());

			// ����� ��������� �� ������� ����������.
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

	// ����������� ��� � �������� ������.
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