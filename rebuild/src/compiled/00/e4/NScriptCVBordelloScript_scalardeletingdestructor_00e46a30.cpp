// NScript::CV_BordelloScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_BordelloScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E46A30, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of vtable 0x012E38B0 (its only reference); calls the dtor 0x00E46A50 proven in this chunk, then operator delete on flag bit 0 (scalar shape).
// Callees: NScript::CV_BordelloScript::~CV_BordelloScript 0x00E46A50 (??1CV_BordelloScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00e46a50; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_BordelloScript {
public:
    CV_BordelloScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CV_BordelloScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CV_BordelloScript();                       // 0x00E46A50 (??1CV_BordelloScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_BordelloScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_BordelloScript::CV_BordelloScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
