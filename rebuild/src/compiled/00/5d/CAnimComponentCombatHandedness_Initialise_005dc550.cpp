// Retail CStringParser is0x28, proven by landed00404720: Comments one pointer+14, ErrorCallback+18, CursorPos[3]+1c.
// Generated parser header is0x30 because donor Comments occupies12 bytes; its explicit vptr also cannot supply virtual cleanup.
// CAnimComponentCombatHandedness::Initialise(CPersistContext&, const CCharString&) -- parse and store start/end handedness.
// Leaf: ?Initialise@CAnimComponentCombatHandedness@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x005DC550, 252-byte virtual method (the row's final 4 bytes are padding). Reset is vtable slot 8;
// parser/string/list/base cleanup callees are 0x0099EBF0, 0x00404720, 0x0099EAE0,
// 0x004046B0, and 0x0099A2E0. The PDB header is data-only (size 0x10); it confirms
// StartHandedness +8 and EndHandedness +0xc, while the local virtual model supplies slots.
class CPersistContext;
class CCharString { public: void* m_pData; CCharString(const char*, long); ~CCharString(); };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); }; // retail list is one sentinel pointer (4 bytes)
class CStringParser : public CBaseClass { public:
 char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
 CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
 CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);
 bool SkipPastNextItemIfIs(const CCharString&); // 009B9E00
};
void __fastcall TransferIn(CPersistContext*, CStringParser*, long*); // 009E2590
inline long ParseLong(CPersistContext& c, CStringParser& p) { long v; TransferIn(&c,&p,&v); return v; }
class CAnimComponentCombatHandedness { public:
 virtual ~CAnimComponentCombatHandedness(); virtual void v01(); virtual void v02();
 virtual bool Initialise(CPersistContext&, const CCharString&); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void Reset();
 long IVRefCount; long StartHandedness; long EndHandedness;
};
bool CAnimComponentCombatHandedness::Initialise(CPersistContext& context, const CCharString& text) {
 Reset();
 CStringParser parser(text, 0, CCharString("", -1), 0); // pooled empty filename at 0x0122D70E
 long start=ParseLong(context, parser);
 bool notFound;
 {
  CCharString marker(",", -1); // marker literal at 0x0122E024
  notFound=!parser.SkipPastNextItemIfIs(marker);
 }
 if (notFound) return false;
 long end=ParseLong(context, parser);
 StartHandedness=start;
 EndHandedness=end;
 return true;
}