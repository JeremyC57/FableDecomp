// CTCBoastingPosition::RemoveBoast() -- clear the boast flag, kill the particle effect when it is alive and disable the owner's interface-9 component
// Leaf: RemoveBoast
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782060, length 114 bytes (+14 int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3: alive test, then the kill; the `push 1` precedes the third),
// CThing::Kill(bool) @ 0x004C9B80, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for
// interface type 9 = bit 9 of the bitset dword at +0x20).
// Layout: engine/CTCBoastingPosition.h (Ego_r.pdb): PBoastingPodium +0xC, OrbPosition +0x14, PositionActivated +0x20, BoastIndex
// +0x24, Cost +0x28, BoastOn +0x2C, HeroInsideRadius +0x2D, PParticleEffect +0x2E (really unaligned, so the polymorphic local
// model is `#pragma pack(1)`; the generated header flattens the vptrs). Retail CThing: bitset +0x20, map +0x44, Dead +0x91.
// Shape: as DeactivatePosition (0x00781FE0) without the PositionActivated store and the visibility call; `this` in edi.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetVisible(bool visible);      // slot 28 (+0x70)
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCInterface9 {                          // the owner's interface-type-9 component
    unsigned char _pad00[0x10];
    bool Enabled;                               // +0x10
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
};
class CTCBoastingPosition : public CTCBase {
public:
    CBaseIntelligentPointer PBoastingPodium;    // +0x0c CIntelligentPointer<CThing>
    float OrbPosition[3];                       // +0x14 C3DVector
    bool PositionActivated;                     // +0x20
    unsigned char _pad21[3];
    long BoastIndex;                            // +0x24
    long Cost;                                  // +0x28
    bool BoastOn;                               // +0x2c
    bool HeroInsideRadius;                      // +0x2d
    CBaseIntelligentPointer PParticleEffect;    // +0x2e CIntelligentPointer<CThing> (unaligned)
    void KillParticleEffect()
    {
        if (PParticleEffect.GetPItem() && !PParticleEffect.GetPItem()->IsDead())
            PParticleEffect.GetPItem()->Kill(true);
    }
    void DeactivatePosition();
    void RemoveBoast();
};
#pragma pack(pop)

void CTCBoastingPosition::RemoveBoast()
{
    BoastOn = false;
    KillParticleEffect();
    CTCInterface9* component;
    if (ParentThing->GetInterface(&component, 9))
        component->Enabled = false;
}