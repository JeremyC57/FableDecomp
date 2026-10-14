// NCameraModes::CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector&, const C3DVector&) -- CScriptModuleBase(), an empty CIntelligentPointer and two vectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007187C0, oracle length 78 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase @ 0x005ED070, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (landed).
// Vtables: 0x012657D8 = NCameraModes::CScriptModuleGoToLookAt, 0x01237F1C = CIntelligentPointer (rtti-vtables.tsv);
// the manifest label "C3DPositionInterpolator::C3DPositionInterpolator" is propagated and rejected.
//
// Retail bytes:  push esi ; push edi ; mov esi,ecx ; call CScriptModuleBase::CScriptModuleBase ; lea edi,[esi+4Ch] ; mov ecx,edi ;
//                mov [esi],offset GoToLookAt::`vftable' ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov eax,[esp+0Ch] ;
//                mov [edi],offset CIntelligentPointer::`vftable' ; (three dword moves into +54h..+5Ch) ; mov eax,[esp+10h] ;
//                (three dword moves into +60h..+68h) ; pop edi ; mov eax,esi ; pop esi ; ret 8
// Shape: CScriptModuleBase is 0x4C bytes. The member at +0x4C is a CIntelligentPointer<T> (8 bytes:
// vtable + pointee) whose inline constructor runs the CBaseIntelligentPointer constructor and then
// stores its own vtable -- which is why the derived vtable store is scheduled before that call and the
// CIntelligentPointer vtable store after it. The two C3DVector members at +0x54 and +0x60 are copied
// as three dword moves each. No PDB layout for the camera module classes; member names are descriptive.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4C - 4];       // +0x4 .. +0x4C: base state, untouched here
    CScriptModuleBase();                   // 0x005ED070
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pThing;                        // +4
    CBaseIntelligentPointer();             // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                    // own vtable (0x01237F1C)
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

namespace NCameraModes {

struct CScriptModuleGoToLookAt : CScriptModuleBase {
    virtual void v00();                    // own vtable (0x012657D8)
    CIntelligentPointer m_pLookAtThing;    // +0x4C
    C3DVector           m_goToPosition;    // +0x54
    C3DVector           m_lookAtPosition;  // +0x60
    CScriptModuleGoToLookAt(const C3DVector& goTo, const C3DVector& lookAt);
};

CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector& goTo, const C3DVector& lookAt)
    : CScriptModuleBase(), m_pLookAtThing(), m_goToPosition(goTo), m_lookAtPosition(lookAt)
{
}

}
