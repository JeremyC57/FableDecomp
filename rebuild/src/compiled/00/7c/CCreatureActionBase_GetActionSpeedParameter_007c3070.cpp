// CCreatureActionBase::GetActionSpeedParameter() -- the creature's "SOUL_EFFECT" sub-definition parameter: looks the sub-def up through the creature, reads the named value out of it (0 when there is no such sub-def) and releases the def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C3070, length 112 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CDefClassBase::GetSubDef<T>(CDefPointer<T>&) @ 0x00669580 (on the
// creature read straight from the intelligent pointer's item word at +0xc: the inline operator->), the sub-def's value lookup
// (const CCharString&, long*) @ 0x00669D30 (manifest "CCRC::CalcAndLookup" is propagated), CCharString::~CCharString @ 0x0099EAE0,
// then the def's release (`dec [p+4] ; jne ; call [vt+4]`) written by hand on a pointer local (GOTCHAS wave 25).
// Shape: the string is a named local built first (`push -1 ; push offset` before the frame stores); the CDefPointer word and the
// result are zeroed in the frame (the result through edi, which also carries the final return); the def pointer is copied into
// esi right after GetSubDef, before the test, and released last; the module is a size TU (the counted-def release stays out of line).
// Layout: engine/CCreatureActionBase.h (PDB) agrees on PCreature at +8 (item at +0xc); `this` is modelled locally.#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CSubDef / CCharString / CDefPointer / CSpecialEffectsDef models with same-TU stand-in bodies over made-up FindDef_CThing_GetSubDef_0 callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"



// NF49: retail passes the intelligent pointer's item word (+0x0C, read inline) straight to GetSubDef<CSpecialEffectsDef> as `this`.
// The landed model gave CThing a GetSubDef member for that; the true receiver type is unproven, so the row now reads the item as the
// shared CDefClassBase through a pointer cast (same bytes; a coordinator judgment, listed in NF49's NOTES).
class CThing;

struct CIntelligentPointer {                // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* operator->() const { return m_pItem; }   // inline: the item word read directly (no GetPItem call in this TU)
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    long GetActionSpeedParameter();
};

long CCreatureActionBase::GetActionSpeedParameter()
{
    CDefPointer<const CSpecialEffectsDef> def;   // NF49: the shared handle; its destructor is the release the row wrote by hand
    long result = 0;                        // the register result (edi), whose zero also initialises the out word
    long value = 0;                         // the addressed out word (frame slot)
    {
        CCharString name("SOUL_EFFECT", -1);
        bool found = ((const CDefClassBase*)PCreature.operator->())->GetSubDef(&def);   // NF49: receiver as the row modelled it (note above)
        if (found) {
            def->GetEffectFromString(name, *(&value));
            result = value;                 // read into edi before the string destructor and the release
        }
    }
    return result;
}