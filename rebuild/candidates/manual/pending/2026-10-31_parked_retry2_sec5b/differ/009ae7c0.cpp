// CDefinitionManager::CompileDefinition(CStringParser& parser, const CCharString& className, const CCharString& defName) -- compile one text definition: resolve defName through the definition symbol map (else "Unable to find def class called <name>"), load the def and hold a counted pointer to it; an optional `specialises <base>` clause copies a Setup base def into it (keeping its Template bit and instantiation name; errors "Def <base> does not exist." / "Def <base> is not setup to specialise."); then read the body up to "#end_definition", compile every <tag>...</tag> block as a sub def (blanking the consumed text except newlines), re-initialise the parser over the cleaned text and Transfer the def through a text CPersistContext, marking it Setup and DefaultValsApplied
// Grade: DIFFER(1411v1456) 313/428 -- every call, string literal and the sub-def loop match in order; the residue is (a) the
//   `base` counted pointer: retail keeps it in edi and releases it on each path without a null test or zeroing store, so the
//   two Error blocks stay separate; here the shared-header ~CDefPointer (Release zeroes Object) keeps it in memory and VC
//   tail-merges the Error calls, (b) the Template bit-field write uses the xor-insert form instead of retail's and/or merge
//   with the Setup clear, (c) register homes (retail keeps the def index in esi after FindSymbol), which shift esp offsets
// Leaf: ?CompileDefinition@CDefinitionManager@@IAEXAAVCStringParser@@ABVCCharString@@1@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AE7C0, 1456 bytes, `ret 0xc` (className is unused).
// Name: Ego_r.pdb ?CompileDefinition@CDefinitionManager@@IAEXAAVCStringParser@@ABVCCharString@@1@Z; CDefClassBase slots
//   10/11/18/19 (GetInstantiationName / SetInstantiationName / Transfer / Copy) and the Setup/Template/DefaultValsApplied
//   bit fields from engine/CDefClassBase.h; CStringParser members from the landed ctor 0x00404720 (CursorPos +0x1c:
//   InQuotes, StringPos, Line); the inline CStringParser::Init(const CCharString&, ...) overload is Ego_r
//   ?Init@CStringParser@@QAEXABVCCharString@@P6IX0@Z0J@Z (its body = the landed ctor body).
// Callees: CSymbolMap::FindSymbol 0x009E5170, LoadDef 0x009AD6E0, CStringParser PeekNextItem 0x009BA8C0 / ReadAsString
//   0x009BA6F0 / ReadAsStringUntilString 0x009BA330 / 0x009B9BA0 (landed "FindKeyword") / 0x009B9E00 / Init 0x009B9530 /
//   ctor 0x00404720, CParsedItem::GetString 0x009B9880, CBasicString<char>::operator== 0x004115A0,
//   GetPDefFromInstantiationName 0x009ADA90, CDefString::operator CCharString 0x00415DD0, 0x009D5060 (CDefString from
//   CCharString), operator+ 0x0099F690 / 0x0099F570 / 0x0099F600, CCharString::SetChar 0x0099F390, CompileSubDefinition
//   0x009AE150, Error 0x009AC9F0, CPersistContext ctor 0x009BADD0 / dtor 0x009BBBF0, CBaseClassNonCopyable dtor 0x0099A2E0.
// Levers: strcmp(text, "") operand order (retail repe-cmpsb length 0xc); if/else around the FindSymbol failure (moves the
//   error block to the tail: 364 -> 314); short-circuit `||`/`&&` operator== (no materialised bool: 313). Inert: if-form
//   with non-null first, `!strcmp`, ternary (not inlined unless forced; forced still materialises), unsigned char Template
//   copy, Template-before-Setup order.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" int __cdecl strcmp(const char*, const char*);
#pragma intrinsic(strcmp)
class CCharStringData { public: char* Data; long Length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    void SetChar(long index, char c);                       // 0x0099F390
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data; }
    char operator[](long index) const { return GetString()[index]; }
    bool operator==(const char* text) const;
};
class CBasicStringChar { public:
    bool operator==(const char* text) const;                // 0x004115A0
};
__forceinline bool CCharString::operator==(const char* text) const
{
    return PStringData == 0 && strcmp(text, "") == 0 || PStringData != 0 && *(const CBasicStringChar*)PStringData == text;
}
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
class CDefString { public:
    long TablePos;
    CDefString(const CCharString& text);                    // 0x009D5060
    operator CCharString() const;                           // 0x00415DD0
};
typedef void (__fastcall* StringParserErrorCallback)(const CCharString&);
class CBaseClassNonCopyable { public:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A2E0
};
struct CCommentNode { CCommentNode* Next; CCommentNode* Prev; long Start; long End; };
class CCommentList { public:
    CCommentNode* Node;
    ~CCommentList()
    {
        CCommentNode* cur = Node->Next;
        while (cur != Node) {
            CCommentNode* tmp = cur;
            cur = cur->Next;
            free(tmp);
        }
        Node->Next = Node;
        Node->Prev = Node;
        if (Node)
            free(Node);
    }
};
class CParsedItem { public:
    long Type;
    long Value;
    CCharString String;
    const CCharString& GetString() const;                   // 0x009B9880
};
class CStringParser : public CBaseClassNonCopyable { public:
    char* String;                                           // +0x04
    unsigned long BaseLineOffset;                           // +0x08
    CCharString Filename;                                   // +0x0c
    CCharString TempCharString;                             // +0x10
    CCommentList Comments;                                  // +0x14
    StringParserErrorCallback ErrorCallback;                // +0x18
    bool InQuotes;                                          // +0x1c
    unsigned long StringPos;                                // +0x20
    unsigned long Line;                                     // +0x24
    CStringParser(const CCharString& text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x00404720
    void Init(const char* text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530
    void Init(const CCharString& text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
    {
        TempCharString = text;
        Init(text.GetString(), errorCallback, filename, baseLineOffset);
    }
    bool PeekNextItem(CParsedItem& item);                   // 0x009BA8C0
    CCharString ReadAsString();                             // 0x009BA6F0
    CCharString ReadAsStringUntilString(const CCharString& delimiter);   // 0x009BA330
    bool FindKeyword(const CCharString& keyword);           // 0x009B9BA0
    bool SkipPastNextItemIfIs(const CCharString& text);     // 0x009B9E00
};
class CSymbolMap { public:
    bool FindSymbol(const CCharString& name, long& value) const;   // 0x009E5170
};
class CPersistContext { public:
    unsigned char _pad[0x2c];
    CPersistContext(CStringParser& parser, StringParserErrorCallback errorCallback, bool useDefaults,
                    const CDefinitionManager* manager, const CSymbolMap* symbolMap, CCharString* stringToClean);   // 0x009BADD0
    ~CPersistContext();                                     // 0x009BBBF0
};
class CDefinitionManager { public:
    struct CInstantiatedDefInfo {
        long ClassName;
        long InstantiationName;
        unsigned long ClassIndex;
        CDefPointer<CDefClassBase> PDef;
    };
    unsigned char _pad0[0x58];
    CCountedPointer<CSymbolMap> PSymbolMap;                 // +0x58
    CCountedPointer<CSymbolMap> PDefinitionSymbolMap;       // +0x60
    unsigned char _pad68[0x3c];
    CInstantiatedDefInfo** InstantiatedDefs;                // +0xa4 CArray data
    unsigned char _padA8[0x8];
    StringParserErrorCallback ErrorCallback;                // +0xb0
    void LoadDef(unsigned long globalIndex);                // 0x009AD6E0
    CDefPointer<CDefClassBase> GetPDefFromInstantiationName(const CCharString& name);   // 0x009ADA90
    void CompileSubDefinition(CCharString text, const CCharString& subName, long parentIndex);   // 0x009AE150
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009AC9F0
    void CompileDefinition(CStringParser& parser, const CCharString& className, const CCharString& defName);
};

void CDefinitionManager::CompileDefinition(CStringParser& parser, const CCharString& className, const CCharString& defName)
{
    long index;
    if (PDefinitionSymbolMap->FindSymbol(defName, index)) {
        LoadDef(index);
        CDefPointer<CDefClassBase> def = InstantiatedDefs[index]->PDef;
        long line = parser.Line;
        CParsedItem item;
        bool specialised = false;
        if (parser.PeekNextItem(item) && item.Type == 2 && item.GetString() == "specialises") {
            parser.ReadAsString();
            CCharString baseName = parser.ReadAsString();
            CDefPointer<CDefClassBase> base = GetPDefFromInstantiationName(baseName);
            if (!base.IsNull()) {
                if (base->Setup) {
                    bool isTemplate = def->Template;
                    specialised = true;
                    CCharString name(def->GetInstantiationName());
                    def->Copy(base.Get());
                    def->Setup = false;
                    def->Template = isTemplate;
                    def->SetInstantiationName(CDefString(name));
                } else
                    Error(parser, CCharString("Def ") + baseName + " is not setup to specialise.");
            } else
                Error(parser, CCharString("Def ") + baseName + " does not exist.");
        }
        CCharString text = parser.ReadAsStringUntilString("#end_definition");
        CStringParser sub(text, ErrorCallback, "", line);
        bool useDefaults = !specialised && !def->DefaultValsApplied;
        while (sub.FindKeyword("<")) {
            long start = sub.StringPos;
            CCharString tag = sub.ReadAsStringUntilString(">");
            sub.SkipPastNextItemIfIs(">");
            CCharString closeTag = "<\\" + tag + ">";
            CCharString body = sub.ReadAsStringUntilString(closeTag);
            sub.FindKeyword(closeTag);
            long end = sub.StringPos;
            for (long i = start - 1; i < end; ++i)
                if (text[i] != '\n')
                    text.SetChar(i, ' ');
            CompileSubDefinition(body, tag, index);
        }
        sub.Init(text, ErrorCallback, "", line);
        CPersistContext context(sub, ErrorCallback, useDefaults, this, PSymbolMap.Get(), &text);
        def->Transfer(context);
        def->Setup = true;
        def->DefaultValsApplied = true;
    } else
        Error(parser, "Unable to find def class called " + defName);
}
