// CFontDataBankEntry::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCFontDataBankEntry@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A63970, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x0129DA70, calls ??1CFontDataBankEntry 0x00A63480; FableWin ??_GCFontDataBankEntry@@UAEPAXI@Z.
// Callees: CFontDataBankEntry::~CFontDataBankEntry 0x00A63480 (??1CFontDataBankEntry@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CFontDataBankEntry::ScalarDeletingDestructor (member stand-in)
//   calling CFontDataBankEntry::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CFontDataBank;

class CFontDataBankEntry {
public:
    CFontDataBankEntry(unsigned long, unsigned long, CFontDataBank*);   // ??0CFontDataBankEntry@@QAE@KKPAVCFontDataBank@@@Z
    virtual ~CFontDataBankEntry();                      // 0x00A63480 (??1CFontDataBankEntry@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CFontDataBankEntry@@QAE@KKPAVCFontDataBank@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CFontDataBankEntry::CFontDataBankEntry(unsigned long, unsigned long, CFontDataBank*) { }
