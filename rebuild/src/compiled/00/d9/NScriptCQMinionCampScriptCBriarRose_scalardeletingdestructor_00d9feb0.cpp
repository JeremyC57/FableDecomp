// NScript::CQ_MinionCampScript::CBriarRose::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCBriarRose@CQ_MinionCampScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9FEB0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of 0x012D6D0C.
// Callees: NScript::CQ_MinionCampScript::CBriarRose::~CBriarRose 0x00D9FED0 (??1CBriarRose@CQ_MinionCampScript@NScript@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00d9fed0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CScriptDataBase;
class CScriptThing;

namespace NScript {
class CQ_MinionCampScript {
public:
    class CBriarRose {
    public:
        CBriarRose(CQ_MinionCampScript&, CScriptDataBase*, const CScriptThing&);   // ??0CBriarRose@CQ_MinionCampScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
        virtual ~CBriarRose();                              // 0x00D9FED0 (??1CBriarRose@CQ_MinionCampScript@NScript@@UAE@XZ)
    };
};
}

// TU emitter: the PDB constructor (??0CBriarRose@CQ_MinionCampScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
NScript::CQ_MinionCampScript::CBriarRose::CBriarRose(CQ_MinionCampScript&, CScriptDataBase*, const CScriptThing&) { }
