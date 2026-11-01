// NScript::CCS_OakValeRevisitedScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCCS_OakValeRevisitedScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8C10, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c692): slot 0 of the RTTI-less vtable 0x012F53C0, which the ctor 0x00EE8100 stores; FableWin.pdb
//   ??_GCCS_OakValeRevisitedScript@NScript@@UAEPAXI@Z (Ego_r.pdb names the class's ??_E / ??1 / ??0 in NScript).
// Callees: NScript::CCS_OakValeRevisitedScript::~CCS_OakValeRevisitedScript 0x00EE8C30 (??1CCS_OakValeRevisitedScript@NScript@@UAE@XZ),
//   operator delete 0x00BFE9BC.
// Naming wave 25 (c692 queue): landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor calling
//   CLandscapeBackgroundPatch_dtor_00ee8c30; re-authored on the compiler-generated ??_G as the wave 16 rows (0x00E68E90) were
//   (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CCS_OakValeRevisitedScript {
public:
    CCS_OakValeRevisitedScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00EE8100 (??0CCS_OakValeRevisitedScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z)
    virtual ~CCS_OakValeRevisitedScript();               // 0x00EE8C30 (??1CCS_OakValeRevisitedScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor defined with an empty body emits the vtable and with it this ??_G (the constructor body itself is its
//   own row, 0x00EE8100; not part of this graded body).
NScript::CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
