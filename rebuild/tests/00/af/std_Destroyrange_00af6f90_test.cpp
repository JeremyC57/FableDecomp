// behaviour test for 0x00af6f90 (_Destroy_range over SComponentPair records)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_destroyed = 0, g_stringsFreed = 0, g_basesDestroyed = 0;
static void __fastcall OnDestroy(void*) { ++g_destroyed; }
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);     // +0x04
    void* m_pObject;                            // +0x08
};

template <class T>
struct CCountedPointer {
    T* Data;                                    // +0x00
    SCountBlock* Info;                          // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CBaseClass {
    virtual ~CBaseClass() { ++g_basesDestroyed; }
};
namespace NParticleEngine {
enum EPSCType { PSC_NONE };
class CPSCBase;
struct CComponentInstance : CBaseClass {        // 20 bytes
    CCountedPointer<CPSCBase> Component;        // +0x04
    bool                      Enabled;          // +0x0C
    unsigned long             InstanceName;     // +0x10
};
struct SComponentPair {                         // std::pair<EPSCType, CComponentInstance>, 24 bytes
    EPSCType           first;                   // +0x00
    CComponentInstance second;                  // +0x04
};
}
using NParticleEngine::SComponentPair;

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
}

int main()
{
    static char dummy;
    // raw storage: the elements are built field by field and destroyed only through _Destroy_range
    static double storage[64];
    SComponentPair* e = (SComponentPair*)storage;
    for (int i = 0; i < 3; ++i) {
        e[i].second.Component.Data = 0;
        e[i].second.Component.Info = new SCountBlock; e[i].second.Component.Info->m_references = (i == 1) ? 2 : 1;
        e[i].second.Component.Info->m_pfnDestroy = &OnDestroy; e[i].second.Component.Info->m_pObject = &e[i];
    }
    SCountBlock* kept = e[1].second.Component.Info;
    std::allocator<SComponentPair> al;
    std::_Destroy_range(e, e, al);                // empty range: nothing happens
    if (g_destroyed != 0 || g_stringsFreed != 0) { printf("00af6f90_TEST FAIL\n"); return 1; }
    std::_Destroy_range(e, e + 3, al);
    int ok = g_destroyed == 2 && g_stringsFreed == 0 && g_basesDestroyed == 3 && e[0].second.Component.Info == 0 && e[1].second.Component.Info == 0 && e[2].second.Component.Info == 0 && e[0].second.Component.Data == 0 && kept->m_references == 1;
    delete kept;
    if (!ok) { printf("00af6f90_TEST FAIL\n"); return 1; }
    printf("00af6f90_TEST PASS\n");
    return 0;
}