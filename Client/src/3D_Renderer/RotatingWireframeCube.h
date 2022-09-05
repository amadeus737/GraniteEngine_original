#pragma once

#include <GraniteEngine.h>
#include "Math.h"

class RotatingWireframeCube : GraniteGame
{
public:
	RotatingWireframeCube() { }

private:
	void MultiplyMatrixVector(Vector3D& i, Vector3D& o, Matrix4x4& m)
	{
		o.x = i.x * m.val[0][0] + i.y * m.val[1][0] + i.z * m.val[2][0] + m.val[3][0];
		o.y = i.x * m.val[0][1] + i.y * m.val[1][1] + i.z * m.val[2][1] + m.val[3][1];
		o.z = i.x * m.val[0][2] + i.y * m.val[1][2] + i.z * m.val[2][2] + m.val[3][2];
		float w = i.x * m.val[0][3] + i.y * m.val[1][3] + i.z * m.val[2][3] + m.val[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}

public:
	void Start()
	{
		_cubeMesh.triangles =
		{
			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		};

		// Projection Matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)_window.ScreenHeight() / (float)_window.ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		_projMatrix.val[0][0] = fAspectRatio * fFovRad;
		_projMatrix.val[1][1] = fFovRad;
		_projMatrix.val[2][2] = fFar / (fFar - fNear);
		_projMatrix.val[3][2] = (-fFar * fNear) / (fFar - fNear);
		_projMatrix.val[2][3] = 1.0f;
		_projMatrix.val[3][3] = 0.0f;

		_startTime = _time;
		_theta = 0.0f;
	}

	void Update()
	{
		_window.Clear(Granite::Color::Granite());

		Matrix4x4 matrixRotZ;
		Matrix4x4 matrixRotX;

		_theta += 1.0f * _deltaTime;

		// Rotation Z
		matrixRotZ.val[0][0] = cosf(_theta);
		matrixRotZ.val[0][1] = sinf(_theta);
		matrixRotZ.val[1][0] = -sinf(_theta);
		matrixRotZ.val[1][1] = cosf(_theta);
		matrixRotZ.val[2][2] = 1;
		matrixRotZ.val[3][3] = 1;

		// Rotation X
		matrixRotX.val[0][0] = 1;
		matrixRotX.val[1][1] = cosf(_theta * 0.5f);
		matrixRotX.val[1][2] = sinf(_theta * 0.5f);
		matrixRotX.val[2][1] = -sinf(_theta * 0.5f);
		matrixRotX.val[2][2] = cosf(_theta * 0.5f);
		matrixRotX.val[3][3] = 1;

		// Draw Triangles
		for (auto tri : _cubeMesh.triangles)
		{
			Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			// Rotate in Z-Axis
			MultiplyMatrixVector(tri.coords[0], triRotatedZ.coords[0], matrixRotZ);
			MultiplyMatrixVector(tri.coords[1], triRotatedZ.coords[1], matrixRotZ);
			MultiplyMatrixVector(tri.coords[2], triRotatedZ.coords[2], matrixRotZ);

			// Rotate in X-Axis
			MultiplyMatrixVector(triRotatedZ.coords[0], triRotatedZX.coords[0], matrixRotX);
			MultiplyMatrixVector(triRotatedZ.coords[1], triRotatedZX.coords[1], matrixRotX);
			MultiplyMatrixVector(triRotatedZ.coords[2], triRotatedZX.coords[2], matrixRotX);

			// Offset into the screen
			triTranslated = triRotatedZX;
			triTranslated.coords[0].z = triRotatedZX.coords[0].z + 3.0f;
			triTranslated.coords[1].z = triRotatedZX.coords[1].z + 3.0f;
			triTranslated.coords[2].z = triRotatedZX.coords[2].z + 3.0f;

			// Project triangles from 3D --> 2D
			MultiplyMatrixVector(triTranslated.coords[0], triProjected.coords[0], _projMatrix);
			MultiplyMatrixVector(triTranslated.coords[1], triProjected.coords[1], _projMatrix);
			MultiplyMatrixVector(triTranslated.coords[2], triProjected.coords[2], _projMatrix);

			// Scale into view
			triProjected.coords[0].x += 1.0f; triProjected.coords[0].y += 1.0f;
			triProjected.coords[1].x += 1.0f; triProjected.coords[1].y += 1.0f;
			triProjected.coords[2].x += 1.0f; triProjected.coords[2].y += 1.0f;
			triProjected.coords[0].x *= 0.5f * (float)_window.ScreenWidth();
			triProjected.coords[0].y *= 0.5f * (float)_window.ScreenHeight();
			triProjected.coords[1].x *= 0.5f * (float)_window.ScreenWidth();
			triProjected.coords[1].y *= 0.5f * (float)_window.ScreenHeight();
			triProjected.coords[2].x *= 0.5f * (float)_window.ScreenWidth();
			triProjected.coords[2].y *= 0.5f * (float)_window.ScreenHeight();

			// Rasterize triangle
			_window.DrawTriangle(triProjected.coords[0].x, triProjected.coords[0].y,
				triProjected.coords[1].x, triProjected.coords[1].y,
				triProjected.coords[2].x, triProjected.coords[2].y,
				Granite::Color::White());
		}
	}

private:
	Mesh _cubeMesh;
	Matrix4x4 _projMatrix;
	float _theta = 0.0f;
	float _startTime = 0.0f;
};