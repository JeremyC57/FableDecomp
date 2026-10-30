// _global::GFParseCommandLine(char* commandLine) -- parse the -options of the command line: -staticmap "world", -q <n>, -build_retail_static_maps, -enable_map_optimizations; a named world skips the front end
// Leaf: ?GFParseCommandLine@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004138D0, 894 bytes (+2 int3), plain `ret`; /Gr __fastcall (the line in ecx).
// Callees: CStringParser(char*, callback, const CCharString&, long) 0x00414E30 (landed; retail parser 0x28 bytes, vtable
// 0x0122D874), CCharString(const char*, long) 0x0099EBF0 / () 0x0099E4B0 / dtor 0x0099EAE0, CStringParser::ReadAsStringUntilString
// (const CCharString&, CParsedItem*) 0x009BA110 and the CCharString-returning overload 0x009BA330, ReadAsSymbol 0x009BA790 (result
// discarded: skips the option marker), ReadAsString 0x009BA6F0, ReadAsInteger 0x009BA540, _stricmp 0x00BFEBA8, CCharString::
// ToWideString 0x0099E480, CWideString::operator= 0x0099B7D0 / operator!=(const wchar_t*) 0x0099B150 / dtor 0x0099B510, the
// parser's comment-list dtor 0x004046B0 and ~CBaseClass 0x0099A2E0.
// Data: options "-" "staticmap" "\"" "q" "build_retail_static_maps" "enable_map_optimizations" (0x0122E630..0x0122E5E8,
// 0x0122DF24); g_WorldToLoad 0x013B8668, g_bCompileFrontendDefinitions 0x013B8648, g_bRuntimeLevelFiles 0x013B8616,
// g_bStartMainGame 0x013B8605 (names as landed elsewhere); 0x013B864A / 0x013B864B / 0x01375490 / 0x0137548F / 0x0137544A
// named descriptively.
// Shape: the option compares are an inline case-insensitive equality taking `const CCharString&`, so each literal is an
// IMPLICIT conversion temporary -- that is what makes VC7.1 give the literal temporaries frame slots in source order ahead of
// the call-result temporaries (explicit `CCharString("...")` temporaries interleave with them, 894v894 with every slot
// shifted); the parser's file name is an explicit `CCharString("")` (constructed after the last argument is pushed).
// The build_retail_static_maps arm's last store is cross-jumped into the enable_map_optimizations arm.
#include <stddef.h>
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: "" (the string library's shared empty literal; this TU's own "" is 0x0122D70E)
class CCharStringData { public:
    char*         PData;                           // +0x0
    unsigned long Length;                          // +0x4
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator!=(const unsigned short* text) const;  // 0x0099B150
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    CWideString ToWideString() const;              // 0x0099E480
    const char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }   // the shared "" 0x0129AAF4
    bool EqualsNoCase(const CCharString& other) const { return _stricmp(GetString(), other.GetString()) == 0; }
};

struct CStringParserCommentList {                  // retail list<CStringParserCommentBoundaries>: the head pointer only
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
struct CCursorPos {
    bool          InQuotes;                        // +0x0
    unsigned long StringPos;                       // +0x4
    unsigned long Line;                            // +0x8
};
class CParsedItem { public:                               // 12 bytes
    long        Type;                              // +0x00
    long        Integer;                           // +0x04
    CCharString String;                            // +0x08
};
typedef void (*TStringParserErrorCallback)(CCharString* message);
class CStringParser : public CBaseClass {               // vtable 0x0122D874, 0x28 bytes in retail
public:
    char*                      String;             // +0x04
    unsigned long              BaseLineOffset;     // +0x08
    CCharString                Filename;           // +0x0C
    CCharString                TempCharString;     // +0x10
    CStringParserCommentList   Comments;           // +0x14
    TStringParserErrorCallback ErrorCallback;      // +0x18
    CCursorPos                 CursorPos;          // +0x1C
    CStringParser(char* text, TStringParserErrorCallback errorCallback = 0, const CCharString& filename = "",
                  long baseLineOffset = 0);        // 0x00414E30
    bool ReadAsStringUntilString(const CCharString& terminator, CParsedItem* item);   // 0x009BA110
    CCharString ReadAsStringUntilString(const CCharString& terminator);             // 0x009BA330
    CCharString ReadAsSymbol();                    // 0x009BA790
    CCharString ReadAsString();                    // 0x009BA6F0
    long ReadAsInteger();                          // 0x009BA540
};

extern CWideString g_WorldToLoad;                  // 0x013B8668
extern bool g_bCompileFrontendDefinitions;         // 0x013B8648
extern bool g_bBuildRetailStaticMaps;              // 0x013B864A
extern bool g_bEnableMapOptimisations;             // 0x013B864B
extern bool g_bRuntimeLevelFiles;                  // 0x013B8616
extern bool g_bStartMainGame;                      // 0x013B8605
extern bool g_bFrontEndFlag_137548F;               // 0x0137548F
extern bool g_bFrontEndFlag_137544A;               // 0x0137544A
extern long g_CommandLineQ;                        // 0x01375490

void GFParseCommandLine(char* commandLine)
{
    bool worldSpecified = false;
    if (commandLine != 0) {
        CStringParser parser(commandLine, 0, CCharString(""), 0);
        CParsedItem item;
        while (parser.ReadAsStringUntilString("-", &item)) {
            parser.ReadAsSymbol();
            CCharString option = parser.ReadAsString();
            if (option.EqualsNoCase("staticmap")) {
                if (!parser.ReadAsStringUntilString("\"", &item))
                    break;
                parser.ReadAsSymbol();
                g_WorldToLoad = parser.ReadAsStringUntilString("\"").ToWideString();
                if (g_WorldToLoad != (const unsigned short*)L"") {
                    worldSpecified = true;
                    g_bCompileFrontendDefinitions = true;
                }
            }
            else if (option.EqualsNoCase("q"))
                g_CommandLineQ = parser.ReadAsInteger();
            else if (option.EqualsNoCase("build_retail_static_maps")) {
                g_bBuildRetailStaticMaps = true;
                g_bRuntimeLevelFiles = true;
                g_bCompileFrontendDefinitions = true;
                g_bEnableMapOptimisations = true;
            }
            else if (option.EqualsNoCase("enable_map_optimizations"))
                g_bEnableMapOptimisations = true;
        }
    }
    if (worldSpecified) {
        g_bFrontEndFlag_137548F = false;
        g_bStartMainGame = true;
        g_bFrontEndFlag_137544A = false;
    }
}