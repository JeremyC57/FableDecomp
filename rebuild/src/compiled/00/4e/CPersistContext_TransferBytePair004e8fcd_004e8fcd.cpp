#pragma optimize("s", on)
// CPersistContext::TransferBytePair_004e8fcd(SBytePair*) -- CheckCRC(""), then in mode 3 (save) write the pair's two bytes to the save stream, in mode 2 (load) read them through a two-long reader local initialised to -1/-1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E8FCD, length 84 bytes.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key @ 0x0122D70E), CDataOutputStream::WriteSLONG @ 0x00993EB0 twice (the save stream +0x28 kept in esi),
// the reader's Read @ 0x004E9021 (`this` = the local, the load stream +0x24 and the value on the stack). Shape: the reader local's two -1 words are written
// (`or [ebp-8],-1 ; or [ebp-4],-1`) before the mode ladder (`dec ; dec ; je ; dec ; jne`: cases 2 and 3); edi is saved only around the save arm. The chunk label
// "CEngineLightingManager::UpdateShadowScene" is propagated from an unrelated body.
struct SBytePair { long m_first; long m_second; };     // the two persisted bytes, each held in a long
class CDataOutputStream { public: public:
    void WriteSLONG(long);   // 0x00993EB0
};   // 0x00993EB0
class CDataInputStream;

struct CPairReader_004e9021 {                      // a two-word reader: both words start at -1
    long m_a;
    long m_b;
    CPairReader_004e9021() : m_a(-1), m_b(-1) { }
    void Read(CDataInputStream* stream, SBytePair* value);   // 0x004E9021
};

class CPersistContext { public:
    unsigned char      _pad0[0x18];
    long               m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char      _pad1[8];
    CDataInputStream*  m_pLoadStream;              // +0x24
    CDataOutputStream* m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);               // 0x00404500
    void TransferBytePair_004e8fcd(SBytePair* value);
};

void CPersistContext::TransferBytePair_004e8fcd(SBytePair* value)
{
    CheckCRC("");
    CPairReader_004e9021 reader;
    switch (m_mode) {
    case 3: {
        CDataOutputStream* stream = m_pSaveStream;
        stream->WriteSLONG(value->m_first);
        stream->WriteSLONG(value->m_second);
        break;
    }
    case 2:
        reader.Read(m_pLoadStream, value);
        break;
    }
}