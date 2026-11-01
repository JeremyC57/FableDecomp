// CTCTurncoatSpell::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCTurncoatSpell@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C77D0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x0126E5C4; calls 0x007C66F0.
// Callees: CTCTurncoatSpell::~CTCTurncoatSpell 0x007C66F0 (??1CTCTurncoatSpell@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CTCTurncoatSpell::ScalarDeletingDestructor (member stand-in)
//   calling CTCTurncoatSpell::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCTurncoatSpell {
public:
    CTCTurncoatSpell(CThing&);   // ??0CTCTurncoatSpell@@QAE@AAVCThing@@@Z
    virtual ~CTCTurncoatSpell();                        // 0x007C66F0 (??1CTCTurncoatSpell@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCTurncoatSpell@@QAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCTurncoatSpell::CTCTurncoatSpell(CThing&) { }
