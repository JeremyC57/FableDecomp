// CStreamingFontDataBank::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCStreamingFontDataBank@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5FA30, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x0129D5CC, calls ??1CStreamingFontDataBank 0x00A5F3A0; Ego_r ??_GCStreamingFontDataBank@@UAEPAXI@Z.
// Callees: CStreamingFontDataBank::~CStreamingFontDataBank 0x00A5F3A0 (??1CStreamingFontDataBank@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CStreamingFontDataBank::ScalarDeletingDestructor (member stand-in)
//   calling CStreamingFontDataBank::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CStreamingFontDataBank {
public:
    CStreamingFontDataBank();   // 0x00A5F290
    virtual ~CStreamingFontDataBank();                  // 0x00A5F3A0 (??1CStreamingFontDataBank@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CStreamingFontDataBank@@QAE@XZ 0x00A5F290) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CStreamingFontDataBank::CStreamingFontDataBank() { }
