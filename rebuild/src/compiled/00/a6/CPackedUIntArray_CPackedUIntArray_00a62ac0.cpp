// Retail 0x00a62ac0  CPackedUIntArray::~CPackedUIntArray  __fastcall, void
// Layout reconstructed from disasm:
//   [esi+0x0]  = heap buffer pointer (freed)
//   [esi+0x4]  = dword (count/capacity) -> 0
//   [esi+0x8]  = byte flag -> 0
//   [esi+0xc]  = dword -> 0

// cdecl free helper at 0xbfeb1c
void operator delete[](void*);

struct CPackedUIntArray
{
    void* buffer;   // +0x0
    int   Size;       // +0x4
    char  Bits;       // +0x8
    char  pad9;
    char  padA;
    char  padB;
    int   Bias;       // +0xc

    ~CPackedUIntArray();
};

CPackedUIntArray::~CPackedUIntArray()
{
    operator delete[](this->buffer);
    this->buffer = 0;
    this->Size = 0;
    this->Bits = 0;
    this->Bias = 0;
}