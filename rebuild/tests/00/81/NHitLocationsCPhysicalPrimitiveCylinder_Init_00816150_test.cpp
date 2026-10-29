// Behaviour test for 00816150: src/00816150.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NHitLocations::CPhysicalPrimitiveCylinder::Init(CPersistContext& context, const CCharString& text)
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NHitLocations::CPhysicalPrimitiveCylinder::Init(CPersistContext& context, const CCharString& text) -- parse "BaseName,Radius,Height" from the def text; an empty text gives the null base name and -1 radius/height
// Retail: Fable.exe (TLC, VC7.1) @ 0x00816150, 406 bytes, ret 8.
// Callees: CBasicString<char>::operator==(const char*) 0x004115A0, CCharString(const char*, long) 0x0099EBF0, ~CCharString
//   0x0099EAE0, CStringParser ctor 0x00404720, CPersistTraits<CDefString>::TransferIn 0x007E3820, CPersistTraits<float>::TransferIn
//   0x0040F7E0, CStringParser::SkipPastNextItemIfIs 0x009B9E00, comment-list dtor 0x004046B0, CBaseClass::~CBaseClass 0x0099A2E0.
// Shape: CPersistTraits<T> is an EMPTY class (landed 0x005DCA60); its temporaries sit on the dead `text` argument slot together
//   with the parsed float. One function-scope `float value` declared after the first ',' test receives both floats, so the
//   second ',' marker cannot reuse its word and takes the dead `context` slot as retail. The float TransferIn instantiation is
//   given a same-TU noinline body (template instantiation compiled in retail's TU): with it VC7.1 knows &value does not
//   escape and lays the traits temporary and the value on the same word (two separate lea's, as retail).
// Leaf: ?Init@CPhysicalPrimitiveCylinder@
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CPersistContext;
class CBaseClass {
public:  virtual ~CBaseClass(); };             // dtor 0x0099A2E0
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); };   // 0x004046B0
class CStringParser : public CBaseClass { public:
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
    CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
    CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);   // 0x00404720
    bool SkipPastNextItemIfIs(const CCharString&);  // 0x009B9E00
};
class CDefString { public:
    long Index;
    CDefString() : Index(-1) {}
};
template<class T> struct CPersistTraits {
    CPersistTraits() {}
    void TransferIn(CPersistContext&, CStringParser&, T*);  // CDefString 0x007E3820, float 0x0040F7E0
};
#pragma auto_inline(off)
float ParseFloatItem_X(CPersistContext&, CStringParser&);   // stand-in for the parser/context reads of 0x0040F7E0
template<> __declspec(noinline) void CPersistTraits<float>::TransferIn(CPersistContext& c, CStringParser& p, float* value) { *value = ParseFloatItem_X(c, p); }
#pragma auto_inline(on)

template<> struct CPersistTraits<CDefString> {   // DefaultVal not modelled: retail never initialises it here
    CPersistTraits() {}
    void TransferIn(CPersistContext&, CStringParser&, CDefString*);   // 0x007E3820
};
namespace NHitLocations {
struct CPhysicalPrimitiveCylinder {
    virtual ~CPhysicalPrimitiveCylinder();
    CDefString BaseName;                        // +0x04
    float      Radius;                          // +0x08
    float      Height;                          // +0x0c
    bool Init(CPersistContext& context, const CCharString& text);
};

bool CPhysicalPrimitiveCylinder::Init(CPersistContext& context, const CCharString& text)
{
    if (text == "") {
        Height = Radius = -1.0f;
        BaseName = CDefString();
        return true;
    }
    CStringParser parser(text, 0, CCharString("", -1), 0);
    CPersistTraits<CDefString>().TransferIn(context, parser, &BaseName);
    bool notFound;
    { CCharString marker(",", -1); notFound = !parser.SkipPastNextItemIfIs(marker); }
    if (notFound) return false;
    float value;
    CPersistTraits<float>().TransferIn(context, parser, &value);
    Radius = value;
    { CCharString marker(",", -1); notFound = !parser.SkipPastNextItemIfIs(marker); }
    if (notFound) return false;
    CPersistTraits<float>().TransferIn(context, parser, &value);
    Height = value;
    return true;
}
}

// ---- stubs and driver ----
class CPersistContext { public: int tag; };
static int g_live = 0;                 // live CCharString objects
static int g_ctxBad = 0;               // calls that got the wrong context
static CPersistContext* g_ctx = 0;
CCharString::CCharString(const char* text, long length)
{
    ++g_live;
    PStringData = (CCharStringData*)calloc(1, sizeof(CCharStringData));
    size_t n = length < 0 ? strlen(text) : (size_t)length;
    PStringData->Data.PData = (char*)calloc(n + 1, 1);
    memcpy(PStringData->Data.PData, text, n);
    PStringData->Data.StringLength = n;
}
CCharString::~CCharString()
{
    --g_live;
    if (PStringData) { free(PStringData->Data.PData); free(PStringData); }
}
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
CBaseClass::~CBaseClass() {}
CStringParserCommentList::~CStringParserCommentList() {}
CStringParser::CStringParser(const CCharString& text, void (*cb)(CCharString*), const CCharString& file, long mode)
    : Filename("", -1), TempCharString("", -1)
{
    String = text.PStringData ? text.PStringData->Data.PData : (char*)"";
    BaseLineOffset = 0; Comments.head = 0; ErrorCallback = cb; CursorPos[0] = CursorPos[1] = CursorPos[2] = 0;
    if (cb != 0 || mode != 0 || !file.PStringData || file.PStringData->Data.PData[0] != 0) ++g_ctxBad;
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item)
{
    const char* m = item.PStringData->Data.PData;
    size_t n = strlen(m);
    if (strncmp(String + CursorPos[0], m, n) != 0) return false;
    CursorPos[0] += (long)n;
    return true;
}
void CPersistTraits<CDefString>::TransferIn(CPersistContext& c, CStringParser& p, CDefString* v)
{
    if (&c != g_ctx) ++g_ctxBad;
    long n = 0;
    while (p.String[p.CursorPos[0]] && p.String[p.CursorPos[0]] != ',' && p.String[p.CursorPos[0]] != ';') { ++n; ++p.CursorPos[0]; }
    v->Index = 100 + n;                // def index stand-in: 100 + name length
}
float ParseFloatItem_X(CPersistContext& c, CStringParser& p)
{
    if (&c != g_ctx) ++g_ctxBad;
    char* end = 0;
    double d = strtod(p.String + p.CursorPos[0], &end);
    p.CursorPos[0] = (long)(end - p.String);
    return (float)d;
}
NHitLocations::CPhysicalPrimitiveCylinder::~CPhysicalPrimitiveCylinder() {}
static void Reset(NHitLocations::CPhysicalPrimitiveCylinder& c) { c.BaseName.Index = 7; c.Radius = 9.0f; c.Height = 8.0f; }
int main()
{
    CPersistContext ctx; ctx.tag = 1; g_ctx = &ctx;
    NHitLocations::CPhysicalPrimitiveCylinder cyl;
    // 1: null string data -> empty: defaults, true
    {
        CCharString t("x", -1); free(t.PStringData->Data.PData); free(t.PStringData); t.PStringData = 0;
        Reset(cyl);
        CHECK(cyl.Init(ctx, t) == true);
        CHECK(cyl.BaseName.Index == -1 && cyl.Radius == -1.0f && cyl.Height == -1.0f);
    }
    // 2: allocated but empty text -> same
    { CCharString t("", -1); Reset(cyl); CHECK(cyl.Init(ctx, t) == true); CHECK(cyl.BaseName.Index == -1 && cyl.Radius == -1.0f && cyl.Height == -1.0f); }
    // 3: full "Ab,1.5,2.25"
    { CCharString t("Ab,1.5,2.25", -1); Reset(cyl); CHECK(cyl.Init(ctx, t) == true); CHECK(cyl.BaseName.Index == 102 && cyl.Radius == 1.5f && cyl.Height == 2.25f); }
    // 4: no first comma -> false after the name, floats untouched
    { CCharString t("Abc;1.5", -1); Reset(cyl); CHECK(cyl.Init(ctx, t) == false); CHECK(cyl.BaseName.Index == 103 && cyl.Radius == 9.0f && cyl.Height == 8.0f); }
    // 5: no second comma -> false with the radius set, height untouched
    { CCharString t("A,3.5;2", -1); Reset(cyl); CHECK(cyl.Init(ctx, t) == false); CHECK(cyl.BaseName.Index == 101 && cyl.Radius == 3.5f && cyl.Height == 8.0f); }
    CHECK(g_live == 0);
    CHECK(g_ctxBad == 0);
    if (fails == 0) printf("00816150_TEST PASS\n");
    return fails;
}