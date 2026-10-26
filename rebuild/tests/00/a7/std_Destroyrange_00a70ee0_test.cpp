// behaviour test for 0x00a70ee0 (_Destroy_range over SMeshVolumePair records)
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
class CCountedPointer { public:
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

class CCharString { public:
    void* m_pStringData;                        // +0x00
    ~CCharString() { if (m_pStringData) ++g_stringsFreed; m_pStringData = 0; }
};

class CContainmentVolume;
class C3DMeshVolume { public:                          // 12 bytes
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};
struct SMeshVolumePair {                        // std::pair<CCharString, C3DMeshVolume>, 16 bytes
    CCharString   first;                        // +0x00
    C3DMeshVolume second;                       // +0x04
};

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
    SMeshVolumePair* e = (SMeshVolumePair*)storage;
    for (int i = 0; i < 3; ++i) {
        e[i].first.m_pStringData = &dummy;
        e[i].second.Name.m_pStringData = &dummy;
        e[i].second.ConvexVolume.Data = 0;
        e[i].second.ConvexVolume.Info = new SCountBlock; e[i].second.ConvexVolume.Info->m_references = (i == 1) ? 2 : 1;
        e[i].second.ConvexVolume.Info->m_pfnDestroy = &OnDestroy; e[i].second.ConvexVolume.Info->m_pObject = &e[i];
    }
    SCountBlock* kept = e[1].second.ConvexVolume.Info;
    std::allocator<SMeshVolumePair> al;
    std::_Destroy_range(e, e, al);                // empty range: nothing happens
    if (g_destroyed != 0 || g_stringsFreed != 0) { printf("00a70ee0_TEST FAIL\n"); return 1; }
    std::_Destroy_range(e, e + 3, al);
    int ok = g_destroyed == 2 && g_stringsFreed == 6 && e[0].second.ConvexVolume.Info == 0 && e[1].second.ConvexVolume.Info == 0 && e[2].second.ConvexVolume.Info == 0 && e[0].second.ConvexVolume.Data == 0 && kept->m_references == 1;
    delete kept;
    if (!ok) { printf("00a70ee0_TEST FAIL\n"); return 1; }
    printf("00a70ee0_TEST PASS\n");
    return 0;
}