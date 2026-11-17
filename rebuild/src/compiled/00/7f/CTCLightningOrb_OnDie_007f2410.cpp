// CTCLightningOrb::OnDie() -- when the orb dies, spawns its definition's LIGHTNING_ORB_HIT special effect at the orb's position (when the def's special-effects sub-def maps that name)
// Leaf: OnDie
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F2410, length 208 bytes (+ int3 padding); speed TU (no frame pointer).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>&) @ 0x00669580, CCharString(const char*, long)
// @ 0x0099EBF0 ("LIGHTNING_ORB_HIT" 0x01271D48) and ~CCharString @ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
// long&) const @ 0x00669D30 (catalog "CCRC::CalcAndLookup"), CThing::GetPos @ 0x004C73D0, CreateEffectThing(long, const C3DVector*,
// bool) @ 0x006E0880 (`__fastcall`), the defs' virtual slot 1 on release. CThing::PDef +0x70; ParentThing +0x04.
// Shape: the thing's definition taken by value (CThing::GetPDef: the copy's addref and its release at the end of the condition) and
// the `&&` string temporary; the effects sub-def is a zeroed out slot released by hand (null-tested) after the spawn. GetSubDef and
// GetEffectFromString get same-TU stand-in bodies that write only the out slot / the `long&`, so VC7.1 knows neither address
// escapes; that orders the zero stores of the effect-type word and the out slot as in retail.
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CDefPointerSlot / CCharString / CDefBase / CDefPointer / CSpecialEffectsDef / CDefClassBase models with same-TU stand-in bodies over made-up Find_CSpecialEffectsDef_GetEffectFromString / Find_CDefClassBase_GetSubDef callees): the call at
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
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;             // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
    const C3DVector& GetPos() const;        // 0x004C73D0
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};

class CTCLightningOrb { public:
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                   // +0x04
    void OnDie();
};
void CTCLightningOrb::OnDie()
{
    CDefPointer<const CSpecialEffectsDef> effectsSlot;   // NF49: the shared handle; its destructor is the release the row wrote by hand
    long effectType = 0;
    bool found = ParentThing->GetPDef()->GetSubDef(&effectsSlot) && effectsSlot.Get()->GetEffectFromString("LIGHTNING_ORB_HIT", effectType);
    if (found)
        CTCDParticleEmitter::Create(effectType, *(&ParentThing->GetPos()), false);
}