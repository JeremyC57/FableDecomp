// NScript::CV_RandomPopulationSimScript::SetCurrentPopulationRegionFromRegionName(CCharString& region) -- map a loaded region name to the population-sim region index (Greatwood 0, Darkwood 1, Witchwood 2, LookoutPoint 3)
// Leaf: ?SetCurrentPopulationRegionFromRegionName@CV_RandomPopulationSimScript@NScript@@QAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC2520, 778 bytes, thiscall, `ret 4`, no EH frame.
// Identity (proven): the landed NScript::CV_RandomPopulationSimScript::ProcessAlbionPopulationSim 0x00EC2240 calls it and binds
//   this exact spelling; Ego_r.pdb names ?SetCurrentPopulationRegionFromRegionName@CV_RandomPopulationSimScript@NScript@@QAEXAAVCCharString@@@Z.
//   Catalog label CScriptBase_ProcessAlbionPopulationSim_Filter is wrong.
// Callees: CBasicString<char>::Compare 0x00411570 (__fastcall, called out of line from the inlined CCharString::operator==),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 (the trailing else arm), strcmp inlined (the null-PStringData arm vs "").
// Data: "GreatwoodEntrance" 0x012C5650, "GreatwoodLake" 0x012C5670, "Greatwood" 0x012CA87C, "GreatwoodBanditToll" 0x012E4CA8,
//   "Darkwood1" 0x012DF218, "Darkwood2" 0x012DF20C, "Darkwood3" 0x012D6CA0, "Darkwood5" 0x012CBB58, "Darkwood6" 0x012DF1C4,
//   "Witchwood2" 0x012DCEF8, "Witchwood4" 0x012DCF10, "LookoutPoint" 0x012C3394, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14): CurrentPopulationRegion +0x5c (CLevelScriptBase Game +0x40).
// Shape: if / else-if chain of `||` comparisons (the landed 0x00F088E0 inline CCharString::operator== model, with Compare
//   defined in the TU under auto_inline(off) so it stays a call); the last `else` is a bare terminating test.
#include <string.h>
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
#pragma auto_inline(off)
template <> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_RandomPopulationSimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float CurrentPopulationScale[4];                    // +0x48
    bool  HighDetailSimulationActive;                   // +0x58
    long  CurrentPopulationRegion;                      // +0x5c
    void SetCurrentPopulationRegionFromRegionName(CCharString& region);
};

void CV_RandomPopulationSimScript::SetCurrentPopulationRegionFromRegionName(CCharString& region)
{
    if (region == "GreatwoodEntrance" || region == "GreatwoodLake" || region == "Greatwood" || region == "GreatwoodBanditToll") {
        if (IsActiveThreadTerminating())
            return;
        CurrentPopulationRegion = 0;
    } else if (region == "Darkwood1" || region == "Darkwood2" || region == "Darkwood3" || region == "Darkwood5"
               || region == "Darkwood6") {
        if (IsActiveThreadTerminating())
            return;
        CurrentPopulationRegion = 1;
    } else if (region == "Witchwood2" || region == "Witchwood4") {
        if (IsActiveThreadTerminating())
            return;
        CurrentPopulationRegion = 2;
    } else if (region == "LookoutPoint") {
        if (IsActiveThreadTerminating())
            return;
        CurrentPopulationRegion = 3;
    } else {
        if (IsActiveThreadTerminating())
            return;
    }
}
}   // namespace NScript