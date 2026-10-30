// CV_HeroDollsScript::HeroHasAnyDolls() -- true when the hero carries at least one of the seven hero dolls
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E98200, 0x22a bytes, plain `ret`, bool in al.
// Callees: CGameScriptInterface slot 70 GetHero() (+0x118), slot 184 IsObjectInThingsPossession(const CCharString&,
//   const CScriptThing&) const (+0x2e0); CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0.
// Strings: the seven OBJECT_DOLL_* names 0x012ED08C .. 0x012ECFF8 (the same pool as RemoveHeroDolls 0x00E97EA0).
// Shape: one short-circuit `||` chain of seven possession tests. Each literal is an implicit conversion temporary
//   that is only built when the chain reaches it, so VC7.1 keeps a conditional-destruction bit mask in ebx
//   (1, 3, 7, .. 0x7f) and destroys the reached temporaries in reverse order at the shared end; the result
//   byte lives in the frame across those destructor calls.
// Layout: this+0x40 is the quest script's game interface (CQuestScriptBase, landed script rows).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
};
namespace NScript {
class CV_HeroDollsScript : public CQuestScriptBase {
public:
    bool HeroHasAnyDolls();
};
} // namespace NScript
using NScript::CV_HeroDollsScript;
bool NScript::CV_HeroDollsScript::HeroHasAnyDolls()
{
    return Game->IsObjectInThingsPossession("OBJECT_DOLL_BRIAR_ROSE_01", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_DOLL_MAZE_01", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_DOLL_SCARLET_ROBE_01", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_DOLL_THUNDER_01", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_DOLL_WHISPER_01", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_DOLL_TWINBLADE_01", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_DOLL_SCYTHE_01", Game->GetHero());
}