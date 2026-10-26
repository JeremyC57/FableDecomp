#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; C2DVector() { } C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } }; struct CMovie { unsigned char _pad[0x5c]; C2DVector m_scale; unsigned char _p[0x68]; C2DVector m_size; C2DVector GetSize() const { C2DVector s = m_size; s.X = s.X * m_scale.X; s.Y = s.Y * m_scale.Y; return s; } };

int main() {
    CMovie* m = new CMovie; m->m_scale.X = 2; m->m_scale.Y = 3; m->m_size.X = 10; m->m_size.Y = 20; C2DVector r = m->GetSize(); if (r.X != 20.0f || r.Y != 60.0f) { std::printf("00547be0_TEST FAIL\n"); return 1; } delete m;
    std::printf("00547be0_TEST PASS\n"); return 0;
}