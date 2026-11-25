// NCameraModes::CScriptModuleGoToLookInDirection::CScriptModuleGoToLookInDirection(const C3DVector& goTo, const C3DVector& lookDirection) -- CScriptModuleBase(), vtable, copy both vectors, then normalise the look direction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ED190, length 123 bytes.
// Leaf: ??0CScriptModuleGoToLookInDirection
// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's base / member construct calls to the PDB constructor spelling
//   (T1 tracker; target self rows' Leaf): the row's own bodiless model ctor takes the PDB parameter types, call sites pass *x:
//   0x005ED070 ??0CScriptModuleBase@NCameraModes@@QAE@XZ (was ??0CScriptModuleBase@@QAE@XZ); the model class is wrapped in namespace NCameraModes (PDB scope) with a using-declaration
// Callees: CScriptModuleBase::CScriptModuleBase @ 0x005ED070. Vtable 0x01255F9C =
// NCameraModes::CScriptModuleGoToLookInDirection (rtti-vtables.tsv); the manifest label
// "C3DPositionInterpolator::C3DPositionInterpolator" is propagated (its only caller is the module factory at 0x005ED24F).
// Layout: PDB CScriptModuleGoToLookInDirection (0x64 bytes): GoTo +0x4c, LookDirection +0x58 over the 0x4c-byte
// CScriptModuleBase (as the landed Update 0x00851E70 and the sibling Dolly constructor 0x008AAE80).
// Shape: the vectors copy as three dword moves each (the engine's memberwise C3DVector copy constructor); the
// inline Normalise() multiplies each component by `1.0f / Magnitude()` (`fdivr [1.0f]`), Magnitude being the
// intrinsic sqrt of x*x + y*y + z*z (VC keeps the three components on the x87 stack and squares the copies).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
    float Magnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise()
    {
        float scale = 1.0f / Magnitude();
        X *= scale;
        Y *= scale;
        Z *= scale;
    }
};

namespace NCameraModes { struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4C - 4];       // +0x4 .. +0x4C: base state, untouched here
    CScriptModuleBase();                   // 0x005ED070
}; } using NCameraModes::CScriptModuleBase;

namespace NCameraModes {

struct CScriptModuleGoToLookInDirection : CScriptModuleBase {
    virtual void v00();                    // own vtable (0x01255F9C)
    C3DVector GoTo;                        // +0x4C
    C3DVector LookDirection;               // +0x58
    CScriptModuleGoToLookInDirection(const C3DVector& goTo, const C3DVector& lookDirection);
};

CScriptModuleGoToLookInDirection::CScriptModuleGoToLookInDirection(const C3DVector& goTo, const C3DVector& lookDirection)
    : CScriptModuleBase(), GoTo(goTo), LookDirection(lookDirection)
{
    LookDirection.Normalise();
}

}