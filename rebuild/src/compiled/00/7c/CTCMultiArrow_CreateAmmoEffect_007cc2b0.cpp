// CTCMultiArrow::CreateAmmoEffect() -- count one ammo effect down; look "AMMO_EFFECT" up in the ability def's special-effects def, create that particle effect at the owner, attach a live effect's emitter to the owner and keep the effect in AmmoEffects
// Leaf: CreateAmmoEffect
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Shape: CalcAndLookup (0x00669D30) gets a same-TU out-value-only stand-in body (the 0x008D9740 idiom), so the effect
//   index does not escape and VC7.1 packs the intelligent pointer over the def/string words as retail (3-word frame).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CC2B0, 352 bytes (+ int3 padding), `ret`. Speed TU (esp frame).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, the special-effects def's
//   CalcAndLookup(const CCharString&, long*) @ 0x00669D30 (label "CCRC::CalcAndLookup" is propagated; as the landed
//   0x008C4170), CCharString(const char*, long = -1) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPos
//   @ 0x004C73D0, CreateParticleEffect(long, const C3DVector&) @ 0x0072AB70 (`__fastcall`), CBaseIntelligentPointer
//   ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCDParticleEmitter, type
//   0x4B = bit 11 of CThing+0x28), CTCDParticleEmitter::AttachToThing @ 0x006E0BE0,
//   vector<CIntelligentPointer<CThing> >::push_back @ 0x004AD520; the def's virtual slot 1 at the last release.
// Data: "AMMO_EFFECT" @ 0x0126EDEC, "" @ 0x0122D70E; CIntelligentPointer<CThing> vtable @ 0x01238C6C.
// Shape: the GetInterface result is not tested and the emitter is not pre-set, so the absent path reads the
//   uninitialised out pointer from the frame word shared with the effect index and the lookup key (as the landed
//   FinalCleanup 0x007CC410). The lookup's string is an implicit conversion temporary (the bool result survives its
//   destructor in bl).
// Layout: Ego_r.pdb CTCMultiArrow with the retail 12-byte vector (as the landed siblings 0x007CC230 / 0x007CC060):
//   PDef +0x0C, CreateAmmoEffectCount +0x24, AmmoEffects +0x30.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class CThing;
class CBaseObject;
class CTCBase;
// NF49 (round 7): the def classes come from the shared headers (were local CCharString / CDefPointeeBase / CDefPointer /
// CSpecialEffectsDef / CDefClassBase models with same-TU stand-in GetSubDef / GetEffectFromString bodies over made-up
// FindDef_CDefClassBase_GetSubDef_0 / Lookup_CSpecialEffectsDef_CalcAndLookup callees): the call at 0x00669580 is the member
// template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z (Ego_r.pdb /
// FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom), and
// CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY,
// the 0x007941D0 idiom), so the def handle and the effect index stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CSpecialAbilitiesMultiArrowDef : public CDefClassBase { public: };

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    const C3DVector& GetPos() const;              // 0x004C73D0
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
// NF49 (round 7): CreateParticleEffect 0x0072AB70 is the public static __fastcall
// ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (Ego_r.pdb / FableWin.pdb), declared by
// engine/CTCSpecialAbilities.h; was a free stand-in.
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "engine/CTCSpecialAbilities.h"

class CTCDParticleEmitter {
public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                        // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)
FABLE_FORCEINLINE bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

template <class T> struct CRetailVector {              // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x);                        // 0x004AD520
};

class CTCMultiArrow {
public:
    void*         __vftable;                           // +0x00
    CThing*       ParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CSpecialAbilitiesMultiArrowDef* PDef;              // +0x0c CDefPointer<const CSpecialAbilitiesMultiArrowDef>
    long          NumberOfShots;                       // +0x10
    long          NumberInVolley;                      // +0x14
    long          NumOfVolleys;                        // +0x18
    long          SpellLevel;                          // +0x1c
    bool          RemovedEffect;                       // +0x20
    long          CreateAmmoEffectCount;               // +0x24
    long          CreateAmmoEffectDelay;               // +0x28
    long          CreateAmmoEffectTimer;               // +0x2c
    CRetailVector<CIntelligentPointer<CThing> > AmmoEffects;   // +0x30
    void CreateAmmoEffect();
};

void CTCMultiArrow::CreateAmmoEffect()
{
    --CreateAmmoEffectCount;
    CDefPointer<const CSpecialEffectsDef> effects;
    long effectIndex = 0;
    if (PDef->GetSubDef(&effects)) {
        if (effects->GetEffectFromString("AMMO_EFFECT", effectIndex)) {
            CIntelligentPointer<CThing> effect(CTCSpecialAbilities::CreateParticleEffect(effectIndex, ParentThing->GetPos()));
            if (IS_THING_ALIVE(effect)) {
                CTCDParticleEmitter* emitter;
                ((CThing*)effect.GetPItem())->GetInterface(&emitter, 0x4b);
                if (emitter)
                    emitter->AttachToThing(*ParentThing, 8, "", 0, 0.0f);
            }
            AmmoEffects.push_back(effect);
        }
    }
}