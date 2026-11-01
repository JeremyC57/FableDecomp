// CTCMultiStrike::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCMultiStrike@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CD810, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x0126EE0C; calls 0x007CCA90.
// Callees: CTCMultiStrike::~CTCMultiStrike 0x007CCA90 (??1CTCMultiStrike@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CTCMultiStrike::ScalarDeletingDestructor (member stand-in)
//   calling CTCMultiStrike::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCMultiStrike {
public:
    CTCMultiStrike(CThing&);   // ??0CTCMultiStrike@@QAE@AAVCThing@@@Z
    virtual ~CTCMultiStrike();                          // 0x007CCA90 (??1CTCMultiStrike@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCMultiStrike@@QAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCMultiStrike::CTCMultiStrike(CThing&) { }
