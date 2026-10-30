// CV_HeroDollsScript::RemoveHeroDolls() -- take each of the seven hero dolls (Briar Rose, Maze, Scarlet Robe, Thunder, Whisper, Twinblade, Scythe) out of the hero's inventory
// Leaf: ?RemoveHeroDolls@CV_HeroDollsScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E97EA0, 294 bytes (304 with padding), plain `ret`.
// Callees: CGameScriptInterface slot 125 TakeObjectFromHero(const CCharString&) const (+0x1f4); CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0.
// Strings: "OBJECT_DOLL_BRIAR_ROSE_01" 0x012ED08C, "OBJECT_DOLL_MAZE_01" 0x012ED078, "OBJECT_DOLL_SCARLET_ROBE_01" 0x012ED05C,
//   "OBJECT_DOLL_THUNDER_01" 0x012ED044, "OBJECT_DOLL_WHISPER_01" 0x012ED02C, "OBJECT_DOLL_TWINBLADE_01" 0x012ED010,
//   "OBJECT_DOLL_SCYTHE_01" 0x012ECFF8.
// Shape: seven statements, each literal an IMPLICIT conversion temporary (built before the interface is read, then
//   re-addressed with `lea`), all sharing the one `push ecx` frame word.
// Layout: this+0x40 is the quest script's game interface (CQuestScriptBase, landed script rows).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
};
namespace NScript {
class CV_HeroDollsScript : public CQuestScriptBase {
public:
    void RemoveHeroDolls();
};
} // namespace NScript
using NScript::CV_HeroDollsScript;
void NScript::CV_HeroDollsScript::RemoveHeroDolls()
{
    Game->TakeObjectFromHero("OBJECT_DOLL_BRIAR_ROSE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_MAZE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_SCARLET_ROBE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_THUNDER_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_WHISPER_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_TWINBLADE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_SCYTHE_01");
}