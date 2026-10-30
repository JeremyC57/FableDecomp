// CExperienceDef::operator=(const CExperienceDef&) -- def copy: CDefClassBase part, orb effect, three experience maps
// Leaf: ??4CExperienceDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4D88, 63 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::map<long,float>::operator= @ 0x004E4EEC, std::map<long,long>::operator= @ 0x004E4DC7.
// Identification: PDB CExperienceDef = CDefClassBase + OrbCombiningParticleEffect +0x28,
//   ParticleScalesPerExperiencePoints (map<long,float>) +0x2c, PickupParticleEffectsPerExperiencePoints (map<long,long>) +0x38,
//   LifeTimeForDistanceThresholds (map<long,float>) +0x44 (retail maps are 12 bytes, PDB 16). The two map<long,float>
//   members share one out-of-line assignment.
// Shape: memberwise; the last member address is formed by `add edi,44h` (natural for the final call).
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CLongFloatMap {                    // std::map<long, float> (retail STLport _Rb_tree, 12 bytes)
    void* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CLongFloatMap& operator=(const CLongFloatMap& other);   // 0x004E4EEC
};


struct CLongLongMap {                    // std::map<long, long> (retail STLport _Rb_tree, 12 bytes)
    void* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CLongLongMap& operator=(const CLongLongMap& other);   // 0x004E4DC7
};


// def vtable 0x01242104: slot 0 0x004E27C4 (implicit inline dtor); real overrides from the def vtable scan
class CExperienceDef : public CDefClassBase { public:
    long OrbCombiningParticleEffect;    // +0x28
    CLongFloatMap ParticleScalesPerExperiencePoints;// +0x2c
    CLongLongMap PickupParticleEffectsPerExperiencePoints;// +0x38
    CLongFloatMap LifeTimeForDistanceThresholds;// +0x44
    CExperienceDef& operator=(const CExperienceDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E5011
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E4D83
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004E088A
};

CExperienceDef& CExperienceDef::operator=(const CExperienceDef& other)
{
    CDefClassBase::operator=(other);
    OrbCombiningParticleEffect = other.OrbCombiningParticleEffect;
    ParticleScalesPerExperiencePoints = other.ParticleScalesPerExperiencePoints;
    PickupParticleEffectsPerExperiencePoints = other.PickupParticleEffectsPerExperiencePoints;
    LifeTimeForDistanceThresholds = other.LifeTimeForDistanceThresholds;
    return *this;
}