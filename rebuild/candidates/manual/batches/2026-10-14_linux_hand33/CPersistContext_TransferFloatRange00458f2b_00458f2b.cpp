#pragma optimize("s", on)
// CPersistContext::TransferFloatRange_00458f2b(CFloatRange* value) -- CheckCRC(""), then in mode 3 (save) write the range's two floats (max first, then min) to the save stream, in mode 2 (load) read them through a zeroed two-float reader local
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458F2B, length 94 bytes.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key @ 0x0122D70E), CDataOutputStream::WriteFloat @ 0x00993EE0 twice (the save stream +0x28 kept in esi;
// each float pushed through the x87), the reader's TransferBinaryIn @ 0x00458F89 (`this` = the local, the load stream +0x24 and the value on the stack).
// Shape: as CPersistContext::TransferBytePair_004e8fcd @ 0x004E8FCD (landed): the reader local's two zero floats are written (`fldz ; fstp`) before the mode
// ladder (`dec ; dec ; je ; dec ; jne`: cases 2 and 3); edi is saved only around the save arm. The chunk label "CEngineLightingManager::UpdateShadowScene" is
// propagated from an unrelated body (the same label sits on 0x004E8FCD).
struct CFloatRange { float m_min; float m_max; };
struct CDataOutputStream { void WriteFloat(float value); };   // 0x00993EE0
struct CDataInputStream;

struct CFloatRangeReader_00458f89 {               // a two-float reader: both start at zero
    float m_min;
    float m_max;
    CFloatRangeReader_00458f89() : m_min(0.0f), m_max(0.0f) { }
    void TransferBinaryIn(CDataInputStream* stream, CFloatRange* value);   // 0x00458F89
};

struct CPersistContext {
    unsigned char      _pad0[0x18];
    long               m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char      _pad1[8];
    CDataInputStream*  m_pLoadStream;              // +0x24
    CDataOutputStream* m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);               // 0x00404500
    void TransferFloatRange_00458f2b(CFloatRange* value);
};

void CPersistContext::TransferFloatRange_00458f2b(CFloatRange* value)
{
    CheckCRC("");
    CFloatRangeReader_00458f89 reader;
    switch (m_mode) {
    case 3: {
        CDataOutputStream* stream = m_pSaveStream;
        stream->WriteFloat(value->m_max);
        stream->WriteFloat(value->m_min);
        break;
    }
    case 2:
        reader.TransferBinaryIn(m_pLoadStream, value);
        break;
    }
}
