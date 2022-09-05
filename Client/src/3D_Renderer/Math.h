#pragma once

#include <vector>

struct Vector3D
{
	float x, y, z;
};

struct Triangle
{
	Vector3D coords[3];
};

struct Mesh
{
	std::vector<Triangle> triangles;
};

struct Matrix4x4
{
	float val[4][4] = { 0 };
};