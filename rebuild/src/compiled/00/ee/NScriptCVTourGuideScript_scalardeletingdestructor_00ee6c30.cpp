// NScript::CV_TourGuideScript::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCV_TourGuideScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE6C30, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x012F46B8; scalar form; Ego_r.pdb ??_GCV_TourGuideScript@NScript@@UAEPAXI@Z.
// Callees: NScript::CV_TourGuideScript::~CV_TourGuideScript 0x00EE6C50 (??1CV_TourGuideScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00ee6c50; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CGameScriptInterfaceBase;

namespace NScript {
class CV_TourGuideScript {
public:
    CV_TourGuideScript(CScriptDataBase*, CGameScriptInterfaceBase&);   // ??0CV_TourGuideScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    virtual ~CV_TourGuideScript();                      // 0x00EE6C50 (??1CV_TourGuideScript@NScript@@UAE@XZ)
};
}

// TU emitter: the PDB constructor (??0CV_TourGuideScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_TourGuideScript::CV_TourGuideScript(CScriptDataBase*, CGameScriptInterfaceBase&) { }
