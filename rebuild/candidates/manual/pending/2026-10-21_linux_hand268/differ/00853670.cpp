// NCameraModes::CScriptModuleOpenChest::OnInitLeaf() -- when both the chest and the creature are alive, set up the orbit: origin at the chest, axes from the creature-to-chest direction, start radius twice the distance, and per-frame deltas toward a random side
// Grade: DIFFER(445v445) -- scheduling only: the `mov edx,[esp+18h]` reload of the AxisY = Origin - pos temporary sits before `fstp [esp+1ch]` (retail after), and the AxisX.Z store `mov [eax+8],edx` sinks below `fstp [esi+74h]` (retail stores it before Theta)
// Leaf: OnInitLeaf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00853670, length 445 bytes (padded to 448).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, the owner camera mode's
//   GetPWorldSeed @ 0x0084D2B0 (`[[this+0x40]+0x74]`, then the world-seed getter; manifest label "_Cons_val<...>" is
//   propagated), GFROR13 @ 0x00497890 (__fastcall).
// Constants: 1.0f 0x0122DED8, 2^32 0x0122DCB4 (unsigned conversion fix-up), 0.5f 0x0122F59C, 2/3 0x012544AC,
//   1.6f 0x0125A9A0, 1.5f 0x0125AADC.
// Layout: PDB NCameraModes::CScriptModuleOpenChest (0xa0 bytes, as the landed ctor 0x008530B0): POwner +0x38,
//   WFDuration +0x4c, PCreature +0x54, PChest +0x5c, Origin +0x64, Theta +0x70, Rad +0x74, Height +0x78,
//   DeltaTheta +0x7c, DeltaRad +0x80, DeltaHeight +0x84, AxisX +0x88, AxisY +0x94.
// Shape: IS_THING_ALIVE as a macro (GetPItem twice, `test byte [eax+91h],1`); C3DVector has a user copy ctor (the
//   `lea`-pointer copies); a `__forceinline` Normalise returning the length (components read Z, Y, X into locals, sqrt
//   intrinsic); the perpendicular axis through an inline member of AxisY (its address stays in eax); the random side is
//   the landed seed step `seed = seed*0x24A1 + 0x24DF; seed = GFROR13(seed)` modulo 2 as an unsigned value; each delta
//   divides by `(float)WFDuration` (a named float local becomes a reciprocal multiply).
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

struct CBaseObject;
struct CThing {
    unsigned char _pad00[0x91];
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

struct CScriptModuleOpenChest {
    virtual void v00();
    unsigned char                   _pad04[0x34];
    CBase*                          POwner;          // +0x38
    unsigned char                   _pad3c[0x10];    // +0x3c base state
    long                            WFDuration;      // +0x4c
    long                            EndFrame;        // +0x50
    CIntelligentPointer_CThingConst PCreature;       // +0x54
    CIntelligentPointer_CThingConst PChest;          // +0x5c
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

void CScriptModuleOpenChest::OnInitLeaf()
{
    if (!IS_THING_ALIVE(PChest))
        return;
    if (!IS_THING_ALIVE(PCreature))
        return;
    Origin = PChest->GetPosition();
    AxisY = Origin - PCreature->GetPosition();
    AxisY.Z = 0.0f;
    float length = AxisY.Normalise();
    AxisX = AxisY.PerpendicularXY();
    Theta = 0.0f;
    Rad = length * 2.0f;
    Height = 3.0f;
    unsigned long* seed = POwner->GetPWorldSeed();
    unsigned long next = *seed * 0x24A1U + 0x24DFU;
    *seed = next;
    unsigned long rotated = GFROR13(next);
    *seed = rotated;
    unsigned long side = rotated % 2;
    DeltaTheta = (((float)side - 0.5f) * 0.6666667f - Theta) / (float)WFDuration;
    DeltaRad = (length * 1.6f - Rad) / (float)WFDuration;
    DeltaHeight = (1.5f - Height) / (float)WFDuration;
}

}
