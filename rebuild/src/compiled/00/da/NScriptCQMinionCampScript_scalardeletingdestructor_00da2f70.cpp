// NScript::CQ_MinionCampScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCQ_MinionCampScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA2F70, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of vtable 0x012D6C60 (its only reference); calls the dtor 0x00DA2F90 proven in this chunk, then operator delete on flag bit 0 (scalar shape).
// Callees: NScript::CQ_MinionCampScript::~CQ_MinionCampScript 0x00DA2F90 (??1CQ_MinionCampScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00da2f90; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_MinionCampScript {
public:
    CQ_MinionCampScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CQ_MinionCampScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CQ_MinionCampScript();                     // 0x00DA2F90 (??1CQ_MinionCampScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CQ_MinionCampScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_MinionCampScript::CQ_MinionCampScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
