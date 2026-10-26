// behaviour test for 0x00a335e0 (std::fill over pair<long, C3DMeshVolume>)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
static int g_destroyed = 0;
static void __fastcall OnDestroy(void*) { ++g_destroyed; }
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);     // +0x04
    void* m_pObject;                            // +0x08
};

template <class T>
class CCountedPointer { public:
    T* Data;                                    // +0x00
    SCountBlock* Info;                          // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* info = other.Info;         // both source words first (retail: edi / the frame word)
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};

class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString& operator=(const CCharString& other) { m_pStringData = other.m_pStringData; ++g_stringAssigns; return *this; }
};

class CContainmentVolume;
class C3DMeshVolume { public:                          // 12 bytes
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<long, C3DMeshVolume> CMeshVolumeEntry;
template void __fastcall fill(CMeshVolumeEntry*, CMeshVolumeEntry*, const CMeshVolumeEntry&);
}

int main()
{
    SCountBlock* oldA = new SCountBlock; oldA->m_references = 1; oldA->m_pfnDestroy = &OnDestroy; oldA->m_pObject = 0;
    SCountBlock* oldB = new SCountBlock; oldB->m_references = 2; oldB->m_pfnDestroy = &OnDestroy; oldB->m_pObject = 0;
    SCountBlock* blk = new SCountBlock;  blk->m_references = 1;  blk->m_pfnDestroy = &OnDestroy;  blk->m_pObject = 0;
    static char s0, s1; static int obj;
    std::CMeshVolumeEntry v[3];
    v[0].second.ConvexVolume.Info = oldA; v[1].second.ConvexVolume.Info = oldB; v[2].second.ConvexVolume.Info = blk;
    for (int i = 0; i < 3; ++i) { v[i].first = i; v[i].second.Name.m_pStringData = &s0; v[i].second.ConvexVolume.Data = 0; }
    std::CMeshVolumeEntry val; val.first = 9; val.second.Name.m_pStringData = &s1;
    val.second.ConvexVolume.Data = (CContainmentVolume*)&obj; val.second.ConvexVolume.Info = blk;
    std::fill(v, v + 3, val);   // v[2] already shares blk: no change for it
    int ok = g_destroyed == 1 && oldB->m_references == 1 && blk->m_references == 3 && g_stringAssigns == 3
          && v[0].first == 9 && v[2].first == 9 && v[1].second.Name.m_pStringData == &s1
          && v[0].second.ConvexVolume.Data == (CContainmentVolume*)&obj && v[1].second.ConvexVolume.Info == blk;
    delete oldB; delete blk;
    if (!ok) { printf("00a335e0_TEST FAIL\n"); return 1; }
    printf("00a335e0_TEST PASS\n");
    return 0;
}