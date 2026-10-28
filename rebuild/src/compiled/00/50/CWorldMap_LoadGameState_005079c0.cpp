// CWorldMap::LoadGameState(CPersistContext& context) -- restore the region game state: binary modes read the region count, resize Regions and transfer each region (from the second) by name; text modes parse REGION "name" ... END_REGION blocks, each known region loading its block through a nested text context (unknown regions are skipped)
// Leaf: ?LoadGameState@CWorldMap@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005079C0, 612 bytes (+ int3 padding), `ret 4`. PDB ?LoadGameState@CWorldMap@@QAEXAAVCPersistContext@@@Z
//   (the counterpart of the landed CWorldMap::SaveGameState 0x004FDE80).
// Callees: CPersistContext::CheckCRC 0x00404500, CDataOutputStream::WriteSLONG 0x00993EB0, CPersistTraits<unsigned
//   long>::TransferBinaryIn 0x0040F8A0, CRegion() 0x006BC410 / ~CRegion 0x0051A110, vector<CRegion>::resize(n, x)
//   0x0051D350, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / operator= 0x0099EFB0 / ~ 0x0099EAE0,
//   CPersistContext::Transfer(CCharString&) 0x00431143, CRegion::TransferGameState 0x006BC290, CStringParser
//   SkipWhitespace 0x009B9C60 / ReadAsStringUntilWhitespace 0x009BA4A0 / ReadNextItemAsQuotedString 0x009BA1D0 /
//   the string-terminated read 0x009BA3E0 / the skip 0x009B9BA0, CWorldMap::GetRegionNumberFromName 0x004FC210,
//   CStringParser(const CCharString&, callback, const CCharString&, long) 0x00404720 (implicit inline destructor: the
//   comment list's clear + node free, two ~CCharString, ~CBaseClass 0x0099A2E0), CPersistContext(CStringParser*, ...)
//   0x009BADD0, free 0x00BFEA14. Literals "" 0x0122D70E, "END_REGION" 0x01244B68.
// Layout: Regions vector<CRegion> at +0x2C (0x58-byte CRegion, Name +0x18) as the landed SaveGameState; CPersistContext
//   PParser +0x08, Mode +0x18, PLoadStream +0x24, PSaveStream +0x28 (landed ctor 0x009BADD0).
// Shape: the count stays in ebp only with the same-TU noinline stand-in for TransferBinaryIn (an out-of-line
//   declaration homes it in memory for the whole function); the two "END_REGION" reads go through inline const char*
//   helpers with a named CCharString (lea-addressed, destroyed at the helper's end); the comment list's destructor is
//   inlined (STLport clear + node free).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CBaseClass {
public:
    virtual void v00();
    virtual ~CBaseClass();                                      // 0x0099A2E0
};
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <class T> class list {
public:
    _List_node<T>* _M_node;
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
}
class CStringParser : public CBaseClass {            // retail 0x28 bytes
public:
    char*                    String;                    // +0x04
    unsigned long            BaseLineOffset;            // +0x08
    CCharString              Filename;                  // +0x0c
    CCharString              TempCharString;            // +0x10
    std::list<long>         Comments;                  // +0x14
    void (*ErrorCallback)(CCharString*);                // +0x18
    long                     CursorPos[3];              // +0x1c (Pos +0x20)
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);                 // 0x00404720
    bool SkipPastString(const CCharString& text);      // 0x009B9BA0 (label NextItemIs)
    long SkipWhitespace();                              // 0x009B9C60
    void ReadNextItemAsQuotedString(CCharString& text);   // 0x009BA1D0
    CCharString ReadAsStringUntilPastString(const CCharString& end);   // 0x009BA3E0
    CCharString ReadAsStringUntilWhitespace();         // 0x009BA4A0
    bool IsAtEnd() const { return String[CursorPos[1]] == 0; }
    bool SkipPastText(const char* text)
    {
        CCharString item(text);
        return SkipPastString(item);
    }
    CCharString ReadUntilPastText(const char* text)
    {
        CCharString item(text);
        return ReadAsStringUntilPastString(item);
    }
};

class CDataOutputStream {
public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};
class CMemoryDataInputStream;
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);   // 0x0040F8A0
};
extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
// Same-TU stand-in for 0x0040F8A0 (as the landed SaveGameState): with the body visible VC7.1 keeps the count in a
// register and only homes it around this call.
template <> void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream* stream, unsigned long* value)
{
    *value = (unsigned long)StandInSink((int)stream, (int)DefaultVal);
}
#pragma auto_inline(on)
class CDefinitionManager;
class CSymbolMap;
class CPersistContext {
public:
    CDefinitionManager*     PDefinitionManager;         // +0x00
    CSymbolMap*             PSymbolMap;                 // +0x04
    CStringParser*          PParser;                    // +0x08
    void (*ErrorCallback)(CCharString*);                // +0x0c
    CCharString*            PStringToClean;             // +0x10
    bool                    UseDefaults;                // +0x14
    bool                    UseCRCCheck;                // +0x15
    bool                    SingleTransfer;             // +0x16
    bool                    Finished;                   // +0x17
    long                    Mode;                       // +0x18
    bool                    Binary;                     // +0x1c
    CCharString*            PStringToSaveTo;            // +0x20
    CMemoryDataInputStream* PLoadStream;                // +0x24
    CDataOutputStream*      PSaveStream;                // +0x28
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool useDefaults,
                    CDefinitionManager* definitionManager, CSymbolMap* symbolMap, CCharString* stringToClean);   // 0x009BADD0
    void CheckCRC(const char* name);                    // 0x00404500
    template <class T> void Transfer(T& value);                  // 0x00431143
    bool IsBinary() const { return Mode == 2 || Mode == 3; }
    __forceinline void TransferBinary(unsigned long& value)
    {
        CheckCRC("");
        CPersistTraits<unsigned long> traits;
        switch (Mode) {
        case 3:
            PSaveStream->WriteSLONG(value);
            break;
        case 2:
            traits.TransferBinaryIn(PLoadStream, &value);
            break;
        }
    }
};

class CRegion {                                         // 0x58 bytes
public:
    unsigned char _pad00[0x18];
    CCharString   Name;                                  // +0x18
    unsigned char _rest[0x58 - 0x1c];
    CRegion();                                           // 0x006BC410
    ~CRegion();                                          // 0x0051A110
    void TransferGameState(CPersistContext& context);    // 0x006BC290
};
template <class T> struct CRetailVector {
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    void resize(unsigned long n, const T& x);   // 0x0051D350
    T* begin() { return Start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
};

class CWorldMap {
public:
    unsigned char          _pad[0x2c];
    CRetailVector<CRegion> Regions;             // +0x2C
    long GetRegionNumberFromName(const CCharString& name) const;   // 0x004FC210
    void LoadGameState(CPersistContext& context);
};

void CWorldMap::LoadGameState(CPersistContext& context)
{
    if (context.IsBinary()) {
        unsigned long count = 0;
        context.TransferBinary(count);
        Regions.resize(count, CRegion());
        for (unsigned long i = 1; i < count; ++i) {
            CCharString name;
            context.Transfer(name);
            Regions[i].Name = name;
            Regions[i].TransferGameState(context);
        }
        return;
    }
    CStringParser* parser = context.PParser;
    parser->SkipWhitespace();
    while (!parser->IsAtEnd()) {
        CCharString keyword = parser->ReadAsStringUntilWhitespace();
        parser->SkipWhitespace();
        CCharString name;
        parser->ReadNextItemAsQuotedString(name);
        long index = GetRegionNumberFromName(name);
        if (index > 0) {
            CCharString body = parser->ReadUntilPastText("END_REGION");
            CStringParser regionParser(body, 0, CCharString(""), 0);
            CPersistContext regionContext(&regionParser, 0, true, 0, 0, 0);
            Regions[index].TransferGameState(regionContext);
        }
        else
            parser->SkipPastText("END_REGION");
        parser->SkipWhitespace();
    }
}