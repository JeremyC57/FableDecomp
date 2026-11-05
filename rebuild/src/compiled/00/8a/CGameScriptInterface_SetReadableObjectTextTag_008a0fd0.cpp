// CGameScriptInterface::SetReadableObjectTextTag(const CScriptThing& thing, const CCharString& tag) const -- give a live readable object (interface 0xd) a new text tag
// Leaf: ?SetReadableObjectTextTag@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008A0FD0, 107 bytes, `ret 8`; Ego_r.pdb same symbol.
// Body: t = thing.GetPThing() (slot 11); if t, not dead (bit 0 of +0x91) and holding interface 0xd (bit 13 of
// UsingComponents word 0, +0x20): r = GetTC (non-const LowerBound 0x0040F020, key in the dead `thing` slot; no null
// test on r); r->SetTextTag(tag.ToWideString()) -- ToWideString 0x0099E480 (labelled GetDefaultVal<CAnimationSet>, a
// folded body), CWideString::operator= 0x0099B7D0 on TextTag +0x18, temp dtor 0x0099B510 (labelled ~CCharString, folded).
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. CTCActionUseReadable: Ego_r.pdb TextTag (CWideString) +0x18,
// inline ?SetTextTag@CTCActionUseReadable@@QAEXABVCWideString@@@Z. Interface 0xd = CTCActionUseReadable is
// inferred from that +0x18 store, not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCActionUseReadable {                             // interface 0xd
public:
    unsigned char _pad00[0x18];
    CWideString TextTag;                                 // +0x18
    void SetTextTag(const CWideString& tag) { TextTag = tag; }
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetReadableObjectTextTag(const CScriptThing& thing, const CCharString& tag) const
{
    CThing* t = thing.GetPThing();
    if (t && !t->IsDead())
    {
        CTCActionUseReadable* readable;
        if (t->GetTC(TCI_ON_ACTION_USE, &readable))
            readable->SetTextTag(tag.ToWideString());
    }
}