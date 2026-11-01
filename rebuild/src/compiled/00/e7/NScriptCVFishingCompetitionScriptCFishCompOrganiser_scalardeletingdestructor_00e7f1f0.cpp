// NScript::CV_FishingCompetitionScript::CFishCompOrganiser::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCFishCompOrganiser@CV_FishingCompetitionScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7F1F0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c681): slot 0 of vtable 0x012EA620 (its only reference); calls the dtor 0x00E7F210 proven in this chunk, then operator delete on flag bit 0 (scalar shape).
// Callees: NScript::CV_FishingCompetitionScript::CFishCompOrganiser::~CFishCompOrganiser 0x00E7F210 (??1CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00e7f210; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CScriptThing;

namespace NScript {
class CV_FishingCompetitionScript {
public:
    class CFishCompOrganiser {
    public:
        CFishCompOrganiser(CV_FishingCompetitionScript&, CScriptDataBase*, const CScriptThing&);   // ??0CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
        virtual ~CFishCompOrganiser();                      // 0x00E7F210 (??1CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@UAE@XZ)
    };
};
}

// TU emitter: the PDB constructor (??0CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CV_FishingCompetitionScript::CFishCompOrganiser::CFishCompOrganiser(CV_FishingCompetitionScript&, CScriptDataBase*, const CScriptThing&) { }
