// NCameraModes::CScriptModuleUseLamp::OnInitLeaf() -- when both the lamp and the creature are alive, set up the orbit: origin at the lamp's target-cursor helper point (or its position), axes from the creature-to-lamp direction, radius four times the distance, and per-frame deltas toward a random side
// Grade: DIFFER(475v475) -- scheduling only: the `mov edx,[esp+1ch]` reload of the AxisY = Origin - pos temporary sits before `fstp [esp+20h]` (retail after), and `mov [esp+14h],eax` (the unsigned side) is stored before `test eax,eax` (retail after)
// Leaf: OnInitLeaf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008539C0, length 475 bytes (padded to 480).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCGraphicAppearance::GetHelperPoint @ 0x004C0560 (the
//   global "TARGET_CURSOR_POINT" name @ 0x013BAC50), CThing::GetPosition @ 0x004C73D0, the owner camera mode's
//   GetPWorldSeed @ 0x0084D2B0 (manifest label "_Cons_val<...>" is propagated), GFROR13 @ 0x00497890 (__fastcall).
// Constants: 1.0f 0x0122DED8, 4.0f 0x0123088C, 2^32 0x0122DCB4 (unsigned conversion fix-up), 0.5f 0x0122F59C,
//   2/3 0x012544AC.
// Layout: PDB NCameraModes::CScriptModuleUseLamp (0xa0 bytes, as the landed ctor 0x00853120): POwner +0x38,
//   WFDuration +0x4c, PCreature +0x54, PLamp +0x5c, Origin +0x64, Theta +0x70, Rad +0x74, Height +0x78,
//   DeltaTheta +0x7c, DeltaRad +0x80, DeltaHeight +0x84, AxisX +0x88, AxisY +0x94; CThing's graphic appearance +0x64.
// Shape: the sibling CScriptModuleOpenChest::OnInitLeaf (0x00853670) with the helper-point origin; IS_THING_ALIVE as a
//   macro; a `__forceinline` Normalise returning the length (components read Z, Y, X into locals); the perpendicular
//   axis through an inline member of AxisY (its address stays in eax); the target radius is a named float reused in
//   DeltaRad (`fst` to the frame); the random side is the landed seed step modulo 2 as an unsigned value.
#include <math.h>
#pragma intrinsic(sqrt)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    void operator*=(float s) { X *= s; Y *= s; Z *= s; }
    __forceinline float Normalise()
    {
        float z = Z;
        float y = Y;
        float x = X;
        float length = (float)sqrt(x * x + y * y + z * z);
        *this *= 1.0f / length;
        return length;
    }
    C3DVector PerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }
};
static inline float Magnitude(C3DVector v) { return (float)sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }

struct CBaseObject;
struct CCharString { void* m_pData; };
extern CCharString g_CCharString_013bac50;           // 0x013BAC50 "TARGET_CURSOR_POINT"
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time);   // 0x004C0560
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;      // +0x64
    unsigned char _pad68[0x29];
    unsigned char m_dead : 1;                        // +0x91 bit 0
    const C3DVector& GetPosition() const;            // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};
#define IS_THING_ALIVE(p) ((p).GetPItem() != 0 && !(p)->m_dead)

namespace NCameraModes {
struct CBase {                                       // the owning camera mode
    unsigned long* GetPWorldSeed();                  // 0x0084D2B0
};

struct CScriptModuleUseLamp {
    virtual void v00();
    unsigned char                   _pad04[0x34];
    CBase*                          POwner;          // +0x38
    unsigned char                   _pad3c[0x10];    // +0x3c base state
    long                            WFDuration;      // +0x4c
    long                            EndFrame;        // +0x50
    CIntelligentPointer_CThingConst PCreature;       // +0x54
    CIntelligentPointer_CThingConst PLamp;          // +0x5c
    C3DVector Origin;                                // +0x64
    float     Theta;                                 // +0x70
    float     Rad;                                   // +0x74
    float     Height;                                // +0x78
    float     DeltaTheta;                            // +0x7c
    float     DeltaRad;                              // +0x80
    float     DeltaHeight;                           // +0x84
    C3DVector AxisX;                                 // +0x88
    C3DVector AxisY;                                 // +0x94
    void OnInitLeaf();
};

void CScriptModuleUseLamp::OnInitLeaf()
{
    if (!IS_THING_ALIVE(PLamp))
        return;
    if (!IS_THING_ALIVE(PCreature))
        return;
    CTCGraphicAppearance* appearance = PLamp->m_pGraphicAppearance;
    if (!appearance->GetHelperPoint(g_CCharString_013bac50, 0, 0, &Origin, 1.0f))
        Origin = PLamp->GetPosition();
    AxisY = Origin - PCreature->GetPosition();
    AxisY.Z = 0.0f;
    float length = AxisY.Normalise();
    AxisX = AxisY.PerpendicularXY();
    Theta = 0.0f;
    float radius = length * 4.0f;
    Rad = radius;
    Height = 0.5f;
    unsigned long* seed = POwner->GetPWorldSeed();
    unsigned long next = *seed * 0x24A1U + 0x24DFU;
    *seed = next;
    unsigned long rotated = GFROR13(next);
    *seed = rotated;
    unsigned long side = rotated % 2;
    DeltaTheta = (((float)side - 0.5f) * 0.6666667f - Theta) / (float)WFDuration;
    DeltaRad = (radius - Rad) / (float)WFDuration;
    DeltaHeight = (0.5f - Height) / (float)WFDuration;
}

}
