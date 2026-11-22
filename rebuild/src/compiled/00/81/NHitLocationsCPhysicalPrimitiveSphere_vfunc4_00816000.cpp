// PDB-recovered CPhysicalPrimitiveSphere::Init. It reads a base-name and a
// radius from the persisted text; an empty string resets both fields.
#include <string.h>
#define CStringParser CStringParserPdb
#include "engine/CStringParser.h"
#undef CStringParser
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    bool operator==(const char*) const;
};
class CCharStringData { public: CBasicString<char> Data; long NoRefs; };
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
class CPersistContext;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CStringParserCommentBoundaries;
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (declaration-only _List_base ctor / dtor): the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"
// Layout: donor CStringParser is0x30 with12B Comments; retail list is4B.
// Retail parser is0x28: Comments14,ErrorCallback18,CursorPos1c.
class CStringParser : public CBaseClass { public:
    char* String; unsigned long BaseLineOffset; CCharString Filename;
    CCharString TempCharString; std::list<CStringParserCommentBoundaries> Comments;
    void (*ErrorCallback)(CCharString*); long CursorPos[3];
    CStringParser(const CCharString&, void (__fastcall*)(const CCharString&), const CCharString&, long);
    bool SkipPastNextItemIfIs(const CCharString&);
};
class CDefString { public:
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