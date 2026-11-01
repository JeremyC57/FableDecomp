// NScript::CQ_HangingTreeGoodScript::CHTG_Audience::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCHTG_Audience@CQ_HangingTreeGoodScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D728D0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of 0x012D292C; Ego_r+FableWin list this exact symbol.
// Callees: NScript::CQ_HangingTreeGoodScript::CHTG_Audience::~CHTG_Audience 0x00D728F0 (??1CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00d728f0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CScriptThing;

namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    class CHTG_Audience {
    public:
        CHTG_Audience(CQ_HangingTreeGoodScript&, CScriptDataBase*, const CScriptThing&);   // ??0CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
        virtual ~CHTG_Audience();                           // 0x00D728F0 (??1CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@UAE@XZ)
    };
};
}

// TU emitter: the PDB constructor (??0CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_HangingTreeGoodScript::CHTG_Audience::CHTG_Audience(CQ_HangingTreeGoodScript&, CScriptDataBase*, const CScriptThing&) { }
