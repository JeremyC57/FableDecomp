// behaviour test for 0x00af6d90 (std::fill over pair<EPSCType, CComponentInstance>)
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

class CBaseClass { public:
    virtual ~CBaseClass() {}
};
namespace NParticleEngine {
enum EPSCType { PSC_NONE };
class CPSCBase;
class CComponentInstance : public CBaseClass { public:        // 20 bytes
    CCountedPointer<CPSCBase> Component;        // +0x04
    bool                      Enabled;          // +0x0C
    unsigned long             InstanceName;     // +0x10
};
}
using NParticleEngine::CComponentInstance;
using NParticleEngine::EPSCType;

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
typedef pair<EPSCType, CComponentInstance> CComponentEntry;
template void __fastcall fill(CComponentEntry*, CComponentEntry*, const CComponentEntry&);
}

int main()
{
    SCountBlock* oldA = new SCountBlock; oldA->m_references = 1; oldA->m_pfnDestroy = &OnDestroy; oldA->m_pObject = 0;
    SCountBlock* oldB = new SCountBlock; oldB->m_references = 2; oldB->m_pfnDestroy = &OnDestroy; oldB->m_pObject = 0;
    SCountBlock* blk = new SCountBlock;  blk->m_references = 1;  blk->m_pfnDestroy = &OnDestroy;  blk->m_pObject = 0;
    static int obj;
    std::CComponentEntry v[3];
    v[0].second.Component.Info = oldA; v[1].second.Component.Info = oldB; v[2].second.Component.Info = 0;
    for (int i = 0; i < 3; ++i) { v[i].first = NParticleEngine::PSC_NONE; v[i].second.Component.Data = 0; v[i].second.Enabled = false; v[i].second.InstanceName = 0; }
    std::CComponentEntry val; val.first = (EPSCType)4; val.second.Component.Data = (NParticleEngine::CPSCBase*)&obj;
    val.second.Component.Info = blk; val.second.Enabled = true; val.second.InstanceName = 0x1234;
    std::fill(v, v + 2, val);
    int ok = g_destroyed == 1 && oldB->m_references == 1 && blk->m_references == 3 && v[0].first == 4 && v[1].second.Enabled
          && v[1].second.InstanceName == 0x1234 && v[0].second.Component.Info == blk && v[2].second.Component.Info == 0 && !v[2].second.Enabled;
    val.second.Component.Info = 0; val.second.Component.Data = 0;
    delete oldB; delete blk;
    if (!ok) { printf("00af6d90_TEST FAIL\n"); return 1; }
    printf("00af6d90_TEST PASS\n");
    return 0;
}