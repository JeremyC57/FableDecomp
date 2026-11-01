// NScript::CLadyGreyWifeManagerScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCLadyGreyWifeManagerScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF91F0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of vtable 0x012C6C10 (its only reference); calls the dtor 0x00CF9210 proven in this chunk, then operator delete on flag bit 0 (scalar shape).
// Callees: NScript::CLadyGreyWifeManagerScript::~CLadyGreyWifeManagerScript 0x00CF9210 (??1CLadyGreyWifeManagerScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00cf9210; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CLadyGreyWifeManagerScript {
public:
    CLadyGreyWifeManagerScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CLadyGreyWifeManagerScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CLadyGreyWifeManagerScript();              // 0x00CF9210 (??1CLadyGreyWifeManagerScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CLadyGreyWifeManagerScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CLadyGreyWifeManagerScript::CLadyGreyWifeManagerScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
