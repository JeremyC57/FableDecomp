// NScript::CQ_NewOakValeIntroScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCQ_NewOakValeIntroScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBEFA0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c676): slot 0 of vtable 0x012D7A28 (??_7CQ_NewOakValeIntroScript@NScript@@6B@, stored by 0x00DAAC00); calls ~CQ_NewOakValeIntroScript 0x00DBEFC0 then delete on bit 0.
// Callees: NScript::CQ_NewOakValeIntroScript::~CQ_NewOakValeIntroScript 0x00DBEFC0 (??1CQ_NewOakValeIntroScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00dbefc0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_NewOakValeIntroScript {
public:
    CQ_NewOakValeIntroScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CQ_NewOakValeIntroScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CQ_NewOakValeIntroScript();                // 0x00DBEFC0 (??1CQ_NewOakValeIntroScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CQ_NewOakValeIntroScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
