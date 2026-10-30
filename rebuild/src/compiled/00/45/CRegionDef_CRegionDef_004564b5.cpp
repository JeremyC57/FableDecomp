// CRegionDef::CRegionDef() -- construct a region definition: the parent-def base, then the sound map's two empty vector maps and the unset wandering-population script name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004564B5, length 48 bytes. Callees: CParentDefClassBase::CParentDefClassBase
// @ 0x00430370. Vtable: CRegionDef 0x01233AAC (rtti-vtables.tsv).
// Layout: PDB names with the retail offsets (engine headers do not exist for CRegionDef). Retail CParentDefClassBase is
// byte-packed (InstantiationName at +0x25, its CVectorMap SubDefInfoMap at +0x29, as its constructor writes) and ends at
// +0x39, so CRegionDef's own members start at +0x3c: EnvironmentTheme .. CameraManagerSetBuildingIndex +0x3c..+0x4c
// (not initialised), CSoundMap SoundMap +0x50 (CVectorMap SoundMap +0x50, CVectorMap CriteriaMap +0x60), the two
// PreventHero* bools +0x70/+0x71 (not initialised) and CDefString WanderingPopulationScriptDefName +0x74. Retail
// CVectorMap is 16 bytes: the three vector pointers, the empty comparator byte and the Dirty flag at +0x0d (the PDB's
// 20-byte layout carries a 16-byte Dinkumware vector).
// Shape: the body is empty -- only member objects with inline constructors run (each vector map zeroes its pointers and
// its flag through eax, the def string stores -1 as `or dword ptr [esi+74h],-1`); that `or` marks a size-optimised TU.
// Leaf: ??0CRegionDef
#pragma optimize("s", on)

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

template <class K, class V> class CKeyPairCompareLess { };   // empty comparator (1 byte)
template <class K, class V, class C> class CVectorMap { public:   // retail: vector (first/last/end), comparator byte, dirty flag
    V*            m_first;                              // +0x00
    V*            m_last;                               // +0x04
    V*            m_end;                                // +0x08
    C             Compare;                              // +0x0c (empty CKeyPairCompareLess)
    bool          Dirty;                                // +0x0d
    CVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) {}
};

class CSoundPair;
class CDefString { public:
    long TablePos;                                      // +0x00
    CDefString() : TablePos(-1) {}
};

class CSoundMap { public:
    CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> > SoundMap;      // +0x00
    CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> > CriteriaMap;   // +0x10
};

class CRegionDef : public CParentDefClassBase { public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CRegionDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CRegionDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CRegionDef@@UAEKXZ
    long       EnvironmentTheme;                        // +0x3c
    long       InternalEnvironmentTheme;                // +0x40
    long       InternalLightingChannelCount;            // +0x44
    long       CameraManagerSetIndex;                   // +0x48
    long       CameraManagerSetBuildingIndex;           // +0x4c
    CSoundMap  SoundMap;                                // +0x50
    bool       PreventHeroWeaponUse;                    // +0x70
    bool       PreventHeroWillUse;                      // +0x71
    CDefString WanderingPopulationScriptDefName;        // +0x74
    CRegionDef();
};

CRegionDef::CRegionDef()
{
}