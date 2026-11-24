// Retail CStringParser is0x28, proven by landed00404720: Comments one pointer+14, ErrorCallback+18, CursorPos[3]+1c.
// Generated parser header is0x30 because donor Comments occupies12 bytes; its explicit vptr also cannot supply virtual cleanup.
// CAnimComponentCombatMisc::Initialise(CPersistContext&, const CCharString&) -- parse both combat miscellany flags separated by marker
// Leaf: ?Initialise@CAnimComponentCombatMisc@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x005DC7B0, 256 bytes. Reset is slot 8; string/parser/list/base
// cleanup callees are 0x0099EBF0, 0x00404720, 0x0099EAE0, 0x004046B0, 0x0099A2E0.
// Header is data-only (size 0x0a), confirming MeleeFlourish +8 and MeleeKnockdown +9;
// the polymorphic local model supplies Initialise at slot 3 and Reset at slot 8.
class CPersistContext;
class CCharString { public: void* data; CCharString(const char*, long); ~CCharString(); };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CStringParserCommentBoundaries;
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (declaration-only _List_base ctor / dtor): the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"
class CStringParser : public CBaseClass { public:
 char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
 std::list<CStringParserCommentBoundaries> Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
 CStringParser(const CCharString&, void (__fastcall*)(const CCharString&), const CCharString&, long);
 bool SkipPastNextItemIfIs(const CCharString&); // 009B9E00
};
// bool TransferIn: 00403FF0 (thiscall).
template<class T> struct CPersistTraits { CPersistTraits() {} void TransferIn(CPersistContext&, CStringParser&, T*); };
class CAnimComponentCombatMisc { public:
 virtual ~CAnimComponentCombatMisc(); virtual void v1(); virtual void v2(); virtual bool Initialise(CPersistContext&, const CCharString&);
 virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void Reset();
 long base; bool MeleeFlourish; bool MeleeKnockdown;
};
bool CAnimComponentCombatMisc::Initialise(CPersistContext& context, const CCharString& text) {
 Reset();
 CStringParser parser(text, 0, CCharString("", -1), 0); // pooled empty filename at 0x0122D70E
 CPersistTraits<bool>().TransferIn(context, parser, &MeleeFlourish);
 bool notFound;
 { CCharString marker(",", -1); notFound=!parser.SkipPastNextItemIfIs(marker); } // literal at 0x0122E024
 if (notFound) return false;
 CPersistTraits<bool>().TransferIn(context, parser, &MeleeKnockdown);
 return true;
}