// NScript::CExpression_FishScript::Init() -- start the fishing expression script at fishing level 1 (LastFishingLevel = 1)
// Leaf: ?Init@CExpression_FishScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE94C0, 8 bytes, thiscall: mov dword ptr [ecx+44h],1 ; ret.
// Identity: slot 3 (Init, engine/CScriptBase.h) of the script vtable 0x012F54E4 that the "Expression_Fish" Alloc stores
//   (slot 1 RegisterMain 0x00EE94D0 and the ctor 0x00EE94A0 are landed); Ego_r.pdb / FableWin.pdb
//   ?Init@CExpression_FishScript@NScript@@UAEXXZ (public virtual).
// Layout: Ego_r.pdb CExpression_FishScript (92 bytes) has its one own member `long LastFishingLevel` at +0x58; retail's
//   CScriptBase is 0x14 smaller (0x40), so it sits at +0x44 after Game +0x40 (the landed ctor's model).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_FishScript : public CScriptBase {
public:
    CExpression_FishScript(CGameScriptInterfaceBase& game);
    virtual ~CExpression_FishScript();      // ??1CExpression_FishScript@NScript@@UAE@XZ
    virtual void Init();                    // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40
    long LastFishingLevel;                  // +0x44 (Ego_r +0x58)
};

void CExpression_FishScript::Init()
{
    LastFishingLevel = 1;
}
}