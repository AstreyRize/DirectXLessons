using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace StlLoader
{
	class Program
	{
		static void Main(string[] args)
		{
			var lastId = 0;
			var result = new List<Vertex>();
			var regExp = new Regex(@"^[\s]{1,}vertex{1}\s(\-*\d+\.\d+e[\+|\-]\d{2})\s(\-*\d+\.\d+e[\+|\-]\d{2})\s(\-*\d+\.\d+e[\+|\-]\d{2})");
			var lines = File.ReadAllLines(@"D:\Desktop\Деталь.stl");

			for (var i = 0; i < lines.Length; i++)
			{
				if (lines[i].Contains("outer loop"))
				{
					var triangle = new List<Vertex>();

					for (var t = 1; t <= 3; t++)
					{
						MatchCollection matches = regExp.Matches(lines[i + t]);

						var newVertex = new Vertex
						{
							ID = lastId,
							X = float.Parse(matches[0].Groups[1].Value.Replace('.', ','), NumberStyles.Float),
							Y = float.Parse(matches[0].Groups[2].Value.Replace('.', ','), NumberStyles.Float),
							Z = float.Parse(matches[0].Groups[3].Value.Replace('.', ','), NumberStyles.Float),
						};

						triangle.Add(newVertex);
						lastId++;
					}

					foreach (var vertex1 in triangle)
					{
						var triangleList = new List<Vertex>();

						foreach (var vertex2 in triangle)
						{
							triangleList.Add(vertex2);
						}

						vertex1.RelatedVertices.Add(triangleList);
					}

					result.AddRange(triangle);
				}
			}

			var vertices = ExportVertices(result);
			var reletionships = ExportReletionships(result);
		}

		static string ExportVertices(List<Vertex> vertices)
		{
			var rand = new Random();
			var result = string.Empty;
			var lines = new List<string>();

			vertices.ForEach(vertex =>
			{
				var x = vertex.X.ToString("0.00000").Replace(",", ".");
				var y = vertex.Y.ToString("0.00000").Replace(",", ".");
				var z = vertex.Z.ToString("0.00000").Replace(",", ".");

				lines.Add($"			{{XMFLOAT3({x}f, {y}f, {z}f), XMFLOAT3({rand.Next(0, 2)}.0f, {rand.Next(0, 2)}.0f, {rand.Next(0, 2)}.0f)}},");
			});

			lines.ForEach(item => result += item + Environment.NewLine);

			return result;
		}

		static string ExportReletionships(List<Vertex> vertices)
		{
			var result = string.Empty;
			var reletionships = new List<string>();

			vertices.ForEach(vertex =>
				vertex.RelatedVertices.ForEach(relationship =>
				{
					var reletionship = string.Join(", ", relationship
						.Select(x => x.ID)
						.OrderBy(x => x)
						.ToArray());
					reletionships.Add(reletionship);
				}));

			reletionships
				.Distinct()
				.ToList()
				.ForEach(item => result += $"			{item},{Environment.NewLine}");

			return result;
		}
	}

	public class Vertex
	{
		public int ID { get; set; }

		public float X { get; set; }

		public float Y { get; set; }

		public float Z { get; set; }

		public List<List<Vertex>> RelatedVertices = new List<List<Vertex>>();
	}
}
