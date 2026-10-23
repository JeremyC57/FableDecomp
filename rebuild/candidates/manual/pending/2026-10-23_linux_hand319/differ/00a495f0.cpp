// Grade: DIFFER(47v1261) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CLipSyncSampleData::ReadFromFile(CDataInputStream* stream) -- read a lip-sync sample: the phoneme-name count and names
//   (byte length + text, inserted into a temporary map<long, CCharString>), the sample length (unsigned -> float), the
//   frame count (resizing the frame vector of 12-byte entries), then per frame a byte count of (phoneme byte, weight byte /
//   255 clamped to [0,1]) keys pushed into the frame's vector; finally clear the temporary map
// Leaf: ReadFromFile
// Stub: first-pass skeleton only (the map header and the first read); the reads, the resize and the per-frame loop are not
//   reconstructed yet. EgoCore's lipsync reader is the format answer key.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
struct CDataInputStream {
    virtual void v00();
    unsigned long        m_position;                    // +0x04
    long                 StreamSize;                    // +0x08
    const unsigned char* m_pData;                       // +0x0c
    long                 SourceChunkStreamPos;          // +0x10
    long                 m_remaining;                   // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
struct SMapNode { unsigned char Color; SMapNode* Parent; SMapNode* Left; SMapNode* Right; long Key; void* Value; };
struct CNameMap {                                       // STLport map<long, CCharString>: header node + count
    SMapNode* Header;
    long      Count;
    CNameMap() : Count(0)
    {
        Header = (SMapNode*)malloc(sizeof(SMapNode));
        Header->Color = 0;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
};
class CLipSyncSampleData {
public:
    void ReadFromFile(CDataInputStream* stream);
};
void CLipSyncSampleData::ReadFromFile(CDataInputStream* stream)
{
    CNameMap names;
    unsigned long count;
    stream->ReadWithSrcChunkOverflow((unsigned char*)&count, 4);
}
