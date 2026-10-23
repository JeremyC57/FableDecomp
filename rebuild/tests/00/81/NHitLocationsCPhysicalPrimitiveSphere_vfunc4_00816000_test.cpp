#include <stdio.h>
// BEGIN exact source snapshot SHA256 922ad2b3e7017e6a97d5800219b2155de2c3adc799fdf9154458a92c9a8e0f9f
// PDB-recovered CPhysicalPrimitiveSphere::Init. It reads a base-name and a
// radius from the persisted text; an empty string resets both fields.
#include <string.h>
#define CStringParser CStringParserPdb
#include "engine/CStringParser.h"
#undef CStringParser
#pragma pack(push, 1)
struct CBasicStringChar {
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    bool operator==(const char*) const;
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char*, long);
    ~CCharString();
    bool operator==(const char* text) const {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0) return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CPersistContext;
struct CBaseClass { virtual ~CBaseClass(); };
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); };
// Layout: donor CStringParser is0x30 with12B Comments; retail list is4B.
// Retail parser is0x28: Comments14,ErrorCallback18,CursorPos1c.
struct CStringParser : CBaseClass {
    char* String; unsigned long BaseLineOffset; CCharString Filename;
    CCharString TempCharString; CStringParserCommentList Comments;
    void (*ErrorCallback)(CCharString*); long CursorPos[3];
    CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);
    bool SkipPastNextItemIfIs(const CCharString&);
};
struct CDefString {
    long Index;
    CDefString() : Index(-1) {}
};
template<class T> struct CPersistTraits {
    CPersistTraits() {}
    void TransferIn(CPersistContext&, CStringParser&, T*);
};
#pragma auto_inline(off)
float ParseFloatItem_X(CPersistContext&, CStringParser&);
template<> __declspec(noinline) void CPersistTraits<float>::TransferIn(
    CPersistContext& c, CStringParser& p, float* value)
{ *value = ParseFloatItem_X(c, p); }
#pragma auto_inline(on)
template<> struct CPersistTraits<CDefString> {
    CPersistTraits() {}
    void TransferIn(CPersistContext&, CStringParser&, CDefString*);
};

namespace NHitLocations {
struct CPhysicalPrimitiveSphere {
    virtual ~CPhysicalPrimitiveSphere();
    CDefString BaseName; // +0x04
    float Radius;        // +0x08
    bool Init(CPersistContext&, const CCharString&);
};
bool CPhysicalPrimitiveSphere::Init(CPersistContext& context,
                                    const CCharString& text)
{
    if (text == "") {
        Radius = -1.0f;
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
    return true;
}
}

// END exact source snapshot
struct CPersistContext { int testState; };
static CCharStringData strings[16];
static int nextString;
static bool commaFound = true;
static float parsedValue = 2.5f;

CBaseClass::~CBaseClass() { }
CStringParserCommentList::~CStringParserCommentList() { }
NHitLocations::CPhysicalPrimitiveSphere::~CPhysicalPrimitiveSphere() { }
bool CBasicStringChar::operator==(const char* text) const
{
    return strcmp(PData, text) == 0;
}
CCharString::CCharString(const char* text, long)
{
    if (text == 0 || text[0] == 0) { PStringData = 0; return; }
    PStringData = &strings[nextString++];
    PStringData->Data.PData = const_cast<char*>(text);
    PStringData->Data.StringLength = (unsigned long)strlen(text);
    PStringData->Data.DataLength = (unsigned long)strlen(text);
    PStringData->Data.UseFastExtend = false;
    PStringData->NoRefs = 1;
}
CCharString::~CCharString() { }
CStringParser::CStringParser(const CCharString&, void (*)(CCharString*),
                             const CCharString&, long)
    : String(0), BaseLineOffset(0), Filename(0, -1), TempCharString(0, -1),
      ErrorCallback(0)
{
    CursorPos[0] = CursorPos[1] = CursorPos[2] = 0;
    Comments.head = 0;
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString&) { return commaFound; }
void CPersistTraits<CDefString>::TransferIn(
    CPersistContext&, CStringParser&, CDefString* out)
{ out->Index = 17; }
float ParseFloatItem_X(CPersistContext&, CStringParser&) { return parsedValue; }

int main()
{
    if (sizeof(CStringParser)!=0x28 || offsetof(CStringParser,ErrorCallback)!=0x18 ||
        offsetof(CStringParser,CursorPos)!=0x1c) return 4;
    static NHitLocations::CPhysicalPrimitiveSphere sphere;
    CPersistContext context;
    CCharString empty("", -1);
    if (!sphere.Init(context, empty) || sphere.BaseName.Index != -1 ||
        sphere.Radius != -1.0f) return 1;

    CCharString text("Base,2.5", -1);
    commaFound = true;
    parsedValue = 2.5f;
    if (!sphere.Init(context, text) || sphere.BaseName.Index != 17 ||
        sphere.Radius != 2.5f) return 2;

    commaFound = false;
    sphere.Radius = 8.0f;
    if (sphere.Init(context, text) || sphere.Radius != 8.0f) return 3;
    puts("00816000_TEST PASS");
    return 0;
}