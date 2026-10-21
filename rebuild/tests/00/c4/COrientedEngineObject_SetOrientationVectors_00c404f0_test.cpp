// Behaviour test for 00c404f0: src/00c404f0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. COrientedEngineObject::SetOrientationVectors(const C3DVector& up, const C3DVector& forward)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// COrientedEngineObject::SetOrientationVectors(const C3DVector& up, const C3DVector& forward) -- store a new up/forward pair, flagging the orientation dirty only for a vector that actually changed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C404F0, length 147 bytes (+13 int3 padding).
// The catalog files this under CRightHandedSet, but `this` is a larger engine object that EMBEDS the right-handed
// set: Up at +0x24, Forward at +0x30 and a dirty-flag word at +0x48 (bit 0x20000 = orientation changed). Callers
// (0x00C3395F, 0x00C4073E) copy two C3DVectors to their frame and pass both by reference. Owning class unknown
// (a descriptive model name is used).
// Shape: an inline C3DVector `operator!=` (three `fucompp ; test ah,44h ; jp` tests, any difference takes the
// store arm) and an inline whole-vector copy; the flag is a one-bit field (setting it gives the dword `or [ecx+48h],20000h`; an explicit `|=` on a
// plain word loads, ors and stores through eax). C3DVector needs its user inline operator= (with the implicit one the
// flag RMW was split, 155v147).
struct C3DVector {
    float x, y, z;
    bool operator!=(const C3DVector& v) const { return v.x != x || v.y != y || v.z != z; }
    C3DVector& operator=(const C3DVector& o) { x = o.x; y = o.y; z = o.z; return *this; }
};

struct COrientedEngineObject {
    unsigned char _head[0x24];
    C3DVector     Up;            // +0x24
    C3DVector     Forward;       // +0x30
    unsigned char _gap[0x48 - 0x3C];
    unsigned long OtherFlags : 17;               // +0x48 bits 0..16
    unsigned long OrientationChanged : 1;        // +0x48 bit 17 (0x20000)

    void SetOrientationVectors(const C3DVector& up, const C3DVector& forward);
};

void COrientedEngineObject::SetOrientationVectors(const C3DVector& up, const C3DVector& forward)
{
    if (up != Up)
    {
        Up = up;
        OrientationChanged = 1;
    }
    if (forward != Forward)
    {
        Forward = forward;
        OrientationChanged = 1;
    }
}

// ---- stubs and driver ----

int main()
{
    static COrientedEngineObject o; o.Up.x = 0; o.Up.y = 0; o.Up.z = 1; o.Forward.x = 1; o.Forward.y = 0; o.Forward.z = 0; o.OtherFlags = 5; o.OrientationChanged = 0;
    C3DVector up = { 0, 0, 1 }, fwd = { 1, 0, 0 };
    o.SetOrientationVectors(up, fwd); CHECK(o.OrientationChanged == 0); CHECK(o.OtherFlags == 5);
    C3DVector up2 = { 0, 1, 0 };
    o.SetOrientationVectors(up2, fwd); CHECK(o.OrientationChanged == 1); CHECK(o.Up.y == 1 && o.Up.z == 0); CHECK(o.OtherFlags == 5);
    o.OrientationChanged = 0; C3DVector fwd2 = { 0, 0, -1 };
    o.SetOrientationVectors(up2, fwd2); CHECK(o.OrientationChanged == 1); CHECK(o.Forward.z == -1 && o.Forward.x == 0); CHECK(o.Up.y == 1);
    if (fails == 0) printf("00c404f0_TEST PASS\n"); else printf("00c404f0_TEST FAIL\n");
    return fails;
}