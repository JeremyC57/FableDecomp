// CTCTrophy::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCTrophy@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DFB40, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x0125FC54; calls ~CTCTrophy 0x006DDB80.
// Callees: CTCTrophy::~CTCTrophy 0x006DDB80 (??1CTCTrophy@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CTCTrophy_scalar_deleting_destructor
//   calling CTCTrophy_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCTrophy {
public:
    CTCTrophy(CThing&);   // ??0CTCTrophy@@QAE@AAVCThing@@@Z
    virtual ~CTCTrophy();                               // 0x006DDB80 (??1CTCTrophy@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCTrophy@@QAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCTrophy::CTCTrophy(CThing&) { }
