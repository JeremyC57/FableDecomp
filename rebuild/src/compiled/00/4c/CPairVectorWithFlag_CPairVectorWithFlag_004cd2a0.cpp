// CPairVectorWithFlag::CPairVectorWithFlag() -- default constructor: empty 8-byte-element vector (start, finish, end_of_storage zeroed) and the flag byte at +0xD cleared
// Leaf: ??0CPairVectorWithFlag
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD2A0, 16 bytes. No callees. Unreferenced in the image (recovered
// from a mis-cut catalog row); its copy constructor is 0x004CD2B0 (vector copy 0x004CC290 + the +0xD byte).
// Shape: `mov eax,ecx ; xor ecx,ecx` then three dword zero stores and a byte store from the same zero
// register; the byte at +0xC is not initialised (nor copied by the copy constructor).
struct SPair8 { long first; long second; };
struct CPairVector {
    SPair8* m_start;                  // +0x00
    SPair8* m_finish;                 // +0x04
    SPair8* m_end_of_storage;         // +0x08
    CPairVector() : m_start(0), m_finish(0), m_end_of_storage(0) {}
};
struct CPairVectorWithFlag {
    CPairVector   m_vector;           // +0x00
    unsigned char m_unused0C;         // +0x0C (left uninitialised by both constructors)
    bool          m_flag;             // +0x0D
    CPairVectorWithFlag();
};

CPairVectorWithFlag::CPairVectorWithFlag()
    : m_flag(false)
{
}