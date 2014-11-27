#include "Solid.h"
#include <array>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>

const double PI = 3.1415926;

void Solid::GenerateBox() 
{
	FaceTable.clear();
	IndexTable.clear();
	VertexTable.clear();
	NormalTable.clear();

	//Box Data is hard coded
	VertexTable.push_back(new Vector3(-0.5f, -0.5f, -0.5f));
	VertexTable.push_back(new Vector3(0.5f, -0.5f, -0.5f));
	VertexTable.push_back(new Vector3(0.5f, 0.5f, -0.5f));
	VertexTable.push_back(new Vector3(-0.5f, 0.5f, -0.5f));

	VertexTable.push_back(new Vector3(-0.5f, -0.5f, 0.5f));
	VertexTable.push_back(new Vector3(0.5f, -0.5f, 0.5f));
	VertexTable.push_back(new Vector3(0.5f, 0.5f, 0.5f));
	VertexTable.push_back(new Vector3(-0.5f, 0.5f, 0.5f));

	NormalTable.push_back(new Vector3(1.0f, 0.0f, 0.0f));
	NormalTable.push_back(new Vector3(-1.0f, 0.0f, 0.0f));
	NormalTable.push_back(new Vector3(0.0f, 1.0f, 0.0f));
	NormalTable.push_back(new Vector3(0.0f, -1.0f, 0.0f));
	NormalTable.push_back(new Vector3(0.0f, 0.0f, 1.0f));
	NormalTable.push_back(new Vector3(0.0f, 0.0f, -1.0f));

	std::array<Vector3*, 2> index = { VertexTable[2], NormalTable[0] };
	IndexTable.push_back(index);
	index[0] = VertexTable[1];
	index[1] = NormalTable[0];
	IndexTable.push_back(index);
	index[0] = VertexTable[5];
	index[1] = NormalTable[0];
	IndexTable.push_back(index);
	index[0] = VertexTable[6];
	index[1] = NormalTable[0];
	IndexTable.push_back(index);
	std::array<int, 2> face = { 4, 0 };
	FaceTable.push_back(face);

	index[0] = VertexTable[0];
	index[1] = NormalTable[1];
	IndexTable.push_back(index);
	index[0] = VertexTable[3];
	index[1] = NormalTable[1];
	IndexTable.push_back(index);
	index[0] = VertexTable[7];
	index[1] = NormalTable[1];
	IndexTable.push_back(index);
	index[0] = VertexTable[4];
	index[1] = NormalTable[1];
	IndexTable.push_back(index);
	face[0] = 4;
	face[1] = 4;
	FaceTable.push_back(face);

	index[0] = VertexTable[3];
	index[1] = NormalTable[2];
	IndexTable.push_back(index);
	index[0] = VertexTable[2];
	index[1] = NormalTable[2];
	IndexTable.push_back(index);
	index[0] = VertexTable[6];
	index[1] = NormalTable[2];
	IndexTable.push_back(index);
	index[0] = VertexTable[7];
	index[1] = NormalTable[2];
	IndexTable.push_back(index);
	face[0] = 4;
	face[1] = 8;
	FaceTable.push_back(face);

	index[0] = VertexTable[1];
	index[1] = NormalTable[3];
	IndexTable.push_back(index);
	index[0] = VertexTable[0];
	index[1] = NormalTable[3];
	IndexTable.push_back(index);
	index[0] = VertexTable[4];
	index[1] = NormalTable[3];
	IndexTable.push_back(index);
	index[0] = VertexTable[5];
	index[1] = NormalTable[3];
	IndexTable.push_back(index);
	face[0] = 4;
	face[1] = 12;
	FaceTable.push_back(face);

	index[0] = VertexTable[4];
	index[1] = NormalTable[4];
	IndexTable.push_back(index);
	index[0] = VertexTable[5];
	index[1] = NormalTable[4];
	IndexTable.push_back(index);
	index[0] = VertexTable[6];
	index[1] = NormalTable[4];
	IndexTable.push_back(index);
	index[0] = VertexTable[7];
	index[1] = NormalTable[4];
	IndexTable.push_back(index);
	face[0] = 4;
	face[1] = 16;
	FaceTable.push_back(face);

	index[0] = VertexTable[0];
	index[1] = NormalTable[5];
	IndexTable.push_back(index);
	index[0] = VertexTable[3];
	index[1] = NormalTable[5];
	IndexTable.push_back(index);
	index[0] = VertexTable[2];
	index[1] = NormalTable[5];
	IndexTable.push_back(index);
	index[0] = VertexTable[1];
	index[1] = NormalTable[5];
	IndexTable.push_back(index);
	face[0] = 4;
	face[1] = 20;
	FaceTable.push_back(face);
}

//Generates a cone with a certain number of side faces, recommended to be >3
void Solid::GenerateCone(int faces) 
{
	FaceTable.clear();
	IndexTable.clear();
	VertexTable.clear();
	NormalTable.clear();

	//Generate bottom face normal and tip vertex
	VertexTable.push_back(new Vector3(0, 0.5, 0));
	NormalTable.push_back(new Vector3(0.0f, -1.0f, 0.0f));

	//Generate side face normals and bottom points
	for (double i = 0; i < 2 * PI; i += 2 * PI / faces) {
		VertexTable.push_back(new Vector3(0.5*cos(i), -0.5, 0.5*sin(i)));
		Vector3* n = new Vector3();
		n->x = 0.5*cos(i);
		n->y = 1;
		n->z = 0.5*sin(i);
		n->normalize();
		NormalTable.push_back(n);
	}

	//Add bottom face
	std::array<int, 2> face = { faces, 0 };
	FaceTable.push_back(face);

	//Add side faces
	face[0] = 3;
	for (int i = 0; i < faces; i++) {
		face[1] = faces + 3 * i;
		FaceTable.push_back(face);
	}

	//Add bottom face indices
	std::array<Vector3*, 2> index = { VertexTable[0], NormalTable[0] };
	for (int i = 0; i < faces; i++) {
		index[0] = VertexTable[i+1];
		IndexTable.push_back(index);
	}

	//Add side face indices
	for (int i = 0; i < faces; i++) {
		index[0] = VertexTable[i + 1];
		index[1] = NormalTable[i + 1];
		IndexTable.push_back(index);

		index[0] = VertexTable[0];
		index[1] = NormalTable[i + 1]->average(NormalTable[(i + 1) % faces + 1]);
		index[1]->normalize();
		IndexTable.push_back(index);

		index[0] = VertexTable[(i + 1) % faces + 1];
		index[1] = NormalTable[(i + 1) % faces + 1];
		IndexTable.push_back(index);
	}
}

//Faces is the number of side faces of the cylinder, recommended to be >3
void Solid::GenerateCylinder(int faces) 
{
	FaceTable.clear();
	IndexTable.clear();
	VertexTable.clear();
	NormalTable.clear();

	//Generate Top and Bottom Normals
	NormalTable.push_back(new Vector3(0.0f, -1.0f, 0.0f));
	NormalTable.push_back(new Vector3(0.0f, 1.0f, 0.0f));

	//Generate Bottom Vertices and side Normals
	for (double j = 0; j < 2 * PI; j += 2 * PI / faces) {
		VertexTable.push_back(new Vector3(0.5*cos(j), -0.5, 0.5*sin(j)));
		Vector3* n = new Vector3();
		n->x = 0.5*cos(j);
		n->y = 0;
		n->z = 0.5*sin(j);
		n->normalize();
		NormalTable.push_back(n);
	}

	//Generate Top Vertices
	for (double k = 0; k < 2 * PI; k += 2 * PI / faces) {
		VertexTable.push_back(new Vector3(0.5*cos(k), 0.5, 0.5*sin(k)));
	}

	//Add top and bottom faces
	std::array<int, 2> face = { faces, 0 };
	FaceTable.push_back(face);
	face[1] = faces;
	FaceTable.push_back(face);

	//Add side faces
	for (int i = 0; i < faces; i++) {
		face[0] = 4;
		face[1] = faces*2 + i * 4;
		FaceTable.push_back(face);
	}
	
	//Set up indices for top and bottom faces
	std::array<Vector3*, 2> index = { VertexTable[0], NormalTable[0] };
	for (int i = 0; i < faces; i++) {
		index[0] = VertexTable[i];
		index[1] = NormalTable[0];
		IndexTable.push_back(index);
	}

	for (int i = 0; i < faces; i++) {
		index[0] = VertexTable[faces + i];
		index[1] = NormalTable[1];
		IndexTable.push_back(index);
	}

	//Set up indices for side faces
	for (int i = 0; i < faces; i++) {
		index[0] = VertexTable[i];
		index[1] = NormalTable[2 + i];
		IndexTable.push_back(index);

		index[0] = VertexTable[(i + 1) % faces];
		index[1] = NormalTable[2 + (i + 1) % faces];
		IndexTable.push_back(index);

		index[0] = VertexTable[faces + ((i + 1) % faces)];
		index[1] = NormalTable[2 + (i + 1) % faces];
		IndexTable.push_back(index);

		index[0] = VertexTable[faces + i];
		index[1] = NormalTable[2 + i];
		IndexTable.push_back(index);
	}
}
void Solid::GenerateSphere(int slices)
{
	FaceTable.clear();
	IndexTable.clear();
	VertexTable.clear();
	NormalTable.clear();

	//Generate Vertices and Normals
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < slices; j++) {
			VertexTable.push_back(new Vector3(
				sinf(PI * i / slices) * cosf(2 * PI * j / slices) * 0.5,
				cosf(PI * i / slices) * 0.5,
				sinf(PI * i / slices) * sinf(2 * PI * j / slices) * 0.5
				));
			NormalTable.push_back(new Vector3(
				sinf(PI * i / slices) * cosf(2 * PI * j / slices) * 0.5,
				cosf(PI * i / slices) * 0.5,
				sinf(PI * i / slices) * sinf(2 * PI * j / slices) * 0.5
				));
		}
	}

	//Set up faces
	std::array<int, 2> face = { 4, 0 };
	for (int i = 0; i < slices*(slices-1); i++) {
		face[1] = i * 4;
		FaceTable.push_back(face);
	}

	//Set up indices
	std::array<Vector3*, 2> index = { VertexTable[0], NormalTable[0] };
	for (int i = 0; i < slices - 1; i++) {
		for (int j = 0; j < slices; j++) {
			index[0] = VertexTable[i*slices + j];
			index[1] = NormalTable[i*slices + j];
			IndexTable.push_back(index);

			index[0] = VertexTable[i*slices + (j + 1) % slices];
			index[1] = NormalTable[i*slices + (j + 1) % slices];
			IndexTable.push_back(index);

			index[0] = VertexTable[((i + 1) % slices)*slices + (j + 1) % slices];
			index[1] = NormalTable[((i + 1) % slices)*slices + (j + 1) % slices];
			IndexTable.push_back(index);

			index[0] = VertexTable[(i + 1)*slices + j];
			index[1] = NormalTable[(i + 1)*slices + j];
			IndexTable.push_back(index);
		}
	}
}

void Solid::render() 
{
	for (int i = 0; i < FaceTable.size(); i++) {
		glPushMatrix();
			glScalef(scale.x, scale.y, scale.z);
			glRotatef(rotation.y, 0.0, 1.0, 0.0);
			glRotatef(rotation.z, 0.0, 0.0, 1.0);
			glRotatef(rotation.x, 1.0, 0.0, 0.0);
			glTranslatef(position.x, position.y, position.z);
			glBegin(GL_TRIANGLE_FAN);
			for (int j = 0; j < FaceTable[i][0]; j++) {
				Vector3* pt = IndexTable[FaceTable[i][1] + j][0];
				Vector3* n = IndexTable[FaceTable[i][1] + j][1];
				glNormal3f(n->x, n->y, n->z);
				glVertex3f(pt->x, pt->y, pt->z);
			}
			glEnd();
		glPopMatrix();
	}
}