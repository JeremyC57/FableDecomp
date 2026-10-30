// CQ_HeroSoulsArenaScript::WatchForAudioInArena() -- script thread: while either arena region (ArenaCells or ArenaHallOfHeroes) is loaded, override the music with the arena set (0x22); when neither is loaded any more, stop the override
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D7D730, length 397 bytes (+ int3 padding), plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0; through the quest's interface (+0x40): NewScriptFrame (slot 7), IsRegionLoaded (12), OverrideMusic (696),
// StopOverrideMusic (697).
// Strings: "ArenaCells" 0x012C7814, "ArenaHallOfHeroes" 0x012C7820.
// Shape: the music flag is a bool local; each `||` region test builds its strings as conditional temporaries (one
// flag bit each in ebx: 1/2 for the start test, 4/8 for the stop test) whose results land in a frame bool; the arms
// are an if / else-if inside `while (!IsActiveThreadTerminating())` with the frame call at the bottom.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CQ_HeroSoulsArenaScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void WatchForAudioInArena();
};
void CQ_HeroSoulsArenaScript::WatchForAudioInArena()
{
    bool musicOverridden = false;
    while (!IsActiveThreadTerminating()) {
        if (!musicOverridden && (GSI->IsRegionLoaded("ArenaCells") || GSI->IsRegionLoaded("ArenaHallOfHeroes"))) {
            if (IsActiveThreadTerminating())
                return;
            musicOverridden = true;
            GSI->OverrideMusic((EMusicSetType)0x22, false, false);
        } else if (musicOverridden && !(GSI->IsRegionLoaded("ArenaCells") || GSI->IsRegionLoaded("ArenaHallOfHeroes"))) {
            if (IsActiveThreadTerminating())
                return;
            musicOverridden = false;
            GSI->StopOverrideMusic(false);
        }
        GSI->NewScriptFrame();
    }
}
}