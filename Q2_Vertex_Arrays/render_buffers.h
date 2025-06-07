#pragma once

#include <GL/glew.h>
#include <vector>

// Q2.cpp 한 군데에서만 정의(allocate)하고,
// 다른 파일에선 extern으로 참조만 하도록.

extern GLuint gVAO;
extern GLuint gVBO_positions;
extern GLuint gVBO_normals;
extern GLuint gEBO;

// bunny 인덱스 배열
extern std::vector<unsigned int> gIndices;
