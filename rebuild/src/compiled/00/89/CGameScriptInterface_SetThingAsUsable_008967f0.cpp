// CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const -- set the usable flag on the
// thing's usable component (TC 0x0D), else on its scripted-use hook (TC 0xC2), adding the hook when it has neither
// Leaf: ?SetThingAsUsable@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap_Retail@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008967F0, length 208 bytes, `ret 8`.
// Callees: the script thing's virtual slot 75 IsValid (+0x12C) and slot 11 GetThing (+0x2C), the TC map lower bound
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0.
// String 0x01239FE4 = "CTCActionUseScriptedHook".
// Layout: CThing interface bitset +0x20 (TC 0x0D = +0x20 bit 13, TC 0xC2 = +0x38 bit 2), TC map +0x44 {first,last};
// TC 0x0D's usable byte +0x10, CTCActionUseScriptedHook::Usable +0x0C.
// Shape: the out-parameter GetInterface template (landed 0x00896000 model) homes both lookup keys in the dead `thing`
// argument slot; the usable-TC arm returns, the hook arm is `if (!GetInterface(&hook)) { named CCharString; AddTC }`
// joined by one `hook->Usable = usable` (tail-duplicated, hook in esi on both paths). The same-TU LowerBound body
// (0x0040F020 stand-in, binary search over {type, component} pairs) tells VC7.1 the key's address does not escape,
// so the 0xC2 key and the hook name share the dead slot instead of opening a push-ecx frame word.

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

class CTCBase;
class CTCParamsBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;

struct CTCUsable_0D {                     // the TC 0x0D component
    unsigned char _pad[0x10];
    bool Usable;                          // +0x10
};

class CTCActionUseScriptedHook { public:
    unsigned char _pad[0xc];
    bool Usable;                          // +0x0C
};

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const
{
    if (thing.IsAlive()) {
        CThing* t = thing.GetPThing();
        CTCUsable_0D* usableTC;
        CTCActionUseScriptedHook* hook;
        if (t->GetTC(TCI_ON_ACTION_USE, &usableTC)) {
            usableTC->Usable = usable;
            return;
        }
        if (!t->GetTC(TCI_ACTION_USE_SCRIPTED_HOOK, &hook)) {
            CCharString name("CTCActionUseScriptedHook", -1);
            hook = (CTCActionUseScriptedHook*)t->AddTC(name, false, 0);
        }
        hook->Usable = usable;
    }
}