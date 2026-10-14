#include <cstdio>
#include <string.h>
#include <new>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
template <class T> struct CArray { T* m_begin; T* m_end; T* m_capacity; CArray() : m_begin(0), m_end(0), m_capacity(0) { } };
struct C3DBoundingSphere { long m_x, m_y, m_z, m_radius; C3DBoundingSphere() : m_x(0), m_y(0), m_z(0), m_radius(0) { } };
struct C3DVertex2; struct C3DVector; struct CTriangleListRef; struct CGroupRef;
namespace N3DPrimitive {
struct CDeviceIndependent : CBase {
    virtual void v00() { }
    CArray<C3DVertex2> Vertices; CArray<CTriangleListRef> TriangleLists; CArray<CGroupRef> Groups; C3DBoundingSphere BoundingSphere; CArray<C3DVector> UTangents; CArray<C3DVector> VTangents;
    CDeviceIndependent() { }
}; }
int main()
{
    unsigned char raw[sizeof(N3DPrimitive::CDeviceIndependent)]; memset(raw, 0xee, sizeof(raw));
    N3DPrimitive::CDeviceIndependent* o = new (raw) N3DPrimitive::CDeviceIndependent;
    for (unsigned i = 4; i < sizeof(raw); ++i) if (raw[i] != 0) { std::printf("00a98100_TEST FAIL\n"); return 1; }
    if (g_base != 1 || sizeof(*o) != 0x50 || (char*)&o->BoundingSphere - (char*)o != 0x28 || (char*)&o->VTangents - (char*)o != 0x44) { std::printf("00a98100_TEST FAIL\n"); return 1; }
    std::printf("00a98100_TEST PASS\n"); return 0;
}
