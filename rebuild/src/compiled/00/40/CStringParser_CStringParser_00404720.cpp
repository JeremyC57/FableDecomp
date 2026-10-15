// CStringParser::CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename, long baseLineOffset) -- build a parser over a CCharString
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404720, 117 bytes. Callees: CBase ctor 0x0099A2D0, CCharString ctor 0x0099E4B0 (x2), malloc 0x00BFEA0E,
//   CCharString::operator=, CStringParser::Init(char*, ...).
//
// The string parser reads our text data files (defs, scripts, config) and tracks line numbers for
// error messages. This constructor takes the text as a CCharString: after the CBase sub-object and our
// own vtable are in place we default-construct the Filename and TempCharString members, create the
// self-linked sentinel node for the comment list, then keep a reference-counted copy of the text in
// TempCharString so the raw characters we parse from stay alive for the parser's lifetime. The raw
// pointer comes from that string's representation (an empty string yields the shared "" literal), and
// everything is handed to Init() which does the real work. See the Init overload at 0x00403C20.
//
// Matching: the vtable store is an immediate after the base constructor; the sentinel allocation is a
// literal 0x10 with two self-link stores; the null-rep check is written with explicit gotos so the
// "" literal path is the out-of-line arm and the data pointer load is the fall-through.

struct CCharString
{
    void* rep; // shared representation block; its first word is the char* data; null means empty

    CCharString& operator=(const CCharString& src);     // share the other string's block
};

typedef void (*StringParserErrorCB)(CCharString*);

extern "C" void __fastcall FableStringParserBaseConstruct_0099A2D0(void* self);   // CBase constructor
extern "C" void __fastcall FableCharStringConstruct_0099E4B0(void* self);         // CCharString default constructor
extern "C" void* __cdecl FableAllocate_00BFEA0E(unsigned int size);              // engine malloc

struct CStringParser
{
    void* vtbl;              // CStringParser's vtable
    unsigned char pad[8];    // String, BaseLineOffset (set by Init)
    CCharString member0c;    // Filename
    CCharString member10;    // TempCharString: our copy of the text
    void* sentinel;          // comment list head (self-linked sentinel node)

    // The real initialiser: parses from a raw buffer we keep alive.
    void Init(char* name, StringParserErrorCB cb, CCharString* ctx, long mode);
};

extern "C" void* __fastcall CStringParser_ctor(
    void* self, void*, const CCharString* src, StringParserErrorCB cb, CCharString* ctx, long mode)
{
    CStringParser* self_ = (CStringParser*)self;

    FableStringParserBaseConstruct_0099A2D0(self);
    self_->vtbl = (void*)0x0122d874;

    FableCharStringConstruct_0099E4B0(&self_->member0c);
    FableCharStringConstruct_0099E4B0(&self_->member10);

    // Empty comment list: a sentinel that points at itself.
    self_->sentinel = 0;
    unsigned int* sentinel = (unsigned int*)FableAllocate_00BFEA0E(0x10);
    sentinel[0] = (unsigned int)sentinel;
    sentinel[1] = (unsigned int)sentinel;
    self_->sentinel = sentinel;
    // Hold a reference to the text so the characters cannot go away under the parser.
    self_->member10 = *src;

    // Raw characters to parse from; an empty string uses the shared "" literal.
    void* srcRep = src->rep;
    char* name;
    if (srcRep == 0)
        goto useDefault;
    name = *(char**)srcRep;
    goto haveName;
useDefault:
    name = (char*)0x0129aaf4;
haveName:
    ;

    self_->Init(name, cb, ctx, mode);
    return self;
}
