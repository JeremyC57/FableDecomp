// CTCHeroCentreDoorMarker::UpdateParticleTeleporter() -- keep the "MARKTELEPORTER" particle on the door marker in step with the world flag at +0xd7: kill it when the flag is clear, create and attach it when the flag is set and no live particle exists
// Leaf: UpdateParticleTeleporter
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
//
// Retail 0x0077F7E0, 395 bytes (+5 int3 padding). No direct callers (Ego_r.pdb names only UpdateParticleExperience for this class;
// the catalog label is propagated from the twin 0x0077F970, which differs only in the world flag +0xd6 and the "EXPERIENCE_SPEND"
// literal). The name here is descriptive.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCBase::PeekWorld @ 0x00686A80 (twice, one per arm), CThing::Kill(bool)
// @ 0x004C9B80, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPos @ 0x004C73D0, the `__fastcall` effect factory
// Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (inline GetTC for the particle emitter, interface 0x4b), CTCDParticleEmitter::AttachToThing @ 0x006E0BE0.
// Shape: two arms on IS_THING_ALIVE(PParticle); `PParticle = CIntelligentPointer<CThing>(x);` builds/assigns/destroys a frame
// temporary (no user dtor on CIntelligentPointer); `T* emitter = 0; if (HasTC) emitter = GetTC;` gives the `xor edi,edi` preload,
// HasTC returns the raw masked word (`test ch,8` on +0x28).
// Layout (packed, landed 0x0077F5C0): PParticle +0x15; retail CWorld flags +0xd6 ExperienceSpendingEnabled (PDB +0xe2) and +0xd7
// (PDB +0xe3 SavingEnabled by the same -0xC shift of three 16-byte PDB vectors; unproven, kept descriptive); CThing interface bitset +0x20, interface map +0x44, dead bit 0 of +0x91.
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

CThing* __fastcall Create(const CCharString& defName, const C3DVector& position, bool flag);   // 0x006E0970
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class P> inline bool IS_THING_ALIVE(const P& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

class CWorld { public:
    unsigned char _pad00[0xd6];
    bool ExperienceSpendingEnabled;                     // +0xd6
    bool WorldFlagD7;                                   // +0xd7 (PDB SavingEnabled by offset; unproven)
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
#pragma pack(push, 1)
class CTCHeroCentreDoorMarker : public CTCBase {        // vtable 0x0126B044
public:
    float Radius;                                       // +0x0c
    long  DoorType;                                     // +0x10
    bool  HeroInsideRadius;                             // +0x14
    CIntelligentPointer<CThing> PParticle;              // +0x15
    CIntelligentPointer<CThing> PHeroCentre;            // +0x1d
    void UpdateParticleTeleporter();
};
#pragma pack(pop)

void CTCHeroCentreDoorMarker::UpdateParticleTeleporter()
{
    if (IS_THING_ALIVE(PParticle)) {
        if (!PeekWorld().WorldFlagD7) {
            PParticle.GetPItem()->Kill(true);
            PParticle = CIntelligentPointer<CThing>(0);
        }
    } else if (PeekWorld().WorldFlagD7) {
        PParticle = CIntelligentPointer<CThing>(Create("MARKTELEPORTER", ParentThing->GetPos(), false));
        if (IS_THING_ALIVE(PParticle)) {
            CTCDParticleEmitter* emitter = 0;
            CThing* particle = PParticle.GetPItem();
            if (particle->HasTC(TCI_PARTICLE_EMITTER))
                emitter = ((CTCDParticleEmitter*)&particle->GetTC(TCI_PARTICLE_EMITTER));
            emitter->AttachToThing(*ParentThing, 0x20, "", 0, 0.0f);
        }
    }
}