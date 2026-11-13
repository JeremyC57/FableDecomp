#ifndef FABLE_ENGINE_CDATEANDTIME_H
#define FABLE_ENGINE_CDATEANDTIME_H
// Original Ego_r TPI and frontend destructor: trivial28-byte element.
#ifdef FABLE_RETAIL_CDATEANDTIME_TOFILETIME
#ifndef FABLE_RETAIL_WIN32_FILETIME
#define FABLE_RETAIL_WIN32_FILETIME
#endif
#include "engine/Win32Types.h"
#endif
#if defined(FABLE_RETAIL_CDATEANDTIME_SYSTEMTIME) && !defined(FABLE_RETAIL_CDATEANDTIME_CTOR)
// Retail opt-in (wf19, row 0x0055D180 CTCPhotojournalCapture::TakeQuickScreenshot): Ego_r.pdb / FableWin.pdb
// ??0CDateAndTime@@QAE@ABU_SYSTEMTIME@@@Z (retail 0x009E9A30) and ?ToFileTime@CDateAndTime@@QBE?AU_FILETIME@@XZ (retail 0x009E9D40,
// hidden result). Declarations only; the caller completes _SYSTEMTIME / _FILETIME.
struct _SYSTEMTIME; struct _FILETIME;
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;
public: CDateAndTime(const _SYSTEMTIME& time); _FILETIME ToFileTime() const;};
#elif !defined(FABLE_RETAIL_CDATEANDTIME_CTOR)
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;};
#else
// Retail opt-in (row 0x00A60D40): ??0CDateAndTime@@QAE@XZ is a real out-of-line ctor (0x009E9AB0) that retail calls.
// Also chunk 2013, row 0x00597DF7 CFrontEndManager::GetSavedGameNames: vector<CDateAndTime>::resize 0x0048623F and every empty save
// slot call it. Same layout; the copy stays the implicit one.
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;
public: CDateAndTime();
#ifdef FABLE_RETAIL_CDATEANDTIME_TOFILETIME
// Nested opt-in (chunk 2013, row 0x00595CC1): Ego_r.pdb / FableWin.pdb ?ToFileTime@CDateAndTime@@QBE?AU_FILETIME@@XZ, out of line in
// retail (0x009E9D40; reloc_bindings.tsv, bound by the landed 0x0047EEE4; hidden result slot pushed at 0x00595DBA).
_FILETIME ToFileTime() const;   // 0x009E9D40
#endif
};
#endif
#endif
