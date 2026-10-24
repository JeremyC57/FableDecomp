// behaviour test for 0x005e1820 (std::fill over pair<unsigned long, CAnimationEntry>)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
static int g_destroys = 0;
struct CIVCountedPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroys; }     // slot 1
    long m_count;                               // +0x04
};

template <class T>
struct CIVCountedPointer {
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

struct CDefString {                             // one word (Ego_r.pdb: TablePos)
    long TablePos;                              // +0x00
};
struct CAnimComponentMap : CIVCountedPointeeBase { };
struct CAnimationEntry {                        // 16 bytes
    long                                 BankIndex;     // +0x00
    CDefString                           AnimName;      // +0x04
    CDefString                           GroupName;     // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0C
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
typedef pair<unsigned long, CAnimationEntry> CAnimationEntryPair;
template void __fastcall fill(CAnimationEntryPair*, CAnimationEntryPair*, const CAnimationEntryPair&);
}

int main()
{
    typedef std::CAnimationEntryPair Pair;
    CAnimComponentMap oldMap, newMap;
    oldMap.m_count = 2; newMap.m_count = 1;
    Pair v[3];
    for (int i = 0; i < 3; ++i) {
        v[i].first = 0; v[i].second.BankIndex = -1; v[i].second.AnimName.TablePos = 0; v[i].second.GroupName.TablePos = 0;
        v[i].second.Components.Object = (i < 2) ? &oldMap : 0;
    }
    Pair val;
    val.first = 77; val.second.BankIndex = 4; val.second.AnimName.TablePos = 11; val.second.GroupName.TablePos = 12;
    val.second.Components.Object = &newMap;
    std::fill(v, v + 2, val);
    int ok = v[0].first == 77 && v[1].second.BankIndex == 4 && v[0].second.AnimName.TablePos == 11 && v[1].second.GroupName.TablePos == 12
          && v[0].second.Components.Object == &newMap && v[1].second.Components.Object == &newMap
          && oldMap.m_count == 0 && g_destroys == 1 && newMap.m_count == 3
          && v[2].first == 0 && v[2].second.Components.Object == 0;
    if (!ok) { printf("005e1820_TEST FAIL\n"); return 1; }
    printf("005e1820_TEST PASS\n");
    return 0;
}