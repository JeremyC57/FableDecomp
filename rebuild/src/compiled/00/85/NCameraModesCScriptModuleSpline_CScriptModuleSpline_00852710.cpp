// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's base / member construct calls to the PDB constructor spelling
//   (T1 tracker; target self rows' Leaf): the row's own bodiless model ctor takes the PDB parameter types, call sites pass *x:
//   0x005ED070 ??0CScriptModuleBase@NCameraModes@@QAE@XZ (was ??0CScriptModuleBase@@QAE@XZ); the model class is wrapped in namespace NCameraModes (PDB scope) with a using-declaration
// NCameraModes::CScriptModuleSpline::CScriptModuleSpline(long nodeToStartOn, long nodeToEndOn) -- CScriptModuleBase(), then the spline state: index 0, FOV 0.2, roll 0, both finished flags clear, the node range from the arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852710, length 53 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase @ 0x005ED070. Vtable 0x01275A6C = NCameraModes::CScriptModuleSpline (rtti-vtables.tsv);
// the manifest label "CDataInputStream::CDataInputStream" is propagated from a same-shape body.
// Shape: CScriptModuleBase is 0x4C bytes; the zero floats and bools share the `xor eax,eax` register, the 0.2f
// member is a 10-byte immediate dword store (VC stores float constants as their bit pattern), the two long
// arguments are copied through ecx/eax. PDB NCameraModes::CScriptModuleSpline (100 bytes): Index +0x4c,
// CurrentSplineFOV +0x50, CurrentSplineRollAngle +0x54, ReallyFinished +0x58, FinishedSpline +0x59,
// NodeToStartOn +0x5c, NodeToEndOn +0x60.
namespace NCameraModes { struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4C - 4];       // +0x4 .. +0x4C: base state, untouched here
    CScriptModuleBase();                   // 0x005ED070
}; } using NCameraModes::CScriptModuleBase;

namespace NCameraModes {

struct CScriptModuleSpline : CScriptModuleBase {
    virtual void v00();                    // own vtable (0x01275A6C)
    float Index;                           // +0x4c
    float CurrentSplineFOV;                // +0x50
    float CurrentSplineRollAngle;          // +0x54
    bool  ReallyFinished;                  // +0x58
    bool  FinishedSpline;                  // +0x59
    long  NodeToStartOn;                   // +0x5c
    long  NodeToEndOn;                     // +0x60
    CScriptModuleSpline(long nodeToStartOn, long nodeToEndOn);
};

CScriptModuleSpline::CScriptModuleSpline(long nodeToStartOn, long nodeToEndOn)
    : CScriptModuleBase(), Index(0.0f), CurrentSplineFOV(0.2f), CurrentSplineRollAngle(0.0f),
      ReallyFinished(false), FinishedSpline(false), NodeToStartOn(nodeToStartOn), NodeToEndOn(nodeToEndOn)
{
}

}