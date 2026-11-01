// CCutsceneDef::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCCutsceneDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29DA0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c674): slot 0 of CCutsceneDef vtable 0x012FB6E0 (landed CCutsceneDef_Alloc_00f2a0d0.cpp comment says so too); calls 0x00F29DC0.
// Callees: CCutsceneDef::~CCutsceneDef 0x00F29DC0 (??1CCutsceneDef@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor
//   calling CLandscapeBackgroundPatch_dtor_00f29dc0; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CCutsceneDef {
public:
    CCutsceneDef();   // 0x00F29D00
    virtual ~CCutsceneDef();                            // 0x00F29DC0 (??1CCutsceneDef@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CCutsceneDef@@QAE@XZ 0x00F29D00) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CCutsceneDef::CCutsceneDef() { }
