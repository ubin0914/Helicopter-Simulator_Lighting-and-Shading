#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <ctime>

#include "PublicValue.h"

using namespace std;

class Environment {
private:
	int building[25][25][16] = { 0 };
	vector<Coordinate> taipei_101_vertex;
	vector<vector<int>> taipei_101_face;
	float buildingHeight[19] = { 10, 10, 10, 10, 13, 13, 13, 13, 16, 16, 16, 16, 19, 19, 19, 19, 27, 36, 60 };
	int centerBlockX = 0, centerBlockZ = 0;	//以block為單位的x與z

	Material taipei_101_Mat, buildingsMat, groundMat, coating, lightningRod_Mat;
	Polyhedron taipei_101_Obj, cubeObj;

public:
	Environment() {
		pushSquareVertex(64, 0, 64);
		pushSquareVertex(47, 110, 47);
		pushSquareVertex(48, 113.5, 48);
		vector<pair<float, float>> dxdz = {
			make_pair(36, 41),
			make_pair(41, 41),
			make_pair(41, 36),
			make_pair(46, 36),
			make_pair(46, -36)
		};
		pushMultiSquareVertex(dxdz, 113.5);
		pushMultiSquareVertex(dxdz, 117);
		for (int i = 0; i < 8; i++) {
			vector<pair<float, float>> dxdz = {
				make_pair(33, 38),
				make_pair(38, 38),
				make_pair(38, 33),
				make_pair(43, 33),
				make_pair(43, -33)
			};
			pushMultiSquareVertex(dxdz, 117 + i * 34);
			dxdz = {
				make_pair(38, 43),
				make_pair(43, 43),
				make_pair(43, 38),
				make_pair(48, 38),
				make_pair(48, -38)
			};
			pushMultiSquareVertex(dxdz, 117 + (i + 1) * 34);
		}
		pushSquareVertex(24, 389, 24);
		pushSquareVertex(26, 401, 26);
		pushSquareVertex(24, 401, 24);
		pushSquareVertex(24, 407, 24);
		pushSquareVertex(12, 407, 12);
		pushSquareVertex(16, 434, 16);
		pushSquareVertex(15.5, 434, 15.5);
		pushSquareVertex(16.5, 448.5, 16.5);
		pushSquareVertex(9, 448.5, 9);
		pushSquareVertex(6, 457.5, 6);

		pushFourFace(0, 1);
		pushFourFace(4, 1);
		for (int k = 0; k < 5; k++)
			pushFourFace(12 + k, 5, (k == 4));
		for (int i = 0; i < 8; i++)
			for (int k = 0; k < 5; k++)
				pushFourFace(52 + i * 40 + k, 5, (k == 4));
		pushFourFace(372, 1);
		pushFourFace(380, 1);
		pushFourFace(388, 1);
		pushFourFace(396, 1);
		pushFourFace(404, 1);

		pushCeilingFace(8, 4);
		pushCeilingFace(32, 20);
		for(int i = 0; i < 8; i++)
			pushCeilingFace(72 + i * 40, 20);
		pushCeilingFace(376, 4);
		pushCeilingFace(384, 4);
		pushCeilingFace(392, 4);
		pushCeilingFace(400, 4);
		pushCeilingFace(408, 4);

		initialBuildings();

		taipei_101_Mat.diffuse = new float[] { 0.4, 0.4, 0.4, 1 };
		taipei_101_Mat.ambient = new float[] { 0.25, 0.25, 0.25, 1 };
		taipei_101_Mat.specular = new float[] { 0.7746, 0.7746, 0.7746, 1 };
		taipei_101_Mat.shininess = 0.6 * 128;

		buildingsMat.diffuse = new float[] { 0.55, 0.55, 0.55, 1 };
		buildingsMat.ambient = new float[] { 0, 0, 0, 1 };
		buildingsMat.specular = new float[] { 0.5, 0.5, 0.5, 1 };
		buildingsMat.shininess = 0.25 * 128;

		groundMat.diffuse = new float[] { 0.2, 0.2, 0.2, 1 };
		groundMat.ambient = new float[] { 0.2, 0.2, 0.2, 1 };
		groundMat.specular = new float[] { 0, 0, 0, 1 };
		groundMat.shininess = 8;

		coating.diffuse = new float[] { 1, 1, 0.2, 1 };
		coating.ambient = new float[] { 5, 5, 1, 1 };
		coating.specular = new float[] { 0, 0, 0, 1 };
		coating.shininess = 8;

		lightningRod_Mat.diffuse = new float[] { 0.7, 0.7, 0.7, 1 };
		lightningRod_Mat.ambient = new float[] { 0.4, 0.4, 0.4, 1 };
		lightningRod_Mat.specular = new float[] { 0.7746, 0.7746, 0.7746, 1 };
		lightningRod_Mat.shininess = 0.05 * 128;

		taipei_101_Obj = Polyhedron(taipei_101_Mat, taipei_101_vertex, taipei_101_face);
		cubeObj = Polyhedron(taipei_101_Mat, cubeVertex, cubeFace);
	}
	void initialBuildings() {
		srand(time(NULL));
		for (int i = 0; i < blockNum; i++)
			for (int j = 0; j < blockNum; j++)
				for (int k = 0; k < 16; k++)
					building[i][j][k] = rand() % 19;
	}
	void display() {
		drawGround();
		drawTapei_101();
		
		/*int i = 5, j = 5, k = 0;
		glPushMatrix();
		glTranslated((j - 4) * 100 + (k % 4) * 25 - 37.5, 0, (i - 4) * 100 + (k / 4) * 25 - 37.5);
		glScaled(1, buildingHeight[building[i][j][k]], 1);
		glPolygonMode(GL_FRONT, GL_FILL);
		for each (Face face in buildingFace) {
			face.drawAsRGB(buildingVertex);
		}
		glPopMatrix();*/

		buildingsMat.select();
		glPolygonMode(GL_FRONT, GL_FILL);
		for (int i = -blockNum / 2; i <= blockNum / 2; i++) {
			for (int j = -blockNum / 2; j <= blockNum / 2; j++) {
				float offsetX = (centerBlockX + j) * 110;
				float offsetZ = (centerBlockZ + i) * 110;
				if (!offsetX && !offsetZ) continue;
				int indexX = (centerBlockX + j + 100000 * blockNum) % blockNum;
				int indexZ = (centerBlockZ + i + 100000 * blockNum) % blockNum;
				for (int k = 0; k < 16; k++) {
					float quarterX = (k < 8 ? 25 : -25) + (k % 4 < 2 ? 10 : -10);
					float quarterZ = (k / 4 % 2 ? 20.5 : -20.5) + (k % 2 ? 10 : -10);
					glPushMatrix();
					glTranslated(offsetX + quarterX, 0, offsetZ + quarterZ);
					glScaled(1, buildingHeight[building[indexX][indexZ][k]], 1);
					cubeObj.draw();
					glPopMatrix();
				}
			}
		}
	}
	void updateBlock(int newX, int newZ) {
		if (newX != centerBlockX) {
			int updateX = (centerBlockX + 100000 * blockNum + (newX - centerBlockX == 1 ? -1 : 1) * (blockNum / 2)) % blockNum;
			for (int i = 0; i < blockNum; i++)
				for (int k = 0; k < 16; k++)
					building[updateX][i][k] = rand() % 19;
			centerBlockX = newX;
		}
		if (newZ != centerBlockZ) {
			int updateZ = (centerBlockZ + 100000 * blockNum + (newZ - centerBlockZ == 1 ? -1 : 1) * (blockNum / 2)) % blockNum;
			for (int i = 0; i < blockNum; i++)
				for (int k = 0; k < 16; k++)
					building[i][updateZ][k] = rand() % 19;
			centerBlockZ = newZ;
		}
	}
private:
	void drawGround() {
		groundMat.select();
		glNormal3f(0, 1, 0);
		glBegin(GL_POLYGON);
		glVertex3f(centerBlockX * 110 + 9999, 0, centerBlockZ * 110 - 9999);
		glVertex3f(centerBlockX * 110 - 9999, 0, centerBlockZ * 110 - 9999);
		glVertex3f(centerBlockX * 110 - 9999, 0, centerBlockZ * 110 + 9999);
		glVertex3f(centerBlockX * 110 + 9999, 0, centerBlockZ * 110 + 9999);
		glEnd();
	}
	void drawTapei_101() {
		glPolygonMode(GL_FRONT, GL_FILL);
		taipei_101_Obj.draw();
		//雷達平臺
		glPushMatrix();
			glTranslated(0, 457.5, 0);
			glRotatef(-90, 1, 0, 0);
			GLUquadricObj* cylind = gluNewQuadric();
			gluQuadricDrawStyle(cylind, GLU_FILL);
			gluQuadricNormals(cylind, GLU_SMOOTH);
			gluCylinder(cylind, 3, 5, 4.5, 24, 3);

			glTranslated(0, 0, 4.5);
			GLUquadricObj* ceiling = gluNewQuadric();
			gluQuadricDrawStyle(ceiling, GLU_FILL);
			gluDisk(ceiling, 0, 5, 24, 3);

			coating.select();
			glTranslated(0, 0, 0.02);
			gluDisk(ceiling, 4, 4.2, 24, 3);

			glDisable(GL_CULL_FACE);

			glRotatef(90, 1, 0, 0);
			glNormal3f(0, 1, 0);
			glBegin(GL_POLYGON);
			glVertex3f(-1.9, 0, 2.5);
			glVertex3f(-1.9, 0, -2.5);
			glVertex3f(-1.4, 0, -2.5);
			glVertex3f(-1.4, 0, 2.5);
			glEnd();
			glBegin(GL_POLYGON);
			glVertex3f(1.4, 0, 2.5);
			glVertex3f(1.4, 0, -2.5);
			glVertex3f(1.9, 0, -2.5);
			glVertex3f(1.9, 0, 2.5);
			glEnd();
			glBegin(GL_POLYGON);
			glVertex3f(-1.4, 0, 0.25);
			glVertex3f(-1.4, 0, -0.25);
			glVertex3f(1.4, 0, -0.25);
			glVertex3f(1.4, 0, 0.25);
			glEnd();

			glEnable(GL_CULL_FACE);
		glPopMatrix();
	}
	void pushSquareVertex(float dx, float y, float dz) {
		taipei_101_vertex.push_back({ dx / 2, y, dz / 2 });
		taipei_101_vertex.push_back({ dz / 2, y, -dx /2 });
		taipei_101_vertex.push_back({ -dx / 2, y, -dz / 2 });
		taipei_101_vertex.push_back({ -dz / 2, y, dx / 2 });
	}
	void pushMultiSquareVertex(vector<pair<float, float>> dxdz, float y) {
		for each (pair<float, float> vertex in dxdz)
			taipei_101_vertex.push_back({ vertex.first / 2, y, vertex.second / 2 });
		for each (pair<float, float> vertex in dxdz)
			taipei_101_vertex.push_back({ vertex.second / 2, y, -vertex.first / 2 });
		for each (pair<float, float> vertex in dxdz)
			taipei_101_vertex.push_back({ -vertex.first / 2, y, -vertex.second / 2 });
		for each (pair<float, float> vertex in dxdz)
			taipei_101_vertex.push_back({ -vertex.second / 2, y, vertex.first / 2 });
	}
	void pushFourFace(int start, int interval, bool cross = 1) {
		for (int i = 0; i < 4; i++) {
			int fin = start + 4 * interval;
			taipei_101_face.push_back(	{i * interval + start, 
										(i == 3 && cross ? -1 : i) * interval + start + 1,
										(i == 3 && cross ? -1 : i) * interval + fin + 1,
										i * interval + fin} );
		}
	}
	void pushCeilingFace(int start, int cycle) {
		vector<int> order;
		for (int i = 0; i < cycle; i++)
			order.push_back(start + i);
		taipei_101_face.push_back(order);
	}

	vector<Coordinate> cubeVertex = {
		{9, 0, 9},
		{9, 0, -9},
		{-9, 0, -9},
		{-9, 0, 9},
		{9, 1, 9},
		{9, 1, -9},
		{-9, 1, -9},
		{-9, 1, 9},
	};
	vector<vector<int>> cubeFace = {
		{ 0, 1, 5, 4 },
		{ 1, 2, 6, 5 },
		{ 2, 3, 7, 6 },
		{ 3, 0, 4, 7 },
		{ 4, 5, 6, 7 }
	};
public:
	void drawLightningRod(const Helicopter& helicopter) {
		float dist = Coordinate(helicopter.origin_r.x, 0, helicopter.origin_r.z).getLength();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		lightningRod_Mat.diffuse[3] = (dist > 5) * (dist - 5 > 25 ? 1 : (dist - 5) / 25);
		lightningRod_Mat.select(GL_FRONT_AND_BACK);
		glPushMatrix();
		glTranslated(0, 462, 0);
		glRotatef(-90, 1, 0, 0);
		GLUquadricObj* rod = gluNewQuadric();
		gluQuadricDrawStyle(rod, GLU_FILL);
		gluQuadricNormals(rod, GLU_SMOOTH);
		gluCylinder(rod, 1.5, 0.5, 46.5, 24, 3);
		glPopMatrix();
		glDisable(GL_BLEND);
	}
};