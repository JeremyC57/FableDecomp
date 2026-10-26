struct CBase
{
    void** vtable;
    void RestoreVTable();
};

extern void* g_CChunkedFileChunkVTable_0129a69c[];

class CChunkedFileChunk : public CBase
{ public:
    ~CChunkedFileChunk();
};

CChunkedFileChunk::~CChunkedFileChunk()
{
    vtable = g_CChunkedFileChunkVTable_0129a69c;
    RestoreVTable();
}