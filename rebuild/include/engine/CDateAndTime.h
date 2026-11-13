#ifndef FABLE_ENGINE_CDATEANDTIME_H
#define FABLE_ENGINE_CDATEANDTIME_H
// Original Ego_r TPI and frontend destructor: trivial28-byte element.
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
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;
public: CDateAndTime();};
#endif
#endif
