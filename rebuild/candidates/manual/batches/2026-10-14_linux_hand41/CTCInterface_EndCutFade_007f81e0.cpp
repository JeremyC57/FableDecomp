// CTCInterface::EndCutFade() -- clears the cut-fade flag (+0xc, inline helper @ 0x007F7700) and posts the end-of-fade message 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F81E0, length 19 bytes.
// Callees: flag clear @ 0x007F7700 (`mov byte ptr [ecx+0Ch],0`), message post @ 0x007F8160 (manifest "TCInterface_HandleEndCutFade",
// one immediate argument 0).
// Class attribution: manifest label "CGameScriptInterface::EndCutFade" is propagated; the callee's label names a TC interface,
// so the class is provisional. Shape: two thiscalls on `this` kept in esi.
struct CTCInterface {
    void ClearCutFade();                            // 0x007F7700
    void HandleEndCutFade(long message);            // 0x007F8160
    void EndCutFade();
};

void CTCInterface::EndCutFade()
{
    ClearCutFade();
    HandleEndCutFade(0);
}
