// CTCOracleMinigame::SetText(unsigned long text) -- when the text changes and the hero is alive, show the narrated text through the hero's region-display component and remember it
// Leaf: ?SetText@CTCOracleMinigame@@QAEXK@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E6230, length 153 bytes (`ret 4`).
// Callees: CTCBase::GetPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960, CPlayer::GetPControlledCreature @ 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCRegionDisplay, type 0x8E = bit 14 of
// CThing+0x30), CTCOracleMinigame::GetTextWithNarrator(unsigned long) @ 0x007E5FF0 (CWideString by value),
// CTCRegionDisplay::SetCurrentAreaName(const CWideString&, float, bool, bool, bool) @ 0x00639A90 (-1.0, false, false, true),
// CWideString::~CWideString @ 0x0099B510.
// Layout: CurrentText +0x0C as in rebuild/include/engine/CTCOracleMinigame.h (data-only header, modelled locally); retail CThing bitset
// +0x20, map +0x44, dead bit +0x91 bit 0.
// Shape: `T* p = 0;` then the out-pointer GetInterface without a test (retail presets esi to 0); the narrated text is an unnamed
// by-value temporary destroyed after the call; CurrentText stored inside the hero block.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };

class CTCRegionDisplay { public:
    void SetCurrentAreaName(const CWideString& name, float duration, bool a, bool b, bool c);   // 0x00639A90
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                         // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCBase
{
public:
    virtual void v00();
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};

class CTCOracleMinigame : public CTCBase { public:
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _base08[4];
    unsigned long CurrentText;                      // +0x0c
    CWideString GetTextWithNarrator(unsigned long text);   // 0x007E5FF0
    void SetText(unsigned long text);
};

void CTCOracleMinigame::SetText(unsigned long text)
{
    if (CurrentText != text) {
        CThing* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (hero && !hero->IsDead()) {
            CTCRegionDisplay* display = 0;
            hero->GetTC(TCI_REGION_DISPLAY, &display);
            display->SetCurrentAreaName(GetTextWithNarrator(text), -1.0f, false, false, true);
            CurrentText = text;
        }
    }
}