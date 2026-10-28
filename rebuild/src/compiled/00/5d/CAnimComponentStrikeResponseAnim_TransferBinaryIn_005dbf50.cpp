// CAnimComponentStrikeResponseAnim::TransferBinaryIn(CDataInputStream& in) -- binary persist read of the response-animation name: a 4-byte length, then (when non-zero) the string itself
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DBF50, 128 bytes, __fastcall, ret 4.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CDataInputStream::ReadString 0x00996BE0
// (returns a CCharString by hidden pointer), CCharString::operator= 0x0099EFB0, CCharString::~CCharString 0x0099EAE0.
// Shape: the length is read by the inlined CDataInputStream::Read(buffer, size) (the same 64-bit
// position range check as the landed TransferBinaryIn family, e.g. 0x007AD7F0: `position + size`
// widened to __int64 and rejected outside [0, 0x7FFFFFFF]; a memcpy from the chunk pointer when the
// chunk holds enough bytes, else the overflow reader). The read goes through the inlined
// CPersistTraits<unsigned long>::TransferBinaryIn: its own `value` local is homed in the dead `in`
// argument slot ([esp+10h] after the three pushes; the stream pointer lives in esi) and copied into
// the outer `length`, which therefore stays in edi (the fast path never stores the slot, the slow and
// rejected paths reload it). With `value` dead, the ReadString result temporary reuses the same slot.
// Reading straight into `length` keeps it in the slot and gives the temporary its own `push ecx` word.
// A zero length (`jbe`, so unsigned) leaves the name untouched; on a rejected position the
// (uninitialised) slot value is used, like retail.
// engine/CAnimComponentStrikeResponseAnim.h keeps the name as a raw CCharStringData* at +8; the class
// is modelled locally so the member can be a real CCharString with its out-of-line operator=.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharStringData;

class CCharString {
public:
    CCharStringData* m_pData;
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CDataInputStream {
public:
    void* m_pVtable;                       // +0x00
    unsigned long m_position;              // +0x04
    long m_sourceLength;                   // +0x08
    const unsigned char* m_pData;          // +0x0C current chunk read pointer
    long m_chunkPosition;                  // +0x10
    long m_remaining;                      // +0x14 bytes left in the current chunk
    CCharString ReadString(unsigned long length);                       // 0x00996BE0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

class CAnimComponentStrikeResponseAnim {
public:
    virtual void v00();
    long IVRefCount;                             // +0x04 (base component data)
    CCharString ResponseAnimName;          // +0x08
    void TransferBinaryIn(CDataInputStream& in);
};

// CPersistTraits<unsigned long>::TransferBinaryIn -- the 4-byte read into its own local, then copied out
static __forceinline void TransferLength(CDataInputStream& in, unsigned long* out)
{
    unsigned long value;
    in.Read(&value, sizeof(value));
    *out = value;
}

void CAnimComponentStrikeResponseAnim::TransferBinaryIn(CDataInputStream& in)
{
    unsigned long length;
    TransferLength(in, &length);
    if (length > 0)
        ResponseAnimName = in.ReadString(length);
}