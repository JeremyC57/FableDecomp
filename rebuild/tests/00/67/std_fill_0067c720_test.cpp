// behaviour test for 0x0067c720 (std::fill over CBlurredAttitudesDef)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
static int g_vecAssigns = 0;
namespace std {
template <class T>
struct vector {                                 // STLport vector: 12 bytes
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    vector<T>& operator=(const vector<T>& x) { _M_start = x._M_start; _M_finish = x._M_finish; _M_end_of_storage = x._M_end_of_storage; ++g_vecAssigns; return *this; }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V>
struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class Compare>
struct CVectorMap {                             // retail 16 bytes
    std::vector<std::pair<K, V> > Data;         // +0x00
    Compare                       Comparator;   // +0x0C (empty)
    bool                          Dirty;        // +0x0D
};

struct CResourceList;
struct CResource {                              // 28 bytes
    virtual void v00() {}
    long           m_refCount;                  // +0x04
    CResourceList* ResourceList;                // +0x08
    CResource*     PrevResource;                // +0x0C
    CResource*     NextResource;                // +0x10
    unsigned long  ResourceSize;                // +0x14
    unsigned long  LastUsedFrame;               // +0x18
};
struct CDefinitionManager;
struct CDefString {                             // one word (Ego_r.pdb: TablePos)
    long TablePos;                              // +0x00
};
#pragma pack(push, 1)
struct CDefPointeeBase : CResource {
};
struct CDefClassBase : CDefPointeeBase {        // 0x25 bytes
    struct CSubDefInfo { long m_0; long m_4; };
    CDefinitionManager* PDefManager;            // +0x1C
    unsigned long       GlobalIndex;            // +0x20
    unsigned char       Setup : 1;              // +0x24 bit 0
    unsigned char       Template : 1;           // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1; // +0x24 bit 2
};
struct CParentDefClassBase : CDefClassBase {    // 0x39 bytes in retail
    CDefString InstantiationName;               // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo,
               CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };
struct CBlurredAttitudesDef : CParentDefClassBase {   // 0x48 bytes
    std::vector<EOpinionAttitudeType> Attitudes; // +0x3C
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CBlurredAttitudesDef*, CBlurredAttitudesDef*, const CBlurredAttitudesDef&);
}

int main()
{
    CBlurredAttitudesDef v[3];
    EOpinionAttitudeType att[2];
    std::pair<unsigned long, CDefClassBase::CSubDefInfo> sub[2];
    for (int i = 0; i < 3; ++i) {
        v[i].m_refCount = 1; v[i].ResourceSize = 0; v[i].PDefManager = 0; v[i].GlobalIndex = 0;
        v[i].Setup = 0; v[i].Template = 1; v[i].DefaultValsApplied = 0; v[i].InstantiationName.TablePos = -1;
        v[i].SubDefInfoMap.Data._M_start = 0; v[i].SubDefInfoMap.Dirty = false; v[i].Attitudes._M_start = 0;
    }
    CBlurredAttitudesDef val;
    val.m_refCount = 7; val.ResourceList = 0; val.PrevResource = 0; val.NextResource = 0; val.ResourceSize = 64; val.LastUsedFrame = 3;
    val.PDefManager = (CDefinitionManager*)0; val.GlobalIndex = 42; val.Setup = 1; val.Template = 0; val.DefaultValsApplied = 1;
    val.InstantiationName.TablePos = 1234;
    val.SubDefInfoMap.Data._M_start = sub; val.SubDefInfoMap.Data._M_finish = sub + 2; val.SubDefInfoMap.Data._M_end_of_storage = sub + 2;
    val.SubDefInfoMap.Dirty = true;
    val.Attitudes._M_start = att; val.Attitudes._M_finish = att + 1; val.Attitudes._M_end_of_storage = att + 2;
    std::fill(v, v + 2, val);
    int ok = v[0].m_refCount == 7 && v[1].ResourceSize == 64 && v[1].GlobalIndex == 42
          && v[0].Setup == 1 && v[1].Template == 0 && v[1].DefaultValsApplied == 1
          && v[0].InstantiationName.TablePos == 1234 && v[1].SubDefInfoMap.Data._M_finish == sub + 2
          && v[0].SubDefInfoMap.Dirty && v[1].Attitudes._M_finish == att + 1 && g_vecAssigns == 4
          && v[2].m_refCount == 1 && v[2].Template == 1 && v[2].InstantiationName.TablePos == -1;
#ifdef _MSC_VER
    ok = ok && sizeof(CParentDefClassBase) == 0x39 && sizeof(CBlurredAttitudesDef) == 0x48;   // retail layout (32-bit)
#endif
    if (!ok) { printf("0067c720_TEST FAIL\n"); return 1; }
    printf("0067c720_TEST PASS\n");
    return 0;
}