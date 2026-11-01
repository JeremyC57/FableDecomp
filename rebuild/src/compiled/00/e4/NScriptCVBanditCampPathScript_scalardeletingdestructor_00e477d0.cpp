// NScript::CV_BanditCampPathScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_BanditCampPathScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E477D0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of quest vtable 0x012E4A9C (identity as 0x00E477F0); calls this chunk's row.
// Callees: NScript::CV_BanditCampPathScript::~CV_BanditCampPathScript 0x00E477F0 (??1CV_BanditCampPathScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00e477f0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_BanditCampPathScript {
public:
    CV_BanditCampPathScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00E46BD0
    virtual ~CV_BanditCampPathScript();                 // 0x00E477F0 (??1CV_BanditCampPathScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_BanditCampPathScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z 0x00E46BD0) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_BanditCampPathScript::CV_BanditCampPathScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
