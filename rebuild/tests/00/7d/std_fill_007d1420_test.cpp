// behaviour test for 0x007d1420 (std::fill over CShoveHaPennyRegionDef)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
struct CResourceList;
struct CResource {                              // 28 bytes
    virtual void v00() {}
    long           m_refCount;                  // +0x04 (1 after construction)
    CResourceList* ResourceList;                // +0x08
    CResource*     PrevResource;                // +0x0C
    CResource*     NextResource;                // +0x10
    unsigned long  ResourceSize;                // +0x14
    unsigned long  LastUsedFrame;               // +0x18
};
struct CShoveHaPennyRegionDef : CResource {     // 48 bytes
    long          m_def1c;                      // +0x1C (unnamed in the PDB)
    long          m_def20;                      // +0x20 (unnamed in the PDB)
    unsigned char m_flag0 : 1;                  // +0x24 bit 0
    unsigned char m_flag1 : 1;                  // +0x24 bit 1
    unsigned char m_flag2 : 1;                  // +0x24 bit 2
    float         Start;                        // +0x28
    float         Score;                        // +0x2C
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CShoveHaPennyRegionDef*, CShoveHaPennyRegionDef*, const CShoveHaPennyRegionDef&);
}

int main()
{
    CShoveHaPennyRegionDef v[3];
    for (int i = 0; i < 3; ++i) { v[i].m_refCount = 1; v[i].m_def1c = 0; v[i].m_flag0 = 0; v[i].m_flag1 = 1; v[i].m_flag2 = 0; v[i].Start = 0; v[i].Score = 0; }
    CShoveHaPennyRegionDef val;
    val.m_refCount = 9; val.ResourceList = 0; val.PrevResource = 0; val.NextResource = 0; val.ResourceSize = 40; val.LastUsedFrame = 41;
    val.m_def1c = 5; val.m_def20 = 6; val.m_flag0 = 1; val.m_flag1 = 0; val.m_flag2 = 1; val.Start = 2.0f; val.Score = 3.5f;
    std::fill(v, v + 2, val);
    int ok = v[0].m_refCount == 9 && v[1].ResourceSize == 40 && v[0].m_def20 == 6 && v[1].m_flag0 == 1 && v[1].m_flag1 == 0
          && v[0].m_flag2 == 1 && v[1].Start == 2.0f && v[0].Score == 3.5f && v[2].m_refCount == 1 && v[2].m_flag1 == 1;
    if (!ok) { printf("007d1420_TEST FAIL\n"); return 1; }
    printf("007d1420_TEST PASS\n");
    return 0;
}