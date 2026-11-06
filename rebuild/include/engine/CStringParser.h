#ifdef FABLE_RETAIL_CSTRINGPARSER
#ifndef FABLE_ENGINE_CSTRINGPARSER_H
#define FABLE_ENGINE_CSTRINGPARSER_H
// Retail opt-in (Codex pass11): original TPI base and names; native offsets
// +4/+8/+c/+10/+14/+18/+1c and total40 after STLport list correction.
#include "engine/CBaseClass.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CParsedItem.h"
// Layout: original pointer boundaries, size8, class keyV.
class CStringParserCommentBoundaries {public:const char* PStart;const char* PEnd;};
class CStringParser : public CBaseClass {
public:
    CStringParser(const CCharString&,void (__fastcall*)(const CCharString&)=0,const CCharString& filename=CCharString(""),long baseLineOffset=0);
    bool ReadAsStringUntilString(const CCharString&,CParsedItem&);
    bool SkipPastString(const CCharString&);
    bool SkipPastNextItemIfIs(const CCharString&);
    bool SkipUntilString(const CCharString&);
    CCharString ReadAsStringUntilString(const CCharString&);
    long SkipPastNewline();
    bool AtEnd(){return String[CursorPos.StringPos]==0;}
    // pass 12 header wave (0x009BC120): Ego_r / FableWin ??0CStringParser@@QAE@ABV0@@Z, ?SkipPastWholeString@CStringParser@@QAE_NABVCCharString@@@Z,
    // ?ReadAsStringUntilPastString@CStringParser@@QAE?AVCCharString@@ABV2@@Z (out of line); ?GetStringPos@CStringParser@@QBEKXZ,
    // ?GetFilename@CStringParser@@QBEABVCCharString@@XZ, ?GetErrorCallback@CStringParser@@QBEP6IXABVCCharString@@@ZXZ (inline in retail).
    CStringParser(const CStringParser&);
    bool SkipPastWholeString(const CCharString&);
    CCharString ReadAsStringUntilPastString(const CCharString&);
    unsigned long GetStringPos() const { return CursorPos.StringPos; }
    const CCharString& GetFilename() const { return Filename; }
    void (__fastcall *GetErrorCallback() const)(const CCharString&) { return ErrorCallback; }
    // pass 12 header wave (0x00AE8D50): Ego_r / FableWin ?Init@CStringParser@@QAEXPBDP6IXABVCCharString@@@Z1J@Z,
    // ?SetStringPos@CStringParser@@QAEXK@Z, ?SkipWhitespace@CStringParser@@QAEJXZ (out of line).
    void Init(const char*,void (__fastcall*)(const CCharString&),const CCharString&,long);
    void SetStringPos(unsigned long);
    long SkipWhitespace();
    // Codex pass 13 (row 0x009BB110 and its siblings): Ego_r / FableWin ??0CStringParser@@QAE@XZ, ??0CStringParser@@QAE@PBDP6IXABVCCharString@@@Z1J@Z,
    // ?Init@CStringParser@@QAEXABVCCharString@@P6IX0@Z0J@Z, ?PeekNextItemType@..., ?ReadNextItem@..., ?ReadAsInteger@..., ?NextItemExists@...,
    // ?ReadAsStringUntilWhitespace@... (out of line); ?GetLine@CStringParser@@QBEKXZ inline.
    CStringParser();
    CStringParser(const char*,void (__fastcall*)(const CCharString&)=0,const CCharString& filename=CCharString(""),long baseLineOffset=0);
    void Init(const CCharString&,void (__fastcall*)(const CCharString&),const CCharString&,long);
    unsigned long GetLine() const { return CursorPos.Line; }
    EParsedItemType PeekNextItemType();
    bool ReadNextItem(CParsedItem&);
#ifdef FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM
    // Chunk 778 (row 0x00615E70): Ego_r.pdb ?PeekNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z (out of line, 0x009BA8C0).
    bool PeekNextItem(CParsedItem&);
#endif
    long ReadAsInteger();
#ifdef FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
    // Nested opt-in (chunk 779, row 0x00837E40 CThingTrackNode::ConstructFromLevelScript): Ego_r.pdb ?ReadAsUHUGE@CStringParser@@QAE_KXZ
    // (retail 0x009BA5B0, result in edx:eax; bound by the landed CTCDRegionExit::OnSerialise 0x00779430), out of line. Declaration only.
    unsigned __int64 ReadAsUHUGE();                     // 0x009BA5B0
#endif
    bool NextItemExists();
#ifdef FABLE_RETAIL_CSTRINGPARSER_READQUOTED
    // Nested opt-in (chunk 795, row 0x004B5500 CQuestManager::LoadGameState): Ego_r.pdb / FableWin.pdb
    // ?ReadNextItemAsQuotedString@CStringParser@@QAE_NAAVCCharString@@@Z (retail 0x009BA1D0, landed; returns bool), out of line.
    bool ReadNextItemAsQuotedString(CCharString&);      // 0x009BA1D0
#endif
    CCharString ReadAsStringUntilWhitespace();
    CCharString ReadAsSymbol();
    CCharString ReadAsString();
    // chunk 751 (row 0x009BA650): Ego_r.pdb ?ReadAsFloat@CStringParser@@QAEMXZ and the protected ?Error@CStringParser@@IAEXABVCCharString@@@Z
    // (0x009B95F0, landed).
    float ReadAsFloat();
    class CCursorPos {
    public:
        bool InQuotes;
        unsigned long StringPos,Line;
    };
protected:
    void Error(const CCharString&);
#ifdef FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS
    // Chunk 756 (row 0x009B9D70 SkipPastNewline): Ego_r.pdb ?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z and ?MoveStringPos@CStringParser@@IAEXK@Z
    // (protected). Inline: retail SkipPastNewline / SkipWhitespace expand both (the out-of-line CalcInQuotes copy is 0x009B9570).
    __forceinline bool CalcInQuotes(bool inQuotes,const char* from,const char* to) const {
        for(const char* p=from;p!=to;++p)
            if(*p=='"'||*p=='\'')
                inQuotes=!inQuotes;
        return inQuotes;
    }
    __forceinline void MoveStringPos(unsigned long n) {
        if(n) {
            CursorPos.InQuotes=CalcInQuotes(CursorPos.InQuotes,String+CursorPos.StringPos,String+CursorPos.StringPos+n);
            CursorPos.StringPos+=n;
        }
    }
#endif
    const char* String;
    unsigned long BaseLineOffset;
    CCharString Filename,TempCharString;
    std::list<CStringParserCommentBoundaries> Comments;
    void (__fastcall *ErrorCallback)(const CCharString&);
    CCursorPos CursorPos;
};
FABLE_STATIC_ASSERT(sizeof(CStringParser)==0x28);
#endif
#else
#ifndef FABLE_ENGINE_CSTRINGPARSER_H
#define FABLE_ENGINE_CSTRINGPARSER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CStringParser = 0x30.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CStringParser = 0x28.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCharStringData;
struct _func___cdecl_void_CCharString_ptr;

#pragma pack(push, 1)
class CStringParser {
public:
    void*                               __vftable;           // +0x00 vptr, or first dword of a flattened base subobject
    void*                               String;              // +0x04 char *
    unsigned long                       BaseLineOffset;      // +0x08
    CCharStringData*                    Filename_PStringData; // +0x0c CCharString::PStringData
    CCharStringData*                    TempCharString_PStringData; // +0x10 CCharString::PStringData
    unsigned char                       Comments[0x4];       // +0x14 list<CStringParserCommentBoundaries,std::allocator<CStringParserCommentBoundaries>_>
    _func___cdecl_void_CCharString_ptr* ErrorCallback;       // +0x18
    unsigned char                       CursorPos[0xc];      // +0x1c CCursorPos
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CStringParser) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CStringParser, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CStringParser, String) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CStringParser, BaseLineOffset) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CStringParser, Filename_PStringData) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CStringParser, TempCharString_PStringData) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CStringParser, Comments) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CStringParser, ErrorCallback) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CStringParser, CursorPos) == 0x1c);

#endif // FABLE_ENGINE_CSTRINGPARSER_H

#endif
