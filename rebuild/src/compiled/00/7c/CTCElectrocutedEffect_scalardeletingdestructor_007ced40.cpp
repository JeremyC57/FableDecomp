// CTCElectrocutedEffect::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCElectrocutedEffect@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CED40, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x0126F044; calls 0x007CE730.
// Callees: CTCElectrocutedEffect::~CTCElectrocutedEffect 0x007CE730 (??1CTCElectrocutedEffect@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CTCElectrocutedEffect::ScalarDeletingDestructor (member stand-in)
//   calling CTCElectrocutedEffect::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCElectrocutedEffect {
public:
    CTCElectrocutedEffect(CThing&);   // ??0CTCElectrocutedEffect@@QAE@AAVCThing@@@Z
    virtual ~CTCElectrocutedEffect();                   // 0x007CE730 (??1CTCElectrocutedEffect@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCElectrocutedEffect@@QAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCElectrocutedEffect::CTCElectrocutedEffect(CThing&) { }
