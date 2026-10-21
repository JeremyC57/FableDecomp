// Behaviour test for 00c40450: src/00c40450.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTrackedTransform_00c40450::SetPosition(const CTrackedVector& position)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTrackedTransform_00c40450::SetPosition(const CTrackedVector& position) -- stores the position at +0x0c and raises dirty bit 0x8000 (+0x48) only when one of its three floats differs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40450, length 75 bytes.
// Leaf: SetPosition
// The manifest label "CShaderRenderManager::SetCameraPos" is propagated (the PDB CShaderRenderManager holds a
// CArray at +0x0c); the class and method names follow the landed caller 0x00C338C0 (script module, no RTTI).
// Shape: three `fld arg ; fld member ; fucompp ; test ah,44h` compares chained with `||` (`jp` to the update for
// the first two, `jnp` past it for the last: `member != argument`), the 12-byte POD copied through eax/eax/edx,
// `or dword ptr [ecx+48h],8000h`, `ret 4`.
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }   // user operator=: the member stores may alias, so the flag RMW stays one `or [mem],imm`
};

struct CTrackedVector {                         // the setter's argument: a position wrapper built from a C3DVector
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};

struct CTrackedTransform_00c40450 {
    unsigned char _pad00[0x0c];
    C3DVector     m_position;                   // +0x0c
    C3DVector     m_direction;                  // +0x18 (0x00C404A0)
    unsigned char _pad24[0x24];
    unsigned long m_dirtyFlags;                 // +0x48

    void SetPosition(const CTrackedVector& position);
};

void CTrackedTransform_00c40450::SetPosition(const CTrackedVector& position)
{
    if (m_position.X != position.Value.X || m_position.Y != position.Value.Y || m_position.Z != position.Value.Z) {
        m_position = position.Value;
        m_dirtyFlags |= 0x8000;
    }
}

// ---- stubs and driver ----

int main()
{
    static CTrackedTransform_00c40450 t; t.m_position.X = 1.0f; t.m_position.Y = 2.0f; t.m_position.Z = 3.0f; t.m_dirtyFlags = 0x1;
    C3DVector same = { 1.0f, 2.0f, 3.0f }; CTrackedVector s(same);
    t.SetPosition(s); CHECK(t.m_dirtyFlags == 0x1);                                         // unchanged: no dirty bit
    C3DVector moved = { 1.0f, 2.5f, 3.0f }; CTrackedVector m(moved);
    t.SetPosition(m); CHECK(t.m_position.Y == 2.5f); CHECK(t.m_position.X == 1.0f && t.m_position.Z == 3.0f); CHECK(t.m_dirtyFlags == (0x1 | 0x8000));
    C3DVector z = { 1.0f, 2.5f, -3.0f }; CTrackedVector zz(z); t.m_dirtyFlags = 0; t.SetPosition(zz); CHECK(t.m_position.Z == -3.0f); CHECK(t.m_dirtyFlags == 0x8000);
    C3DVector x = { 9.0f, 2.5f, -3.0f }; CTrackedVector xx(x); t.m_dirtyFlags = 0; t.SetPosition(xx); CHECK(t.m_position.X == 9.0f); CHECK(t.m_dirtyFlags == 0x8000);
    if (fails == 0) printf("00c40450_TEST PASS\n"); else printf("00c40450_TEST FAIL\n");
    return fails;
}