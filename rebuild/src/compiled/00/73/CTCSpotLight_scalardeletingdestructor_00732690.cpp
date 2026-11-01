// CTCSpotLight::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCSpotLight@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00732690, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x01266FBC; calls 0x00731DB0.
// Callees: CTCSpotLight::~CTCSpotLight 0x00731DB0 (??1CTCSpotLight@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CTCSpotLight::ScalarDeletingDestructor (member stand-in)
//   calling CTCSpotLight::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCSpotLight {
public:
    CTCSpotLight(CThing&);   // ??0CTCSpotLight@@QAE@AAVCThing@@@Z
    virtual ~CTCSpotLight();                            // 0x00731DB0 (??1CTCSpotLight@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCSpotLight@@QAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCSpotLight::CTCSpotLight(CThing&) { }
