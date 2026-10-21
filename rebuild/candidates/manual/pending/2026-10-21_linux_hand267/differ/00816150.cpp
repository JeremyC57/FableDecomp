// Grade: DIFFER(400v406) -- two residues: (1) retail computes the float traits' this and the value pointer as separate lea's of the same slot (ours CSEs them: mov ecx,edx), so retail's value is a distinct expression aliasing the CPersistTraits<float> temporary; (2) retail gives the second ',' marker temporary the dead context-argument slot (esp+38h) while ours reuses the value slot. Tried: per-parse float locals (frame 0x30), inline ParseFloat returning a local (406v406), traits.DefaultVal value (best, this file), named locals, a shared function-scope traits/value (frame 0x2c), natural SkipPastNextItemIfIs(CCharString(...)) (394v406)
// NHitLocations::CPhysicalPrimitiveCylinder::Init(CPersistContext& context, const CCharString& text) -- parse "BaseName,Radius,Height" from the def text; an empty text gives the null base name and -1 radius/height
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
    T DefaultVal;
    CPersistTraits() {}
    void TransferIn(CPersistContext&, CStringParser&, T*);  // CDefString 0x007E3820, float 0x0040F7E0
};
inline float ParseFloat(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<float> traits;
    traits.TransferIn(context, parser, &traits.DefaultVal);
    return traits.DefaultVal;
}
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
    Radius = ParseFloat(context, parser);
    { CCharString marker(",", -1); notFound = !parser.SkipPastNextItemIfIs(marker); }
    if (notFound) return false;
    Height = ParseFloat(context, parser);
    return true;
}
}
