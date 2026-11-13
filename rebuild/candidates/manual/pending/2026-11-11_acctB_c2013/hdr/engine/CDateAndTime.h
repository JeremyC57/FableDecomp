#ifndef FABLE_ENGINE_CDATEANDTIME_H
#define FABLE_ENGINE_CDATEANDTIME_H
// Original Ego_r TPI and frontend destructor: trivial28-byte element.
#ifdef FABLE_RETAIL_CDATEANDTIME_TOFILETIME
#ifndef FABLE_RETAIL_WIN32_FILETIME
#define FABLE_RETAIL_WIN32_FILETIME
#endif
#include "engine/Win32Types.h"
#endif
#ifdef FABLE_RETAIL_CDATEANDTIME_CTOR
// Opt-in (chunk 2013, row 0x00597DF7 CFrontEndManager::GetSavedGameNames): Ego_r.pdb / FableWin.pdb ??0CDateAndTime@@QAE@XZ, out of
// line in retail (0x009E9AB0, zeroes the seven dwords; called by vector<CDateAndTime>::resize 0x0048623F and for every empty save
// slot at 0x00597F19 / 0x00597FBA / 0x005981A5 / 0x005982CB / 0x0059836F). Same layout; the copy stays the implicit one (rep movsd).
class CDateAndTime {public:CDateAndTime();
#ifdef FABLE_RETAIL_CDATEANDTIME_TOFILETIME
// Nested opt-in (chunk 2013, row 0x00595CC1): Ego_r.pdb / FableWin.pdb ?ToFileTime@CDateAndTime@@QBE?AU_FILETIME@@XZ, out of line in
// retail (0x009E9D40; reloc_bindings.tsv, bound by the landed 0x0047EEE4; hidden result slot pushed at 0x00595DBA).
_FILETIME ToFileTime() const;   // 0x009E9D40
#endif
protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;};
#else
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;};
#endif
#endif
