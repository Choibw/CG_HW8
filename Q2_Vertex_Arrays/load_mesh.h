#pragma once

#include <vector>
#include <string>

struct Vector3 {
    float x, y, z;
};

struct Triangle {
    unsigned int indices[3];
};

// load_mesh.cpp 에서 정의된 전역 변수들
extern std::vector<Vector3> gPositions;
extern std::vector<Vector3> gNormals;
extern std::vector<Triangle> gTriangles;

// 메쉬 로드 함수
void load_mesh(std::string filename);
