// CPersistContext::ReadSection(CPersistContextReadSection& section, const CCharString& name) -- open a named section for reading: in binary-load mode check the stored section name, read its byte size and point the section's own memory stream at that slice of the load stream (skipping it); in text-load mode find "START_<name>", take the text up to "END_<name>" and parse it with the section's own string parser (an empty text and false when the start marker is missing); the section context inherits this context's settings
// Leaf: ReadSection
// Grade: RELOCATION_MATCH -- the parser Init gets an explicit `CCharString("")` filename argument (instead of the inline
//   wrapper's default argument): VC7.1 then builds the temporary in the dead name slot and passes the constructor-
//   returned pointer, as retail
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BAE70, 591 bytes (+ int3 padding), `ret 8`.
// Callees: the load stream's virtual ReadNullTerminatedString (slot 6) and GetPosition (slot 2), the section stream's
//   virtual slot 3, CCharString::operator!=(const CCharString&) 0x0099E900, CDataInputStream::ReadWithSrcChunkOverflow
//   0x00993CA0, CMemoryDataInputStream::SetPositionAssumeWithinSourceChunk 0x00993C10 / OpenMemorySource 0x00994570,
//   operator+(const char*, const CCharString&) 0x0099F690 (__fastcall, hidden result in ecx), CStringParser::FindKeyword
//   0x009B9BA0 / SkipPastNewline 0x009B9D70 / ReadAsStringUntilString 0x009BA330 / Init 0x009B9530, CCharString()
//   0x0099E4B0 / (const char*, long) 0x0099EBF0 / operator= 0x0099EFB0 / dtor 0x0099EAE0.
// Layout: CPersistContext and CPersistContextReadSection per Ego_r.pdb with the retail 0x28-byte CStringParser (STLport
//   4-byte comment list; landed 0x00463334-family models): InputStream +0x2c, Parser +0x48, Text +0x70.
// Literals: "START_" 0x0129B31C, "END_" 0x0129B324, "" 0x0122D70E; the null-string text 0x0129AAF4.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    bool operator!=(const CCharString& other) const;   // 0x0099E900
    char* GetData() const
    {
        char* data = "";
        if (PStringData)
            data = PStringData->Data.PData;
        return data;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CBase { virtual ~CBase(); };
typedef void (__cdecl* TErrorCallback)(CCharString*);
struct CCursorPos {
    bool InQuotes;
    long StringPos;
    long Line;
};
class CStringParser : public CBase { public:                         // retail 0x28 bytes
    char*          String;                             // +0x04
    unsigned long  BaseLineOffset;                     // +0x08
    CCharString    Filename;                           // +0x0c
    CCharString    TempCharString;                     // +0x10
    void*          Comments;                           // +0x14 STLport list head
    TErrorCallback ErrorCallback;                      // +0x18
    CCursorPos     CursorPos;                          // +0x1c
    void Init(char* text, TErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530
    bool FindKeyword(const CCharString& keyword);      // 0x009B9BA0
    long SkipPastNewline();                            // 0x009B9D70
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
    __forceinline void Init(const CCharString& text, const CCharString& filename = "")
    {
        TempCharString = text;
        Init(text.GetData(), 0, filename, 0);
    }
};
class CMemoryDataInputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    virtual void Close();                              // slot 3 (+0x0c)
    virtual void v04();
    virtual void v05();
    virtual CCharString ReadNullTerminatedString();    // slot 6 (+0x18)
    unsigned long        StreamPos;                    // +0x04
    long                 StreamSize;                   // +0x08
    const unsigned char* CurrentSourcePtr;             // +0x0c
    unsigned long        SourceChunkStreamPos;         // +0x10
    long                 SourceChunkBytesRemaining;    // +0x14
    const unsigned char* StreamSource;                 // +0x18
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    void SetPositionAssumeWithinSourceChunk(unsigned long position);  // 0x00993C10
    void OpenMemorySource(void* data, unsigned long size);            // 0x00994570
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long value; Read(&value, 4); return value; }
    __forceinline const unsigned char* GetDataAndSkip(long bytes)
    {
        unsigned long position = GetPosition();
        __int64 newPosition = (__int64)(position + bytes);
        if (IsValidPosition(newPosition)) {
            const unsigned char* data = StreamSource + position;
            SetPositionAssumeWithinSourceChunk((unsigned long)newPosition);
            return data;
        }
        return 0;
    }
};
class CDefinitionManager;
class CSymbolMap;
class CDataOutputStream;
class CPersistContextReadSection;
class CPersistContext { public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY };
    CDefinitionManager*     PDefinitionManager;        // +0x00
    CSymbolMap*             PSymbolMap;                // +0x04
    CStringParser*          PParser;                   // +0x08
    TErrorCallback          ErrorCallback;             // +0x0c
    CCharString*            PStringToClean;            // +0x10
    bool                    UseDefaults;               // +0x14
    bool                    UseCRCCheck;               // +0x15
    bool                    SingleTransfer;            // +0x16
    bool                    Finished;                  // +0x17
    EMode                   Mode;                      // +0x18
    bool                    Binary;                    // +0x1c
    CCharString*            PStringToSaveTo;           // +0x20
    CMemoryDataInputStream* PLoadStream;               // +0x24
    CDataOutputStream*      PSaveStream;               // +0x28
    bool ReadSection(CPersistContextReadSection& section, const CCharString& name);
    __forceinline void InitBinaryLoad(CMemoryDataInputStream* stream)
    {
        Mode = MODE_LOAD_BINARY;
        PLoadStream = stream;
        PSaveStream = 0;
        PDefinitionManager = 0;
        UseCRCCheck = false;
    }
    __forceinline void InitTextLoad(CStringParser* parser, TErrorCallback errorCallback, bool useDefaults,
                                    CDefinitionManager* definitionManager, CSymbolMap* symbolMap, CCharString* stringToClean)
    {                                                  // the inline body of the text-load constructor 0x009BADD0
        Mode = MODE_LOAD_TEXT;
        PDefinitionManager = definitionManager;
        PSymbolMap = symbolMap;
        PParser = parser;
        ErrorCallback = errorCallback;
        PStringToClean = stringToClean;
        UseDefaults = useDefaults;
        UseCRCCheck = false;
        Finished = false;
        SingleTransfer = false;
    }
};
class CPersistContextReadSection { public:                    // retail 0x74 bytes
    CPersistContext        Context;                    // +0x00
    CMemoryDataInputStream InputStream;                // +0x2c
    CStringParser          Parser;                     // +0x48
    CCharString            Text;                       // +0x70
};

bool CPersistContext::ReadSection(CPersistContextReadSection& section, const CCharString& name)
{
    bool result = true;
    if (Mode == MODE_LOAD_BINARY) {
        CCharString sectionName = PLoadStream->ReadNullTerminatedString();
        if (sectionName != name)
            return false;
        long size = PLoadStream->ReadLong();
        section.InputStream.Close();
        CMemoryDataInputStream* stream = PLoadStream;
        section.InputStream.OpenMemorySource(size > 0 ? (void*)stream->GetDataAndSkip(size) : 0, size);
        section.Context.InitBinaryLoad(&section.InputStream);
    } else if (Mode == MODE_LOAD_TEXT) {
        if (PParser->FindKeyword("START_" + name)) {
            PParser->SkipPastNewline();
            section.Text = PParser->ReadAsStringUntilString("END_" + name);
        } else {
            section.Text = CCharString();
            result = false;
        }
        section.Parser.Init(section.Text, CCharString(""));
        section.Context.InitTextLoad(&section.Parser, ErrorCallback, UseDefaults, PDefinitionManager, PSymbolMap, PStringToClean);
    }
    return result;
}