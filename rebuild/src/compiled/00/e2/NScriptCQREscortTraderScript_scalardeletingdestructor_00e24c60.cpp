// NScript::CQR_EscortTraderScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCQR_EscortTraderScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E24C60, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of vtable 0x012E12E0 (its only reference); calls the dtor 0x00E24C80 proven in this chunk, then operator delete on flag bit 0 (scalar shape).
// Callees: NScript::CQR_EscortTraderScript::~CQR_EscortTraderScript 0x00E24C80 (??1CQR_EscortTraderScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00e24c80; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CQR_EscortTraderScript {
public:
    CQR_EscortTraderScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CQR_EscortTraderScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CQR_EscortTraderScript();                  // 0x00E24C80 (??1CQR_EscortTraderScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CQR_EscortTraderScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQR_EscortTraderScript::CQR_EscortTraderScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
