// NCreatureMode::CModeJackOfBladesFloat::OnActivate() -- spawn the FLOAT_POWER effect at the creature and attach it to the creature's target cursor point
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9740, 293 bytes, plain `ret` (virtual).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30,
//   CThing::GetPos 0x004C73D0, __fastcall CreateEffectThing 0x006E0880, CBaseIntelligentPointer ctor 0x00A01B10 /
//   SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CTCDParticleEmitter::AttachToThing 0x006E0BE0.
// Layout: PEmitter2 +0x7C (CIntelligentPointer<CThing>); the creature (+0x04) definition +0x70, particle emitter +0x5C.
// Shape: GetSubDef and GetEffectFromString both have same-TU out-slot-only stand-in bodies, so neither the sub-def slot
//   nor the effect type escapes: the slot word is reused by the "FLOAT_POWER" string, the effect-type word by the
//   intelligent-pointer temporary and the bone-name string (frame 0xc); the definition is a counted reference in its own
//   block (add-ref, GetSubDef, release), the effect sub-def is released by hand at the end (known non-null: no test).
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CDefSlot / CCharString / CDefVirtual / CDefBase / CDefPointer / CSpecialEffectsDef / CDefClassBase models with same-TU stand-in bodies over made-up Lookup_CSpecialEffectsDef_GetEffectFromString / Find_CDefClassBase_GetSubDef callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), and CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body
// (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, the 0x007941D0 idiom).
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
#include "engine/C3DVector.h"
class CThing;
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
private: public:
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
};
class CThing { public:
    unsigned char        _pad00[0x20];
    unsigned long        m_presentBits[9];      // +0x20 std::bitset of interface types
    unsigned char        _pad44[0x5c - 0x44];
    CTCDParticleEmitter* m_pParticleEmitter;    // +0x5c
    unsigned char        _pad60[0x70 - 0x60];
    CDefPointer<CDefClassBase> m_pDef;          // +0x70 (NF49: the def handle; the counted copy below is its copy constructor)
    unsigned char        _pad74[0x91 - 0x74];
    unsigned char        m_flags91;             // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    const C3DVector& GetPos() const;       // 0x004C73D0
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    FABLE_FORCEINLINE CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CModeJackOfBladesFloat {
    void*                    __vftable;         // +0x00
    CThing*                  PCreature;         // +0x04
    unsigned char            _pad08[0x74 - 0x08];
    CIntelligentPointer<CThing> PEmitter;          // +0x74
    CIntelligentPointer<CThing> PEmitter2;         // +0x7c
    void OnActivate();
};

void CModeJackOfBladesFloat::OnActivate()
{
    CDefPointer<const CSpecialEffectsDef> effectsSlot;   // NF49: the shared handle; its destructor is the release the row wrote by hand
    long effectType = 0;
    {
        CDefPointer<CDefClassBase> def(PCreature->m_pDef);
        def->GetSubDef(&effectsSlot);
    }
    const CSpecialEffectsDef* effects = effectsSlot.Get();
    if (effects) {
        if (effects->GetEffectFromString("FLOAT_POWER", effectType)) {
            CThing* effect = CTCDParticleEmitter::Create(effectType, *(&PCreature->GetPos()), false);
            if (effect && !(effect->m_flags91 & 1)) {
                PEmitter2 = effect;
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = effect->m_pParticleEmitter;
                    emitter->AttachToThing(*PCreature, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
    }
}