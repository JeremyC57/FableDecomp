#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif
// CFlammableDef::operator=(const CFlammableDef&) -- def copy: CDefClassBase part, fire scalars, fire-effect creation set
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5839, 63 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::map<CDefString,long>::operator= @ 0x004E5878 (labelled "CTCCarryable::operator_assign", propagated).
// Identification: reached through the incremental-link thunk 0x004E5834 (manifest "CFlammableDef::Copy", vtable 0x012429D0);
//   PDB CFlammableDef = CDefClassBase + FireResistance +0x28, MaxDamageCaused, FireDamagePerCycle, FireDamagePeriod (floats)
//   +0x2c..+0x34, EffectCreationSet (CFireEffectCreationSet: vptr, byte +4, ContainmentMap +8) +0x38.
// Shape: the set's assignment is inlined (`__forceinline`; the DDK out-lines the implicit one): its vptr is not copied,
//   the byte at +0x3c is loaded before `add edi,40h` and stored after the `lea ecx`, then the map's out-of-line operator=.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CContainmentMap {                // std::map<CDefString, long, CCompareDefString<long> > (retail STLport _Rb_tree, 12 bytes)
    void* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CContainmentMap& operator=(const CContainmentMap& other);   // 0x004E5878
};

struct CFireEffectCreationSet {         // PDB 0x18 bytes (retail 0x14): vptr, a base byte at +4, ContainmentMap +8
    virtual void v00();
    bool m_baseFlag;                    // +0x04 (unnamed base-class byte in the PDB)
    CContainmentMap ContainmentMap;     // +0x08
    __forceinline CFireEffectCreationSet& operator=(const CFireEffectCreationSet& other)
    {
        m_baseFlag = other.m_baseFlag;
        ContainmentMap = other.ContainmentMap;
        return *this;
    }
};

struct CFlammableDef : CDefClassBase {
    long FireResistance;                // +0x28
    float MaxDamageCaused;              // +0x2c
    float FireDamagePerCycle;           // +0x30
    float FireDamagePeriod;             // +0x34
    CFireEffectCreationSet EffectCreationSet;// +0x38
    CFlammableDef& operator=(const CFlammableDef& other);
};

CFlammableDef& CFlammableDef::operator=(const CFlammableDef& other)
{
    CDefClassBase::operator=(other);
    FireResistance = other.FireResistance;
    MaxDamageCaused = other.MaxDamageCaused;
    FireDamagePerCycle = other.FireDamagePerCycle;
    FireDamagePeriod = other.FireDamagePeriod;
    EffectCreationSet = other.EffectCreationSet;
    return *this;
}


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
void CFireEffectCreationSet::v00() {}
CContainmentMap& CContainmentMap::operator=(const CContainmentMap& other) { m_pHeader = other.m_pHeader; m_nodeCount = other.m_nodeCount; return *this; }

int main()
{
    static CFlammableDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.FireResistance = 30;
    b.MaxDamageCaused = 10.0f;
    b.FireDamagePerCycle = 1.0f;
    b.FireDamagePeriod = 0.5f;
    b.EffectCreationSet.m_baseFlag = true;
    b.EffectCreationSet.ContainmentMap.m_pHeader = (void*)&b;
    b.EffectCreationSet.ContainmentMap.m_nodeCount = 3;
    CFlammableDef* r = &(a = b);
    if (r != &a) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e5839"); return 1; }
    if (!(a.FireResistance == b.FireResistance)) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    if (!(a.MaxDamageCaused == b.MaxDamageCaused)) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    if (!(a.FireDamagePerCycle == b.FireDamagePerCycle)) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    if (!(a.FireDamagePeriod == b.FireDamagePeriod)) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    if (!(a.EffectCreationSet.m_baseFlag == b.EffectCreationSet.m_baseFlag)) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    if (!(a.EffectCreationSet.ContainmentMap.m_pHeader == b.EffectCreationSet.ContainmentMap.m_pHeader)) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    if (!(a.EffectCreationSet.ContainmentMap.m_nodeCount == b.EffectCreationSet.ContainmentMap.m_nodeCount)) { std::printf("004e5839_TEST FAIL\n"); return 1; }
    std::printf("004e5839_TEST PASS\n");
    return 0;
}
