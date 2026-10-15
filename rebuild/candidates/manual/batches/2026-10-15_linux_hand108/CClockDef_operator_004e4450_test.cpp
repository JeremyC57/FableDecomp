#include <cstdio>
// CClockDef::operator=(const CClockDef&) -- def copy: CDefClassBase part, sound map, hand type
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4450, 39 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::map<CCharString,long>::operator= @ 0x00432B38.
// Identification: reached through the incremental-link thunk 0x004E444B (manifest "CClockDef::Copy", vtable 0x01242C80);
//   PDB CClockDef = CDefClassBase + Sound (std::map<CCharString,long>, 12 bytes in retail) +0x28, HandType +0x34.
// Shape: memberwise; the map goes through its out-of-line operator= (labelled "CTCCarryable_assign", propagated).
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CSoundNameMap {                    // std::map<CCharString, long> (retail STLport _Rb_tree, 12 bytes)
    void* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CSoundNameMap& operator=(const CSoundNameMap& other);   // 0x00432B38
};


struct CClockDef : CDefClassBase {
    CSoundNameMap Sound;                // +0x28
    long HandType;                      // +0x34
    CClockDef& operator=(const CClockDef& other);
};

CClockDef& CClockDef::operator=(const CClockDef& other)
{
    CDefClassBase::operator=(other);
    Sound = other.Sound;
    HandType = other.HandType;
    return *this;
}


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
CSoundNameMap& CSoundNameMap::operator=(const CSoundNameMap& other) { m_pHeader = other.m_pHeader; m_nodeCount = other.m_nodeCount; return *this; }

int main()
{
    static CClockDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.Sound.m_pHeader = (void*)&b;
    b.Sound.m_nodeCount = 4;
    b.HandType = 2;
    CClockDef* r = &(a = b);
    if (r != &a) { std::printf("004e4450_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e4450"); return 1; }
    if (!(a.Sound.m_pHeader == b.Sound.m_pHeader)) { std::printf("004e4450_TEST FAIL\n"); return 1; }
    if (!(a.Sound.m_nodeCount == b.Sound.m_nodeCount)) { std::printf("004e4450_TEST FAIL\n"); return 1; }
    if (!(a.HandType == b.HandType)) { std::printf("004e4450_TEST FAIL\n"); return 1; }
    std::printf("004e4450_TEST PASS\n");
    return 0;
}
