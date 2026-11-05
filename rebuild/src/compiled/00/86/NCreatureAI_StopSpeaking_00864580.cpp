// NCreatureAI::StopSpeaking(CThingCreatureBase&) -- clears the creature's speech emote: SetEmoteText(priority 7, L"", 0, 0) on its emote icon component
// Leaf: ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail 0x00864580, 0x5C bytes (+ int3 padding), __fastcall (creature in ecx). PDB (Ego_r):
// ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z. Callers 0x00919A06 (tail jmp), 0x0094BB13, 0x0098013D, ...
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the non-const twin of 0x004365B0: the
// creature is a non-const reference), CWideString(const wchar_t*) @ 0x0099B6B0 on L"" @ 0x0122D70C,
// CTCEmoteIcon::SetEmoteText(EEmotePriority, const CWideString&, float, float) @ 0x0071AC70, ~CWideString @ 0x0099B510.
// Interface type 0x45 = CTCEmoteIcon (bitset word 2 bit 5: `test byte ptr [ecx+28h],20h`).
// Shape: the __forceinline GetInterface(type, &out) template puts the component in esi; the empty string is a NAMED local
// built before the argument pushes (a bare temporary argument is constructed between the pushes and passed as the
// constructor's returned eax) and it reuses the key's `push ecx` slot; the two float zeros are pushed as immediates.
// CThing is modelled locally: rebuild/include/engine has no CThing.h and CThingCreatureBase.h pads the CThing bytes.
typedef unsigned short wchar_t;                      // VC7.1 without /Zc:wchar_t

class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"

enum EEmotePriority { EMOTE_PRIORITY_7 = 7 };

class CTCEmoteIcon                                   // interface type 0x45
{
public:
    void SetEmoteText(EEmotePriority priority, const CWideString& text, float scale, float time);   // 0x0071AC70
};

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { };

namespace NCreatureAI
{
    void __fastcall StopSpeaking(CThingCreatureBase& creature);
}

void __fastcall NCreatureAI::StopSpeaking(CThingCreatureBase& creature)
{
    CTCEmoteIcon* emote;
    if (creature.GetTC(TCI_EMOTEICON, &emote))
    {
        CWideString text(L"");
        emote->SetEmoteText(EMOTE_PRIORITY_7, text, 0.0f, 0.0f);
    }
}