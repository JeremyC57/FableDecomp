// CDefStringTable::Unlock() -- leave the locked state: unless read-only, rebuild the name -> offset map from the string blob (each record: a 4-byte header, the characters and their terminator) and clear Locked
// Leaf: Unlock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4F10, length 119 bytes.
// Callees: CMemoryDataOutputStream::PeekData @ 0x00995E70 and ::GetSize @ 0x00994B40 on the stream at +0xc (the manifest's
// "CCreatureActionBase::GetFramesRemaining" for 0x00994B40 is propagated), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, map<CCharString, long>::operator[] @ 0x00519230 (manifest "map<CCharString, C2DVector>" is propagated across
// identical bodies), CCharString::~CCharString @ 0x0099EAE0.
// Layout: the landed WriteTable (0x009D4A50) retail view -- StringMap +0x00 (12-byte STLport map), StringStream +0x0c
// (24 bytes), counts +0x24.., TableFilename +0x30, Locked +0x34, ReadOnly +0x35 (Ego_r.pdb +0x3c/+0x3d).
// Shape: the two flag tests are early-outs; the stream sub-object is addressed once (`lea esi,[edi+0Ch]`); the blob base
// and size are fetched before the rotated `offset < size` loop (unsigned); each record's name is a scoped CCharString built
// at blob + offset + 4, its length read through the null-case-first accessor for the `+ 5` step.
class CCharStringData { public:
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
class CCharString { public:
    CCharStringData* m_pStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
};

struct CStringOffsetMap {                           // map<CCharString, long> (STLport, 12 bytes)
    unsigned char _tree[0xc];
    long& operator[](const CCharString& key);       // 0x00519230
};

class CMemoryDataOutputStream { public:
    unsigned long GetSize();                        // 0x00994B40
    void* PeekData();                               // 0x00995E70
};

struct CDefStringTable {
    CStringOffsetMap        StringMap;              // +0x00
    CMemoryDataOutputStream StringStream;           // +0x0c (24 bytes)
    unsigned char           _stream[0x18 - sizeof(CMemoryDataOutputStream)];
    unsigned long           StringCount;            // +0x24
    unsigned long           SharedCount;            // +0x28
    unsigned long           RandomID;               // +0x2c
    void*                   TableFilename;          // +0x30 CWideString
    bool                    Locked;                 // +0x34
    bool                    ReadOnly;               // +0x35
    void Unlock();
};

void CDefStringTable::Unlock()
{
    if (!Locked)
        return;
    if (ReadOnly)
        return;
    CMemoryDataOutputStream* stream = &StringStream;
    const char* data = (const char*)stream->PeekData();
    unsigned long size = stream->GetSize();
    for (unsigned long offset = 4; offset < size; ) {
        CCharString name(data + offset + 4, -1);
        StringMap[name] = offset;
        offset += name.GetLength() + 5;
    }
    Locked = false;
}