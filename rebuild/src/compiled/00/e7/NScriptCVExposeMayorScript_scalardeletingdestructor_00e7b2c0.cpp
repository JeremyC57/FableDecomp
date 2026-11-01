// NScript::CV_ExposeMayorScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_ExposeMayorScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7B2C0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of vtable 0x012E96CC (its only reference); calls the dtor 0x00E7B2E0 proven in this chunk, then operator delete on flag bit 0 (scalar shape).
// Callees: NScript::CV_ExposeMayorScript::~CV_ExposeMayorScript 0x00E7B2E0 (??1CV_ExposeMayorScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00e7b2e0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_ExposeMayorScript {
public:
    CV_ExposeMayorScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CV_ExposeMayorScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CV_ExposeMayorScript();                    // 0x00E7B2E0 (??1CV_ExposeMayorScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_ExposeMayorScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_ExposeMayorScript::CV_ExposeMayorScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
