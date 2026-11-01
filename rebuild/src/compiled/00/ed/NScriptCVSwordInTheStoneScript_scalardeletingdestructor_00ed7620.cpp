// NScript::CV_SwordInTheStoneScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_SwordInTheStoneScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7620, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x012F301C (stored by the CV_SwordInTheStoneScript ctor 0x00ED4AA0); Ego_r.pdb name set.
// Callees: NScript::CV_SwordInTheStoneScript::~CV_SwordInTheStoneScript 0x00ED7640 (??1CV_SwordInTheStoneScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00ed7640; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_SwordInTheStoneScript {
public:
    CV_SwordInTheStoneScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CV_SwordInTheStoneScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CV_SwordInTheStoneScript();                // 0x00ED7640 (??1CV_SwordInTheStoneScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_SwordInTheStoneScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
