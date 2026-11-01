// NScript::CQ_WizardBattleScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCQ_WizardBattleScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1DA20, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of 0x012E0F54.
// Callees: NScript::CQ_WizardBattleScript::~CQ_WizardBattleScript 0x00E1DA40 (??1CQ_WizardBattleScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00e1da40; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_WizardBattleScript {
public:
    CQ_WizardBattleScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CQ_WizardBattleScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CQ_WizardBattleScript();                   // 0x00E1DA40 (??1CQ_WizardBattleScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CQ_WizardBattleScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_WizardBattleScript::CQ_WizardBattleScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
