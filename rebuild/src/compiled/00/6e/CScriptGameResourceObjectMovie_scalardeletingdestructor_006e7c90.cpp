// CScriptGameResourceObjectMovie::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCScriptGameResourceObjectMovie@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7C90, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x01260F00; Ego_r.pdb ??_GCScriptGameResourceObjectMovie@@UAEPAXI@Z.
// Callees: CScriptGameResourceObjectMovie::~CScriptGameResourceObjectMovie 0x006E7550 (??1CScriptGameResourceObjectMovie@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CScriptGameResourceObjectMovie::ScalarDeletingDestructor (member stand-in)
//   calling CScriptGameResourceObjectMovie::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CGameScriptInterface;

class CScriptGameResourceObjectMovie {
public:
    CScriptGameResourceObjectMovie(CGameScriptInterface&, bool);   // ??0CScriptGameResourceObjectMovie@@QAE@AAVCGameScriptInterface@@_N@Z
    virtual ~CScriptGameResourceObjectMovie();          // 0x006E7550 (??1CScriptGameResourceObjectMovie@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CScriptGameResourceObjectMovie@@QAE@AAVCGameScriptInterface@@_N@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CScriptGameResourceObjectMovie::CScriptGameResourceObjectMovie(CGameScriptInterface&, bool) { }
