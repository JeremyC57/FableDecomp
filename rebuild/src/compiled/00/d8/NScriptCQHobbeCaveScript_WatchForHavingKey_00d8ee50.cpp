// CQ_HobbeCaveScript::WatchForHavingKey() -- each time the Hobbe cave main chamber loads, make the hexagon door usable only if the hero carries its key, then wait for the chamber to unload
// Leaf: WatchForHavingKey
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8EE50, 490 bytes (496 with padding), plain `ret`.
// Label: the manifest's "CV_TrophyDealerScript::HilightGuildTeleporter" is propagated from the byte-similar 0x00EE71A0.
//   This body is the CSpawnedFunc target stored by CQ_HobbeCaveScript code at 0x00D8E59B (the only reference), sits
//   right after CQ_HobbeCaveScript::WatchForTermination (0x00D8E990), and tests the hexagon door key -- Ego_r.pdb's
//   CQ_HobbeCaveScript has exactly one such watcher, WatchForHavingKey.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7),
//   IsRegionLoaded (slot 12), GetHero (slot 70), GetThingWithScriptName (slot 72, hidden result),
//   IsObjectInThingsPossession (slot 184), SetThingAsUsable (slot 447).
// Strings: "HobbeCaveMainChamber" 0x012D55EC, "HexagonDoor" 0x012D5924, "OBJECT_HEXAGON_DOOR_KEY" 0x012D56D0.
// Shape: an outer `while (!IsActiveThreadTerminating())` loop around two WAIT_UNTIL waits (`while (!x)` gives the
//   neg/sbb/inc bool, the second wait is written `while (!(!x))` for the setne one); every string is an implicit
//   conversion; the key test keeps the macro terminating test inside each arm (VC hoists the shared call).
// Layout: interface pointer at +0x40 (CQuestScriptBase). Local model: CGameScriptInterface.h carries no virtual slots.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing { public:                                       // vtable 0x01238C8C, 12 bytes
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                        // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_HobbeCaveScript {
public:
    unsigned char _base[0x40];                              // CScriptBase
    CGameScriptInterface* Game;                             // +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940 (CScriptBase)
    void WatchForHavingKey();
};
} // namespace NScript
using NScript::CQ_HobbeCaveScript;
void NScript::CQ_HobbeCaveScript::WatchForHavingKey()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("HobbeCaveMainChamber")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing door = Game->GetThingWithScriptName("HexagonDoor");
            if (Game->IsObjectInThingsPossession("OBJECT_HEXAGON_DOOR_KEY", Game->GetHero())) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetThingAsUsable(door, true);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetThingAsUsable(door, false);
            }
            while (!(!Game->IsRegionLoaded("HobbeCaveMainChamber"))) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}