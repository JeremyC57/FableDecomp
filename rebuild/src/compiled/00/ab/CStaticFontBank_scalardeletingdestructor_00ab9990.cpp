// CStaticFontBank::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCStaticFontBank@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB9990, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x0129EAD4 (RTTI CStaticFontBank); Ego_r.pdb name set; calls ~CStaticFontBank 0x00AB7A70.
// Callees: CStaticFontBank::~CStaticFontBank 0x00AB7A70 (??1CStaticFontBank@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CStaticFontBank::ScalarDeletingDestructor (member stand-in)
//   calling CStaticFontBank::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CFontDataBank;

class CStaticFontBank {
public:
    CStaticFontBank(CFontDataBank*, unsigned long);   // ??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z
    virtual ~CStaticFontBank();                         // 0x00AB7A70 (??1CStaticFontBank@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CStaticFontBank::CStaticFontBank(CFontDataBank*, unsigned long) { }
