// CTCOpinionSource::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCOpinionSource@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771A70, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x0126A81C; calls 0x00771A90.
// Callees: CTCOpinionSource::~CTCOpinionSource 0x00771A90 (??1CTCOpinionSource@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CTCOpinionSource_scalar_deleting_destructor
//   calling CTCOpinionSource_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCOpinionSource {
public:
    CTCOpinionSource(CThing&);   // ??0CTCOpinionSource@@QAE@AAVCThing@@@Z
    virtual ~CTCOpinionSource();                        // 0x00771A90 (??1CTCOpinionSource@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCOpinionSource@@QAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCOpinionSource::CTCOpinionSource(CThing&) { }
