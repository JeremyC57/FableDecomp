// Retail CStringParser is0x28, proven by landed00404720: Comments one pointer+14, ErrorCallback+18, CursorPos[3]+1c.
// Generated parser header is0x30 because donor Comments occupies12 bytes; its explicit vptr also cannot supply virtual cleanup.
// CAnimComponentCombatHandedness::Initialise(CPersistContext&, const CCharString&) -- parse and store start/end handedness.
// Leaf: ?Initialise@CAnimComponentCombatHandedness@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x005DC550, 252-byte virtual method (the row's final 4 bytes are padding). Reset is vtable slot 8;
// parser/string/list/base cleanup callees are 0x0099EBF0, 0x00404720, 0x0099EAE0,
// 0x004046B0, and 0x0099A2E0. The PDB header is data-only (size 0x10); it confirms
// StartHandedness +8 and EndHandedness +0xc, while the local virtual model supplies slots.
// NF136 (2026-11-20, q5199 / q5220): the free stand-in declaration TransferIn(CPersistContext*, CStringParser*, long*) and the local
//   ParseLong helper replaced by the shared engine/CPersistTraits.h CPersistTraitsSLONG (partial mode FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY):
//   its inline long-returning TransferIn(context, parser) over the static __fastcall Ego_r.pdb / FableWin.pdb
//   ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z (0x009E2590, the landed self spelling).
class CPersistContext;
class CCharString { public: void* m_pData; CCharString(const char*, long); ~CCharString(); };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CStringParserCommentBoundaries;
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public: void* head; ~list(); };
} // namespace std -- retail list is one sentinel pointer (4 bytes)
class CStringParser : public CBaseClass { public:
 char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
 std::list<CStringParserCommentBoundaries> Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
 CStringParser(const CCharString&, void (__fastcall*)(const CCharString&), const CCharString&, long);
 bool SkipPastNextItemIfIs(const CCharString&); // 009B9E00
};
#define FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY
#include "engine/CPersistTraits.h"
class CAnimComponentCombatHandedness { public:
 virtual ~CAnimComponentCombatHandedness(); virtual void v01(); virtual void v02();
 virtual bool Initialise(CPersistContext&, const CCharString&); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void Reset();
 long IVRefCount; long StartHandedness; long EndHandedness;
};
bool CAnimComponentCombatHandedness::Initialise(CPersistContext& context, const CCharString& text) {
 Reset();
 CStringParser parser(text, 0, CCharString("", -1), 0); // pooled empty filename at 0x0122D70E
 long start=CPersistTraitsSLONG::TransferIn(context, parser);
 bool notFound;
 {
  CCharString marker(",", -1); // marker literal at 0x0122E024
  notFound=!parser.SkipPastNextItemIfIs(marker);
 }
 if (notFound) return false;
 long end=CPersistTraitsSLONG::TransferIn(context, parser);
 StartHandedness=start;
 EndHandedness=end;
 return true;
}