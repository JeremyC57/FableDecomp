// NScript::CQ_AmbushTradersScript::WatchForRegionChange() -- script thread: wait for each region load; on Gibbet Woods, the Deserted Farm or Bowerstone Jail refresh the spies' allegiances
// Leaf: ?WatchForRegionChange@CQ_AmbushTradersScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F058D0, 308 bytes. Identity: the catalog label CV_ChapelOfEvilScript::WatchForLeaving
//   is wrong -- the thread registration at 0x00F02CEE stores this address under the name "WatchForRegionChange" (0x012F7C48)
//   and the body calls ?RefreshSpyAllegiences@CQ_AmbushTradersScript@NScript@@ (0x00F05A10).
// Callees: CCharString() 0x0099E4B0, ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, Compare
//   0x00411570 (same-TU stand-in body), RefreshSpyAllegiences 0x00F05A10, game script interface slots 7 / 22.
// Layout: local models (no engine/ header for the quest script classes); Game +0x40.
// Shape: the natural `region == "GibbetWoods" || ... ` with the inline CCharString ==: its null-data arm is
//   `if (strcmp(text, "") == 0) return true; return false;` (VC7.1 branches off the repe cmpsb flags, and the non-null
//   arms' block compares are homed in a frame byte as retail).
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (the landed body, stand-in)
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
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) == 0; }   // 0x004115A0 (inlined here)
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CQ_AmbushTradersScript : CQuestScriptBase {
    void RefreshSpyAllegiences();                           // 0x00F05A10
    void WatchForRegionChange();
};
void CQ_AmbushTradersScript::WatchForRegionChange()
{
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (region == "GibbetWoods" || region == "DesertedFarm" || region == "BowerstoneJail") {
            if (IsActiveThreadTerminating())
                return;
            RefreshSpyAllegiences();
        }
        Game->NewScriptFrame();
    }
}