// NScript::CQ_ArenaScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCQ_ArenaScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFAC20, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x012C7798; scalar form; Ego_r.pdb ??_GCQ_ArenaScript@NScript@@UAEPAXI@Z.
// Callees: NScript::CQ_ArenaScript::~CQ_ArenaScript 0x00CFAC40 (??1CQ_ArenaScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00cfac40; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_ArenaScript {
public:
    CQ_ArenaScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CQ_ArenaScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CQ_ArenaScript();                          // 0x00CFAC40 (??1CQ_ArenaScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CQ_ArenaScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_ArenaScript::CQ_ArenaScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
