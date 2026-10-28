// CWorld::ConsoleSetFreeCameraLookVector(CWorld& world, const vector<CGenericVar>& args) -- console command: point the main player's free camera along (args[0], args[1], args[2]) read as floats, with the up vector the normalised cross product of the horizontal right vector (look.Y, -look.X, 0) -- or (1, 0, 0) when the look is vertical -- and the look (static)
// Grade: DIFFER(310v310) -- x87 operand order in the cross product only: retail forms the up vector's middle term as `fld right.X ; fmul look.Z` and its last term with `fxch st(1) ; fmulp st(2)`, ours as `fld look.Z ; fmul right.X` and `fxch st(2) ; fmul st(1)`; all 64 operand orders of operator^, a free CrossProduct, const-reference aliases, a C3DVector right and four degenerate-branch forms give the same code
// Leaf: ?ConsoleSetFreeCameraLookVector@CWorld@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049ED80, 310 bytes, plain `ret` (static __fastcall: the world in ecx, the argument
//   vector in edx); FPO speed TU (esp frame, 0x20 bytes).
// Identity (proven): (1) Ego_r.pdb names ?ConsoleSetFreeCameraLookVector@CWorld@@KIXAAV1@ABV?$vec... (protected static
//   __fastcall, CWorld& + const vector<CGenericVar>&) beside ConsoleSetFreeCameraPos, whose landed body 0x0049ED30 is the
//   address just before this one with the same prologue (world.PlayerManager +0x0C -> GetMainPlayer, args[0..2].ValFloat at
//   +0x18 / +0x34 / +0x50 of the 28-byte CGenericVar); (2) it ends in CCamera::SetView(look, up) 0x00A0BE30 on
//   CPlayer::CurrentFreeCamera (+0x40), which stores the two vectors into CCamera::RHSet (Up +0x0C, Forward +0x18) and
//   normalises both. Like the other console commands it has no direct caller (registered through the console table).
// Label: the catalog's `_global::ConsoleSetFreeCameraLookVector` misses the class (protected static of CWorld).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970, CCamera::SetView(const C3DVector&, const C3DVector&) 0x00A0BE30.
//   Data: the inverse-sqrt mantissa table 0x013CE558, the shared 1.0e-4f tolerance 0x0129BA3C; literals 1.0f / 0.0f /
//   3.0f / 0.5f.
// Shape: the horizontal right vector is a C2DVector tested by its length against the tolerance; up = right ^ look
//   (C3DVector cross product, right.Z = 0.0f) and the inline fast Normalise (table estimate + one Newton step).
// Names: the tolerance's real name is not in the dumped name sets; the landed majority spelling is kept.
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
#include "engine/CPlayer.h"

extern unsigned long g_InvSqrtMantissaTable[128];       // 0x013CE558
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1.0e-4f)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C2DVector {                                       // Ego_r.pdb, 8 bytes
public:
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLength() const { float r = Y * Y; r += X * X; return (float)sqrt(r); }
};
class C3DVector {                                       // Ego_r.pdb, 12 bytes
public:
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator^(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    __forceinline void Normalise()
    {
        float lengthSquared = X * X + Y * Y + Z * Z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
class CCamera {                                         // Ego_r.pdb, 0x38 bytes (engine/CPlayer.h keeps it opaque)
public:
    void SetView(const C3DVector& forward, const C3DVector& up);   // 0x00A0BE30
};
class CCharString { public: void* PStringData; };
enum EGenericVarType { };
class CGenericVar {                                     // Ego_r.pdb, 28 bytes
public:
    int             _padding_;                          // +0x00
    EGenericVarType Type;                               // +0x04
    CCharString     ValString;                          // +0x08
    long            ValSLONG;                           // +0x0C
    unsigned long   ValULONG;                           // +0x10
    bool            ValBool;                            // +0x14
    float           ValFloat;                           // +0x18
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
};
}
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                           // 0x00449970
};
class CWorld : public CWorldLayout {
protected:
    static void __fastcall ConsoleSetFreeCameraLookVector(CWorld& world, const std::vector<CGenericVar>& args);
};

void __fastcall CWorld::ConsoleSetFreeCameraLookVector(CWorld& world, const std::vector<CGenericVar>& args)
{
    CPlayer& player = world.PlayerManager->GetMainPlayer();
    CCamera& camera = reinterpret_cast<CCamera&>(player.CurrentFreeCamera);
    C3DVector look(args[0].ValFloat, args[1].ValFloat, args[2].ValFloat);
    C2DVector side(look.Y, -look.X);
    if (side.GetLength() < g_epsilon_0129ba3c) {
        side.X = 1.0f;
        side.Y = 0.0f;
    }
    C3DVector right(side.X, side.Y, 0.0f);
    C3DVector up = right ^ look;
    up.Normalise();
    camera.SetView(look, up);
}
