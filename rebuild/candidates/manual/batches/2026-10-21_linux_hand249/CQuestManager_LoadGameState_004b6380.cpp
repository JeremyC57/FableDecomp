// CQuestManager::LoadGameState(CPersistContext&) -- read the quest game-state text (length-prefixed in binary contexts, else from the context's parser), load it, then transfer the boasts
// Leaf: ?LoadGameState@CQuestManager@@QAEXAAUCPersistContext
// Retail 0x004B6380, 346 bytes, ret 4 (the reader of the landed SaveGameState 0x004B64E0). Callees: the stream's inline
// ReadLong (slow path CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0), vector<unsigned char>(n) 0x00411910, the
// stream's virtual ReadBuffer (slot 4, +0x10), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
// CStringParser ctor 0x00404720 and its implicit inline dtor (~list 0x004046B0, CBaseClass::~CBaseClass 0x0099A2E0),
// free 0x00BFEA14, CQuestManager::LoadGameState(CStringParser&) 0x004B5500, TransferBoasts 0x004B05C0.
// Layout: CPersistContext per Ego_r.pdb (PParser +8, Mode +0x18, PLoadStream +0x24); the load stream is
// engine/CDataInputStream.h's layout, modelled locally because the body makes a virtual call through its vptr (the
// header carries the vptr as a plain data word).
// Shape: the mode test is a named bool (`Mode == 2 || Mode == 3`, as in the landed SaveGameState) so the binary arm
// comes first; the count read is the landed ReadPresizedString 0x00996390 inline ReadLong (value homed in the dead
// context-argument slot); the byte array's GetData idiom; the parser's "" filename an unnamed temporary.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);            // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};
struct CByteArray {                                        // retail vector<unsigned char>: begin/end/capacity
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                           // 0x00411910
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
    unsigned char& operator[](long index) { return m_begin[index]; }
};
struct CMemoryDataInputStream {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void ReadBuffer(void* buffer, long size);      // slot 4 (+0x10)
    unsigned long StreamPos;                               // +0x04
    long          StreamSize;                              // +0x08
    void*         CurrentSourcePtr;                        // +0x0c
    unsigned long SourceChunkStreamPos;                    // +0x10
    long          SourceChunkBytesRemaining;               // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, 4);
        return value;
    }
};
struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
struct CStringParser : CBaseClass {                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
};
struct CPersistContext {                           // Ego_r.pdb, 0x2c bytes
    void*                   PDefinitionManager;    // +0x00
    void*                   PSymbolMap;            // +0x04
    CStringParser*          PParser;               // +0x08
    void*                   ErrorCallback;         // +0x0c
    CCharString*            PStringToClean;        // +0x10
    bool                    UseDefaults, UseCRCCheck, SingleTransfer, Finished;   // +0x14
    long                    Mode;                  // +0x18
    bool                    Binary;                // +0x1c
    CCharString*            PStringToSaveTo;       // +0x20
    CMemoryDataInputStream* PLoadStream;           // +0x24
    void*                   PSaveStream;           // +0x28
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ""
struct CQuestManager {
    void LoadGameState(CStringParser& parser);     // 0x004B5500
    void TransferBoasts(CPersistContext& context); // 0x004B05C0
    void LoadGameState(CPersistContext& context);
};

void CQuestManager::LoadGameState(CPersistContext& context)
{
    bool binary = context.Mode == 2 || context.Mode == 3;
    if (binary) {
        CMemoryDataInputStream* stream = context.PLoadStream;
        long length = stream->ReadLong();
        CByteArray buffer(length + 1);
        stream->ReadBuffer(buffer.GetData(), length);
        buffer[length] = 0;
        CCharString text((const char*)buffer.GetData(), -1);
        CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
        LoadGameState(parser);
    } else
        LoadGameState(*context.PParser);
    TransferBoasts(context);
}
