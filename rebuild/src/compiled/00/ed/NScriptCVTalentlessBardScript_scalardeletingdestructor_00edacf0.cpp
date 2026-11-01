// NScript::CV_TalentlessBardScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_TalentlessBardScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDACF0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x012F3428 (class proven above); scalar form; FableWin.pdb ??_GCV_TalentlessBardScript@NScript@@UAEPAXI@Z.
// Callees: NScript::CV_TalentlessBardScript::~CV_TalentlessBardScript 0x00EDAD10 (??1CV_TalentlessBardScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00edad10; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_TalentlessBardScript {
public:
    CV_TalentlessBardScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CV_TalentlessBardScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CV_TalentlessBardScript();                 // 0x00EDAD10 (??1CV_TalentlessBardScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_TalentlessBardScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
