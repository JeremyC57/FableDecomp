// CVsyncCallbackTimer::OnPreDeviceReset() -- before the D3D device is reset, release every outstanding vsync query (ClearAllQueries)
// Leaf: ?OnPreDeviceReset@CVsyncCallbackTimer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F73A0, length 19 bytes, plain `ret`.
// PDB: ?OnPreDeviceReset@CVsyncCallbackTimer@@QAEXXZ. Caller: CDisplayManager @ 0x009BF516 (`mov ecx,[edi+294h] ; call`, the
//   scoped_ptr<CVsyncCallbackTimer> member), right after the device-lost handling.
// Callees: CVsyncCallbackTimer::ClearAllQueries @ 0x009F7250 (private).
// Label correction: the chunk label "operator_new" is propagated garbage.
// Shape: the vsync timer TU is unoptimised (`#pragma optimize("", off)`): `this` homed at [ebp-4] and reloaded for the call.
// Layout: retail CVsyncCallbackTimer (Ego_r.pdb names, 4-byte STLport lists): Device +0, FreeQueoryObjects +4, PendingQueories +8,
//   PendingFrame +0xc, FinishedFrame +0x10.
#pragma optimize("", off)
class CVsyncCallbackTimer {
public:
    void* Device;                                    // +0x00 IDirect3DDevice9*
    void* FreeQueoryObjects;                         // +0x04 STLport list<CPendingQuery> (one node pointer)
    void* PendingQueories;                           // +0x08 STLport list<CPendingQuery> (one node pointer)
    long  PendingFrame;                              // +0x0c
    long  FinishedFrame;                             // +0x10
    void OnPreDeviceReset();
private:
    void ClearAllQueries();                          // 0x009F7250
};

void CVsyncCallbackTimer::OnPreDeviceReset()
{
    ClearAllQueries();
}