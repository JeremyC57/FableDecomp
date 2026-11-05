// CTCBoastingPosition::RemoveBoast() -- clear the boast flag, kill the particle effect when it is alive and disable the owner's interface-9 component
// Leaf: RemoveBoast
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782060, length 114 bytes (+14 int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3: alive test, then the kill; the `push 1` precedes the third),
// CThing::Kill(bool) @ 0x004C9B80, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for
// interface type 9 = bit 9 of the bitset dword at +0x20).
// Layout: engine/CTCBoastingPosition.h (Ego_r.pdb): PBoastingPodium +0xC, OrbPosition +0x14, PositionActivated +0x20, BoastIndex
// +0x24, Cost +0x28, BoastOn +0x2C, HeroInsideRadius +0x2D, PParticleEffect +0x2E (really unaligned, so the polymorphic local
// model is `#pragma pack(1)`; the generated header flattens the vptrs). Retail CThing: bitset +0x20, map +0x44, Dead +0x91.
// Shape: as DeactivatePosition (0x00781FE0) without the PositionActivated store and the visibility call; `this` in edi.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCInterface9 {                          // the owner's interface-type-9 component
    unsigned char _pad00[0x10];
    bool Enabled;                               // +0x10
};
#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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
    CIntelligentPointer<CThing> PBoastingPodium;    // +0x0c CIntelligentPointer<CThing>
    float OrbPosition[3];                       // +0x14 C3DVector
    bool PositionActivated;                     // +0x20
    unsigned char _pad21[3];
    long BoastIndex;                            // +0x24
    long Cost;                                  // +0x28
    bool BoastOn;                               // +0x2c
    bool HeroInsideRadius;                      // +0x2d
    CIntelligentPointer<CThing> PParticleEffect;    // +0x2e CIntelligentPointer<CThing> (unaligned)
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
    if (ParentThing->GetTC(TCI_TARGETED, &component))
        component->Enabled = false;
}