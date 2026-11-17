// NCreatureMode::CModeScreamerSuperAttack::OnActivate() -- the stand-mode activation, suspend the creature's animation when it is in state 1, then (while the target is alive) look up the "SUPER_ATTACK_BEAM" effect in the thing definition's special-effects sub-def, spawn it at the creature and keep it in PLightningBeamParticleEmitter
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D8330, 285 bytes, plain `ret` (virtual).
// Callees: CModeStandBase::OnActivate 0x008621A0 (qualified base call, first), CThingAICreature::SuspendAI(bool,
//   long) 0x00833A10, CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0x74, twice), CDefClassBase::GetSubDef<
//   CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>&) 0x00669580, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30 (result
//   ignored), CThing::GetPos 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Layout: Ego_r.pdb CModeScreamerSuperAttack (0x84): PTarget +0x74, PLightningBeamParticleEmitter +0x7C; CModeStandBase
//   ParentThing +0x04. Thing (retail offsets): state byte +0x10, PBaseDef +0x70, Dead bit 0 of +0x91.
// Shape: the def copy is add-ref'd around GetSubDef and released; the effect sub-def is released by hand at the end. The
// effect-type long is declared before the sub-def slot, and GetEffectFromString gets a same-TU stand-in body (writes only the
// `long&`), so VC7.1 knows the word does not escape and stores its zero first, after the def's null test, as retail.
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CCharString / CDefVirtual / CDefBase / CDefPointer / CSpecialEffectsDef / CDefClassBase models with same-TU stand-in bodies over made-up Find_CSpecialEffectsDef_GetEffectFromString / LookupSpecialEffectsDef callees): the call at
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


static FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
#pragma auto_inline(on)

struct CThingAnimatable {
};
class CThing : public CThingAnimatable { public:
    unsigned char  _pad00[0x10];
    unsigned char  State;                       // +0x10
    unsigned char  _pad11[0x70 - 0x11];
    CDefClassBase* PBaseDef;                    // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  Dead : 1;                    // +0x91 bit 0
    const C3DVector& GetPos() const;       // 0x004C73D0
    void GetSpecialEffectsDef(CDefPointer<const CSpecialEffectsDef>& out) const
    {
        CDefClassBase* def = PBaseDef;
        if (def)
            ++def->IVRefCount;
        def->GetSubDef(&out);
        if (def && --def->IVRefCount == 0)
            def->Delete();
    }
};

class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

struct CModeStandBase {                         // retail sizeof 0x68
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnActivate();                  // slot 5, 0x008621A0
    CThing*       ParentThing;                  // +0x04
    unsigned char _body[0x60];
};

namespace NCreatureMode {

struct CModeScreamerSuperAttack : CModeStandBase {
    unsigned char            _pad68[0x74 - 0x68];
    CBaseIntelligentPointer  PTarget;           // +0x74 CIntelligentPointer<const CThing>
    CIntelligentPointerThing PLightningBeamParticleEmitter;   // +0x7C
    virtual void OnActivate();
};

void CModeScreamerSuperAttack::OnActivate()
{
    CModeStandBase::OnActivate();
    if (ParentThing->State == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(true, (void*)((long)this));
    CBaseIntelligentPointer* target = &PTarget;
    if (((CThing*)target->GetPItem()) == 0 || ((CThing*)target->GetPItem())->Dead)
        return;
    long effectType = 0;
    CDefPointer<const CSpecialEffectsDef> slot;
    ParentThing->GetSpecialEffectsDef(slot);
    slot.Get()->GetEffectFromString("SUPER_ATTACK_BEAM", effectType);
    CThing* beam = CTCDParticleEmitter::Create(effectType, *(&ParentThing->GetPos()), false);
    if (beam && !beam->Dead)
        PLightningBeamParticleEmitter = CIntelligentPointerThing(beam);
}

}