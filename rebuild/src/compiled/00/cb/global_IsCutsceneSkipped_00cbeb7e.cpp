// IsCutsceneSkipped() -- true when the player skipped the cutscene: the action-mode button when that skip mode is set, else the interface's cut-scene-skipped message
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?IsCutsceneSkipped@@YI_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEB7E, 41 bytes, `__fastcall` global with no arguments, plain `ret`
//   (PDB ?IsCutsceneSkipped@@YI_NXZ).
// Data: the skip-with-action-button flag byte @ 0x0143E8F4 (descriptive name), the game script interface @ 0x0143E8F8.
// Callees: CGameScriptInterface (vtable 0x01260F0C) slot 42 MsgIsActionModeButtonPressed (+0xa8, 0x00894440) and slot 44
//   MsgIsCutSceneSkipped (+0xb0, 0x00893BA0); the flag picks one.
// Shape: an if/else on the flag with one `if (call()) return true;` per arm and a shared `return false`: the interface
//   and its vtable are loaded once above the flag branch (common to both arms), the two calls merge into one byte
//   test and the early returns give `mov al,1` / `xor al,al` (a ternary of the two calls gives `setne al`, 37 v 41).
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual bool MsgIsActionModeButtonPressed() const;   // slot 42 (+0xa8)
    virtual void v043();
    virtual bool MsgIsCutSceneSkipped() const;           // slot 44 (+0xb0)
};
extern bool g_bSkipCutsceneWithActionButton;                  // 0x0143E8F4
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

bool __fastcall IsCutsceneSkipped()
{
    if (g_bSkipCutsceneWithActionButton) {
        if (g_pGameScriptInterface->MsgIsActionModeButtonPressed())
            return true;
    } else {
        if (g_pGameScriptInterface->MsgIsCutSceneSkipped())
            return true;
    }
    return false;
}