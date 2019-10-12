#pragma once

#include <string>
#include <regex>
#include <fstream>

#include "../Content/ShaderStructures.h"

namespace DX
{
	struct Vertex
	{
	public:
		int ID;
		float X;
		float Y;
		float Z;
		std::vector<std::vector<Vertex>> RelatedVertices;
	};

	class ModelHelper
	{
	public:
		ModelHelper(std::string modelPath);
		Lesson_1::VertexPositionColor* GetVertices();
		unsigned short* GetReletionships();

	private:
		std::vector<Vertex> result;
	};
}
