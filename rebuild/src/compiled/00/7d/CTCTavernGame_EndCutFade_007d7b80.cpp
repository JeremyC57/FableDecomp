// CTCTavernGame::EndCutFade() -- resets the playback info and moves the tavern game to state 0x27
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D7B80, length 32 bytes.
// Callees: reset helper @ 0x005E9A90 (manifest "CXMVPlaybackInfo::CXMVPlaybackInfo", a propagated label), CTCTavernGame::
// SetState(ETavernGameStates) @ 0x005E9B20 (one immediate argument 0x27).
// Class attribution: the manifest labels this (and two unrelated two-call bodies) "CGameScriptInterface::EndCutFade"; the
// SetState callee is labelled CTCTavernGame, which is the only class evidence -- provisional. The name is the manifest's.
// Shape: two thiscalls on `this` kept in esi.
class CTCTavernGame { public:
    void ResetPlayback();                           // 0x005E9A90
    void SetState(long state);                      // 0x005E9B20
    void EndCutFade();
};

void CTCTavernGame::EndCutFade()
{
    ResetPlayback();
    SetState(0x27);
}