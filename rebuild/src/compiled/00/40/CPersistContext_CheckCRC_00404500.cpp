// CPersistContext::CheckCRC(const char* name) -- binary persist key guard: on binary load skip the 4-byte key CRC, on binary save write the CRC of the key
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404500, 181 bytes (+ int3 padding), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CCharString::CCharString(const char*, long) 0x0099EBF0,
//   Calc (CRC32) 0x004014A0 (`__fastcall` seed, data, length), CDataOutputStream::WriteULong 0x00993EA0 (catalogued
//   "WriteCBYTE"), CCharString::~CCharString 0x0099EAE0.
// Shape: every CPersistContext::Transfer<T> calls this first. Mode 2 (binary load) reads the stored key CRC through the
//   inlined by-value ReadULong() over CMemoryDataInputStream::Read (64-bit position range check, chunk fast path,
//   ReadWithSrcChunkOverflow fallback) and drops the result -- the comparison is debug-only -- so the fast path only
//   advances the stream (an address-taken local read in the body would keep a load/store) and the buffer lives in the dead
//   `name` argument slot. Mode 3 (binary save) CRCs a CCharString temporary of the key (also in the dead `name` slot)
//   through an inline GetCRC(const CCharString&): the ctor's returned `this` is reused and the text block is null-tested
//   once per accessor (length first: VC pushes arguments right to left); a named string local merges the two tests.
//   Layout from Ego_r.pdb: CPersistContext Mode +0x18, LoadStream +0x24, SaveStream +0x28.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharStringData { public:
    const char*   Text;                    // +0
    unsigned long Length;                  // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Text; }
};
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0 CRC
inline unsigned long GetCRC(const CCharString& text) { return Calc(0, text.GetPString(), text.GetLength()); }

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
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
    __forceinline unsigned long ReadULong()
    {
        unsigned long value;
        Read(&value, sizeof(value));
        return value;
    }
};
class CDataOutputStream { public:
    void WriteULong(unsigned long value);  // 0x00993EA0
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    Mode;                 // +0x18 (2 binary load, 3 binary save)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* LoadStream;           // +0x24
    CDataOutputStream*      SaveStream;           // +0x28
    protected: void CheckCRC(const char* name); public:
};

void CPersistContext::CheckCRC(const char* name)
{
    if (Mode == 2) {
        LoadStream->ReadULong();
    }
    else if (Mode == 3) {
        SaveStream->WriteULong(GetCRC(CCharString(name)));
    }
}