// CAnimComponentFlags::Initialise(CPersistContext&, const CCharString&) -- reset, then OR the flags parsed from the text into Flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC3C0, length 130 bytes (virtual, `ret 8`).
// Callees: this's virtual slot 8 (+0x20, the component reset), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CStringParser::CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&,
// long) @ 0x00404720 (landed), CCharString::~CCharString @ 0x0099EAE0, TransferIn(CPersistContext*,
// CStringParser*, long*) @ 0x009E2590 (__fastcall), std::list<CStringParserCommentBoundaries>::~list
// @ 0x004046B0 (landed), CBase::~CBase @ 0x0099A2E0.
//
// Shape: after the virtual reset, a CStringParser local (0x28 bytes, the whole frame above the temp string's
// word) is built over the text with no error callback, an explicit `CCharString("", -1)` file-name
// temporary (its constructor's returned pointer pushed straight from eax; the trailing 0 pushed before it is
// evaluated, right to left) and line offset 0; the temporary dies right after the declaration. The parsed
// value is the local of an INLINE helper returning it, which VC homes in the dead text-argument slot
// (`lea edx,[esp+38h]`); it is OR-ed into Flags (+0x08). A plain `long` local in the function body takes a
// frame word of its own (0x30 frame), an inlined by-value parameter adds a self-copy. The parser's implicit destructor is inlined (comment list, TempCharString,
// Filename, then the out-of-line CBase destructor: no vtable store of its own). The engine header
// CAnimComponentFlags.h is data-only, so the polymorphic class is modelled locally (Flags +0x08, PDB name).
// Leaf: ?Initialise@CAnimComponentFlags

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CBase {
    virtual void v00();
    ~CBase();                                   // 0x0099A2E0
};

struct CStringParserCommentList {               // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                // 0x004046B0
};

struct CStringParser : CBase {                  // retail 0x28 bytes
    char* String;                               // +0x04
    unsigned long BaseLineOffset;               // +0x08
    CCharString Filename;                       // +0x0c
    CCharString TempCharString;                 // +0x10
    CStringParserCommentList Comments;          // +0x14
    void (*ErrorCallback)(CCharString*);        // +0x18
    long CursorPos[3];                          // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);         // 0x00404720
};

struct CPersistContext;

void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590

// The parse step as an inline helper: its local is homed in the caller's dead argument slot.
inline long ParseLong(CPersistContext& context, CStringParser& parser)
{
    long value;
    TransferIn(&context, &parser, &value);
    return value;
}

struct CAnimComponentBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Reset();                       // slot 8 (+0x20)
    long _base04;                               // +0x04
};

struct CAnimComponentFlags : CAnimComponentBase {
    long Flags;                                 // +0x08
    virtual bool Initialise(CPersistContext& context, const CCharString& text);
};

bool CAnimComponentFlags::Initialise(CPersistContext& context, const CCharString& text)
{
    Reset();
    CStringParser parser(text, 0, CCharString("", -1), 0);
    Flags |= ParseLong(context, parser);
    return true;
}