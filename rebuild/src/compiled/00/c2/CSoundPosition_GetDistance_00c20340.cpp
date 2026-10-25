// CSoundPosition::GetDistance(const C3DVector& listener) const -- the distance of a sound's position from the listener: the vector's own length when the position is listener-relative (flag +0xc), else the length of (position - listener); returned on the FPU stack
// Leaf: ?GetDistance@CSoundPosition@@QBEMABUC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20340, 38 bytes, __thiscall `ret 4`, float result in st0:
//   mov al,[ecx+0Ch] ; sub esp,0Ch ; test al,al ; jne len ; mov eax,[esp+10h] ; mov edx,ecx ; push eax ;
//   lea ecx,[esp+4] ; call operator- ; lea ecx,[esp] ; len: call C3DVector::GetMagnitude ; add esp,0Ch ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CDragSource_00c20340::BeginDrag returning void (FINDINGS chunk 437: "declared
//   void but returns a float in st0"; the catalog "CDraw2DText::ClearPrimitivesIfNecessary" and the callee labels
//   "CGuiWindow::OnBeginDrag" / "Math_Vector3Length" are propagated). The callees are the landed C3DVector::GetMagnitude
//   0x00C25970 (sqrt of the squared components, result in st0) and C3DVector operator- 0x00C257B0 (__fastcall: hidden
//   12-byte result in ecx, the left operand in edx, the right one on the stack). The one caller 0x00C3448E (libpk voice
//   update) passes the listener record's +8 vector and feeds the st0 result to the two falloff curves at +0x14 / +0x1c
//   of the voice (0x00C1BDA0). The 16-byte object (3 floats, flag byte +0xc) is the one 0x00C20390 zeroes and 0x00C20370
//   compares (flag, then the vectors). CSoundPosition is descriptive (no PDB covers libpk).
// Shape: C3DVector has a user constructor (as the landed operator- row models it), so the __fastcall operator- takes the
//   hidden result pointer in ecx and the operands in edx / on the stack; the two GetMagnitude calls are tail-merged: the relative path reaches the call with ecx still = `this`, the
//   other with `lea ecx,[esp]` (the difference temporary in the 12-byte frame).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}   // as in the landed operator- 0x00C257B0
    float GetMagnitude() const;                 // 0x00C25970
};

C3DVector __fastcall operator-(const C3DVector& a, const C3DVector& b);   // 0x00C257B0

struct CSoundPosition {                         // 0x10 bytes
    C3DVector m_position;                       // +0x00
    bool m_bListenerRelative;                   // +0x0c
    float GetDistance(const C3DVector& listener) const;
};

float CSoundPosition::GetDistance(const C3DVector& listener) const
{
    if (m_bListenerRelative)
        return m_position.GetMagnitude();
    C3DVector offset = m_position - listener;
    return offset.GetMagnitude();
}
