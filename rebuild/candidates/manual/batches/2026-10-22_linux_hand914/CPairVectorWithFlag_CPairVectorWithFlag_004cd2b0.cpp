// CPairVectorWithFlag::CPairVectorWithFlag(const CPairVectorWithFlag&) -- copy constructor: copies the 8-byte-element vector and the flag byte at +0xD
// Leaf: ??0CPairVectorWithFlag
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD2B0, 27 bytes. Callees: the vector copy constructor 0x004CC290
// (Vector_CopyFrom: allocates (n*8) bytes and copies the pairs). Unreferenced in the image (recovered from
// a mis-cut catalog row); the default constructor is 0x004CD2A0.
// Shape: `push edi(other) ; mov esi,ecx ; call vector(const vector&)` then the byte copy and `mov eax,esi`;
// the +0xC byte is not copied, so the copy constructor is user-written.
struct SPair8 { long first; long second; };
struct CPairVector {
    SPair8* m_start;                  // +0x00
    SPair8* m_finish;                 // +0x04
    SPair8* m_end_of_storage;         // +0x08
    CPairVector(const CPairVector& other);   // 0x004CC290
};
struct CPairVectorWithFlag {
    CPairVector   m_vector;           // +0x00
    unsigned char m_unused0C;         // +0x0C (left uninitialised by both constructors)
    bool          m_flag;             // +0x0D
    CPairVectorWithFlag(const CPairVectorWithFlag& other);
};

CPairVectorWithFlag::CPairVectorWithFlag(const CPairVectorWithFlag& other)
    : m_vector(other.m_vector), m_flag(other.m_flag)
{
}
