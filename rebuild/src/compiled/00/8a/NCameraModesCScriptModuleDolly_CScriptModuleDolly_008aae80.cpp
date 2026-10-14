// NCameraModes::CScriptModuleDolly::CScriptModuleDolly(const C3DVector&, const C3DVector&, const float&) -- CScriptModuleBase(), vtable, two vectors and a float
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AAE80, oracle length 69 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase @ 0x005ED070. Vtable 0x01278160 = NCameraModes::CScriptModuleDolly
// (rtti-vtables.tsv); the manifest label "C3DPositionInterpolator::C3DPositionInterpolator" is propagated and rejected.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CScriptModuleBase::CScriptModuleBase ; mov eax,[esp+8] ; mov [esi],offset vftable ;
//                (three dword moves into +4Ch..+54h) ; mov eax,[esp+0Ch] ; mov ecx,[eax] ; mov [esi+58h],ecx ; mov edx,[eax+4] ;
//                mov ecx,[esp+10h] ; mov [esi+5Ch],edx ; mov eax,[eax+8] ; mov [esi+60h],eax ; mov edx,[ecx] ; mov [esi+64h],edx ;
//                mov eax,esi ; pop esi ; ret 0Ch
// Shape: CScriptModuleBase is 0x4C bytes; the two C3DVector members at +0x4C and +0x58 are copied as
// three dword moves each, and the third argument is read through its pointer (`mov edx,[ecx]`), so it
// is a reference to a 4-byte value (a float, the dolly duration/speed) stored at +0x64. No PDB layout
// for the camera module classes; member names are descriptive.
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

struct CScriptModuleDolly : CScriptModuleBase {
    virtual void v00();                    // own vtable (0x01278160)
    C3DVector m_start;                     // +0x4C
    C3DVector m_end;                       // +0x58
    float     m_time;                      // +0x64
    CScriptModuleDolly(const C3DVector& start, const C3DVector& end, const float& time);
};

CScriptModuleDolly::CScriptModuleDolly(const C3DVector& start, const C3DVector& end, const float& time)
    : CScriptModuleBase(), m_start(start), m_end(end), m_time(time)
{
}

}