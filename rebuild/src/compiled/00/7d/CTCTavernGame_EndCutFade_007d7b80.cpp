// CTCTavernGame::EndCutFade() -- resets the playback info and moves the tavern game to state 0x27
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D7B80, length 32 bytes.
// Callees: reset helper @ 0x005E9A90 (manifest "CXMVPlaybackInfo::CXMVPlaybackInfo", a propagated label), CTCTavernGame::
// SetState(ETavernGameStates) @ 0x005E9B20 (one immediate argument 0x27).
// Class attribution: the manifest labels this (and two unrelated two-call bodies) "CGameScriptInterface::EndCutFade"; the
// SetState callee is labelled CTCTavernGame, which is the only class evidence -- provisional. The name is the manifest's.
// Shape: two thiscalls on `this` kept in esi.
class CTCTavernGame { public:
    virtual void NewRound();                        // 0x005E9A90 ?NewRound@CTCTavernGame@@UAEXXZ (vtable slot 32; Ego_r/FableWin)
    // NF159 (q4271): Ego_r.pdb / FableWin.pdb ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z: the state is the nested enum
    //   CTCTavernGame::ETavernGameStates (enumerator names unproven: values passed as numbers); was SetState(long).
    enum ETavernGameStates { };
    void SetState(ETavernGameStates state);                      // 0x005E9B20
    void EndCutFade();
};

void CTCTavernGame::EndCutFade()
{
    CTCTavernGame::NewRound();
    SetState((ETavernGameStates)0x27);
}