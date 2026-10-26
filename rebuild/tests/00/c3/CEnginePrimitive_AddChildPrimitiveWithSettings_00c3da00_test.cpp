#include <cstdio>
struct CEngineInternalPrimitiveBase_00c3da00 { long m_error; long AddChildPrimitive(void*, long) { return m_error; } };
static long g_lastError_0143bb48 = 0; static long g_reported = 0; static void Report(long e) { g_reported = e; } static void (*g_pfnReport_0143bbd4)(long) = &Report; static void* g_applied = 0;
class CEnginePrimitive { public: long Type; CEngineInternalPrimitiveBase_00c3da00* m_pInternal; unsigned char _pad[0x9c]; unsigned long m_dirtyFlags; void ApplySettings(void* c) { g_applied = c; }
    bool AddChildPrimitiveWithSettings(void* child, long settings) { long error = m_pInternal->AddChildPrimitive(child, settings); if (!error) { ApplySettings(child); m_dirtyFlags |= 0x8e02; return true; }
        if (!g_lastError_0143bb48) { g_lastError_0143bb48 = error; if (g_pfnReport_0143bbd4) g_pfnReport_0143bbd4(error); } return false; } };
int main() {
    CEnginePrimitive* p = (CEnginePrimitive*)new char[sizeof(CEnginePrimitive)]; CEngineInternalPrimitiveBase_00c3da00 in; in.m_error = 0; p->m_pInternal = &in; p->m_dirtyFlags = 0x1; int child = 0;
    if (!p->AddChildPrimitiveWithSettings(&child, 3) || g_applied != &child || p->m_dirtyFlags != 0x8e03 || g_lastError_0143bb48 != 0) { std::printf("00c3da00_TEST FAIL\n"); return 1; }
    in.m_error = 7; if (p->AddChildPrimitiveWithSettings(&child, 3) || g_lastError_0143bb48 != 7 || g_reported != 7 || p->m_dirtyFlags != 0x8e03) { std::printf("00c3da00_TEST FAIL\n"); return 1; }
    in.m_error = 9; if (p->AddChildPrimitiveWithSettings(&child, 3) || g_lastError_0143bb48 != 7 || g_reported != 7) { std::printf("00c3da00_TEST FAIL\n"); return 1; }
    delete[] (char*)p;
    std::printf("00c3da00_TEST PASS\n"); return 0;
}