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

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

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


class CExperienceDef : public CDefClassBase { public:
    long OrbCombiningParticleEffect;    // +0x28
    CLongFloatMap ParticleScalesPerExperiencePoints;// +0x2c
    CLongLongMap PickupParticleEffectsPerExperiencePoints;// +0x38
    CLongFloatMap LifeTimeForDistanceThresholds;// +0x44
    CExperienceDef& operator=(const CExperienceDef& other);
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