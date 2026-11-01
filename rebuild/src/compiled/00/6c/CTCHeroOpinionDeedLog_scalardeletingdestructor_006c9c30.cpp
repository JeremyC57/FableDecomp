// CTCHeroOpinionDeedLog::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCHeroOpinionDeedLog@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9C30, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c676): slot 0 of RTTI vtable 0x0125EA44 (.?AVCTCHeroOpinionDeedLog@@), calls ~CTCHeroOpinionDeedLog 0x006C5D70 then operator delete on bit 0; Ego_r ??_GCTCHeroOpinionDeedLog@@UAEPAXI@Z.
// Callees: CTCHeroOpinionDeedLog::~CTCHeroOpinionDeedLog 0x006C5D70 (??1CTCHeroOpinionDeedLog@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 17: landed as the CTCHeroOpinionDeedLog::ScalarDeletingDestructor (member stand-in)
//   calling CTCHeroOpinionDeedLog::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCHeroOpinionDeedLog {
public:
    CTCHeroOpinionDeedLog(CThing&);   // ??0CTCHeroOpinionDeedLog@@QAE@AAVCThing@@@Z
    virtual ~CTCHeroOpinionDeedLog();                   // 0x006C5D70 (??1CTCHeroOpinionDeedLog@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCHeroOpinionDeedLog@@QAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCHeroOpinionDeedLog::CTCHeroOpinionDeedLog(CThing&) { }
