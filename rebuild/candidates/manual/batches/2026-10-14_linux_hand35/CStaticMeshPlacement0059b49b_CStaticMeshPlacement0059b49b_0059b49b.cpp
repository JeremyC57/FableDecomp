#pragma optimize("y", off)
#pragma optimize("s", on)
// CStaticMeshPlacement_0059b49b::CStaticMeshPlacement_0059b49b(S16 a, S16 b, S16 c) -- a constructor that forwards its three 16-byte by-value arguments (copied onto the stack with `movsd` runs) and the address of the third one's last byte to the `__fastcall` initialiser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B49B, length 61 bytes.
// Callees: the initialiser @ 0x0059B2CC (`__fastcall`: `this` in ecx, the byte address in edx, then the three structs; the manifest label
// "CNavigatorAStar::MoveToNavLineOfSightProximity" is propagated). Shape: a frame pointer (the 0x0059xxxx module), the three argument copies built with
// `sub esp,10h ; mov edi,esp ; lea esi,[ebp+..] ; movsd x4` from the last argument down, `lea edx,[ebp+37h]` and `mov ebx,ecx` before the last movsd,
// `this` returned, `ret 30h`. The chunk label "CEnginePrimitiveRenderer::AddStaticMesh" is propagated; the class is named by address.
struct S16_0059b49b { float m_x, m_y, m_z; unsigned char m_flags[4]; };   // 16 bytes by value

struct CStaticMeshPlacement_0059b49b;
void __fastcall InitPlacement_0059b2cc(CStaticMeshPlacement_0059b49b* self, unsigned char* flag, S16_0059b49b a, S16_0059b49b b, S16_0059b49b c);   // 0x0059B2CC

struct CStaticMeshPlacement_0059b49b {
    CStaticMeshPlacement_0059b49b(S16_0059b49b a, S16_0059b49b b, S16_0059b49b c);
};

CStaticMeshPlacement_0059b49b::CStaticMeshPlacement_0059b49b(S16_0059b49b a, S16_0059b49b b, S16_0059b49b c)
{
    InitPlacement_0059b2cc(this, &c.m_flags[3], a, b, c);
}
