#include <cstdio>
static int g_ctors = 0;
struct CTransform2D { float m_values[8]; CTransform2D() { ++g_ctors; for (int i = 0; i < 8; ++i) m_values[i] = (float)i; } };
struct CMesh { CTransform2D GetObjectTransform() const { CTransform2D t; return t; } };
int main() { CMesh m; CTransform2D t = m.GetObjectTransform(); if (g_ctors < 1 || t.m_values[7] != 7.0f || t.m_values[0] != 0.0f) { std::printf("00a60860_TEST FAIL\n"); return 1; } std::printf("00a60860_TEST PASS\n"); return 0; }