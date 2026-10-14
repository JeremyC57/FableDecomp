// NCameraModes::CScriptModuleCameraOnAStick::CScriptModuleCameraOnAStick(T* target, const C3DVector&, const C3DVector&) -- CScriptModuleBase(), vtable, a pointer and two vectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AB450, oracle length 67 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase @ 0x005ED070. Vtable 0x0126CF04 = NCameraModes::CScriptModuleCameraOnAStick
// (rtti-vtables.tsv); the manifest label "C3DPositionInterpolator::C3DPositionInterpolator" is propagated and rejected.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CScriptModuleBase::CScriptModuleBase ; mov eax,[esp+8] ; mov [esi+4Ch],eax ;
//                mov eax,[esp+0Ch] ; mov [esi],offset vftable ; (three dword moves into +50h..+58h) ; mov eax,[esp+10h] ;
//                (three dword moves into +5Ch..+64h) ; mov eax,esi ; pop esi ; ret 0Ch
// Shape: CScriptModuleBase is 0x4C bytes (vtable + state); the first argument is stored raw at +0x4C
// before the vtable store (the compiler hoists the pointer store next to the argument load), then the
// two C3DVector members at +0x50 and +0x5C are copied as three dword moves each. No PDB layout for the
// camera module classes; member names are descriptive.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4C - 4];       // +0x4 .. +0x4C: base state, untouched here
    CScriptModuleBase();                   // 0x005ED070
};

namespace NCameraModes {

struct CScriptModuleCameraOnAStick : CScriptModuleBase {
    virtual void v00();                    // own vtable (0x0126CF04)
    void*     m_pStickTarget;              // +0x4C
    C3DVector m_offset;                    // +0x50
    C3DVector m_lookAt;                    // +0x5C
    CScriptModuleCameraOnAStick(void* pStickTarget, const C3DVector& offset, const C3DVector& lookAt);
};

CScriptModuleCameraOnAStick::CScriptModuleCameraOnAStick(void* pStickTarget, const C3DVector& offset, const C3DVector& lookAt)
    : CScriptModuleBase(), m_pStickTarget(pStickTarget), m_offset(offset), m_lookAt(lookAt)
{
}

}