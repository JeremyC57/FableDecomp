#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <new>
namespace NDisplayView {
struct CDisplayEngine { unsigned char _pad00[0x2c]; void* m_pCameraManager; };
static CDisplayEngine g_engine; static int g_getCalls = 0;
struct CView3DBase {
    virtual void v00() { }
    void* m_pCameraManager; unsigned char _pad08[0x10]; void* m_pTarget; unsigned char m_targetLocked; unsigned char _pad1d[0x27];
    unsigned char m_flag44; unsigned char m_flag45; unsigned char _pad46[0x36]; unsigned char m_flag7c; unsigned char m_flag7d;
    CDisplayEngine* GetDisplayEngine() { ++g_getCalls; return &g_engine; }
    CView3DBase() : m_pTarget(0), m_targetLocked(0), m_flag44(0), m_flag45(0), m_flag7c(0), m_flag7d(0) { m_pCameraManager = GetDisplayEngine()->m_pCameraManager; }
};
struct CView3DThirdPerson : CView3DBase { virtual void v00() { } CView3DThirdPerson() : CView3DBase() { } };
}
int main()
{
    int camera; NDisplayView::g_engine.m_pCameraManager = &camera;
    unsigned char raw[sizeof(NDisplayView::CView3DThirdPerson)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    NDisplayView::CView3DThirdPerson* v = new (raw) NDisplayView::CView3DThirdPerson();
    if (NDisplayView::g_getCalls != 1 || v->m_pCameraManager != &camera || v->m_pTarget != 0 || v->m_targetLocked != 0 || v->m_flag44 != 0 || v->m_flag45 != 0 || v->m_flag7c != 0 || v->m_flag7d != 0 || v->_pad08[0] != 0xAA) { std::printf("0069e940_TEST FAIL\n"); return 1; }
    std::printf("0069e940_TEST PASS\n"); return 0;
}