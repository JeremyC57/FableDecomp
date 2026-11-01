// NScript::CQ_HangingTreeGoodScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCQ_HangingTreeGoodScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D782C0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c677): slot 0 of vtable 0x012D260C (the CQ_HangingTreeGoodScript vtable stored by ctor 0x00D6EF20).
// Callees: NScript::CQ_HangingTreeGoodScript::~CQ_HangingTreeGoodScript 0x00D782E0 (??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00d782e0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    CQ_HangingTreeGoodScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CQ_HangingTreeGoodScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CQ_HangingTreeGoodScript();                // 0x00D782E0 (??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CQ_HangingTreeGoodScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
