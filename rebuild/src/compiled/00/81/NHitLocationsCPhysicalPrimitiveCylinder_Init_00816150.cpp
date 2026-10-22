// NHitLocations::CPhysicalPrimitiveCylinder::Init(CPersistContext& context, const CCharString& text) -- parse "BaseName,Radius,Height" from the def text; an empty text gives the null base name and -1 radius/height
// Retail: Fable.exe (TLC, VC7.1) @ 0x00816150, 406 bytes, ret 8.
// Callees: CBasicString<char>::operator==(const char*) 0x004115A0, CCharString(const char*, long) 0x0099EBF0, ~CCharString
//   0x0099EAE0, CStringParser ctor 0x00404720, CPersistTraits<CDefString>::TransferIn 0x007E3820, CPersistTraits<float>::TransferIn
//   0x0040F7E0, CStringParser::SkipPastNextItemIfIs 0x009B9E00, comment-list dtor 0x004046B0, CBase::~CBase 0x0099A2E0.
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
struct CCharStringData {
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
struct CPersistContext;
struct CBase { virtual ~CBase(); };             // dtor 0x0099A2E0
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); };   // 0x004046B0
struct CStringParser : CBase {
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
    CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
    CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);   // 0x00404720
    bool SkipPastNextItemIfIs(const CCharString&);  // 0x009B9E00
};
struct CDefString {
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