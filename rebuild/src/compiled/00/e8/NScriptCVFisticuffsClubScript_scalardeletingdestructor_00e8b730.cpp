// NScript::CV_FisticuffsClubScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_FisticuffsClubScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8B730, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x012EAF14; scalar form; FableWin.pdb ??_GCV_FisticuffsClubScript@NScript@@UAEPAXI@Z.
// Callees: NScript::CV_FisticuffsClubScript::~CV_FisticuffsClubScript 0x00E8B750 (??1CV_FisticuffsClubScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00e8b750; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_FisticuffsClubScript {
public:
    CV_FisticuffsClubScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CV_FisticuffsClubScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CV_FisticuffsClubScript();                 // 0x00E8B750 (??1CV_FisticuffsClubScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_FisticuffsClubScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_FisticuffsClubScript::CV_FisticuffsClubScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
