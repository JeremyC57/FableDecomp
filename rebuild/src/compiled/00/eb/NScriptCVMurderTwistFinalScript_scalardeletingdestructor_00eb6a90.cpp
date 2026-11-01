// NScript::CV_MurderTwistFinalScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_MurderTwistFinalScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB6A90, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of 0x012EF978.
// Callees: NScript::CV_MurderTwistFinalScript::~CV_MurderTwistFinalScript 0x00EB6AB0 (??1CV_MurderTwistFinalScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00eb6ab0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_MurderTwistFinalScript {
public:
    CV_MurderTwistFinalScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CV_MurderTwistFinalScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CV_MurderTwistFinalScript();               // 0x00EB6AB0 (??1CV_MurderTwistFinalScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_MurderTwistFinalScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_MurderTwistFinalScript::CV_MurderTwistFinalScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
