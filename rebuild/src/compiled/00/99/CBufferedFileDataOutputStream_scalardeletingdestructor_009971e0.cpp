// CBufferedFileDataOutputStream::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCBufferedFileDataOutputStream@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009971E0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x0129A710; calls 0x009947A0.
// Callees: CBufferedFileDataOutputStream::~CBufferedFileDataOutputStream 0x009947A0 (??1CBufferedFileDataOutputStream@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CBufferedFileDataOutputStream::ScalarDeletingDestructor (member stand-in)
//   calling CBufferedFileDataOutputStream::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CAFile;

class CBufferedFileDataOutputStream {
public:
    CBufferedFileDataOutputStream(CAFile&, long);   // ??0CBufferedFileDataOutputStream@@QAE@AAVCAFile@@J@Z
    virtual ~CBufferedFileDataOutputStream();           // 0x009947A0 (??1CBufferedFileDataOutputStream@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CBufferedFileDataOutputStream@@QAE@AAVCAFile@@J@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CBufferedFileDataOutputStream::CBufferedFileDataOutputStream(CAFile&, long) { }
