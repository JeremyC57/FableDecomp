// CStreamingFontDataBankEntry::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCStreamingFontDataBankEntry@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F800, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x0129D5B4, calls ??1CStreamingFontDataBankEntry 0x00A5F150; Ego_r ??_GCStreamingFontDataBankEntry@@UAEPAXI@Z.
// Callees: CStreamingFontDataBankEntry::~CStreamingFontDataBankEntry 0x00A5F150 (??1CStreamingFontDataBankEntry@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CStreamingFontDataBankEntry::ScalarDeletingDestructor (member stand-in)
//   calling CStreamingFontDataBankEntry::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CStreamingFontDataBank;

class CStreamingFontDataBankEntry {
public:
    CStreamingFontDataBankEntry(unsigned long, unsigned long, CStreamingFontDataBank*);   // ??0CStreamingFontDataBankEntry@@QAE@KKPAVCStreamingFontDataBank@@@Z
    virtual ~CStreamingFontDataBankEntry();             // 0x00A5F150 (??1CStreamingFontDataBankEntry@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CStreamingFontDataBankEntry@@QAE@KKPAVCStreamingFontDataBank@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CStreamingFontDataBankEntry::CStreamingFontDataBankEntry(unsigned long, unsigned long, CStreamingFontDataBank*) { }
