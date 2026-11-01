// NScript::CQ_RansomVictimScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCQ_RansomVictimScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEBC90, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of vtable 0x012DCE70 (its only reference); calls the dtor 0x00DEBCB0 proven in this chunk, then operator delete on flag bit 0 (scalar shape).
// Callees: NScript::CQ_RansomVictimScript::~CQ_RansomVictimScript 0x00DEBCB0 (??1CQ_RansomVictimScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00debcb0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_RansomVictimScript {
public:
    CQ_RansomVictimScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CQ_RansomVictimScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CQ_RansomVictimScript();                   // 0x00DEBCB0 (??1CQ_RansomVictimScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CQ_RansomVictimScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_RansomVictimScript::CQ_RansomVictimScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
