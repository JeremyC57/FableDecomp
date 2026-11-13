#ifndef FABLE_ENGINE_CXBCONFIG_H
#define FABLE_ENGINE_CXBCONFIG_H
// CXBConfig (new, chunk 2013, row 0x00595CC1 CFrontEndManager::ConstructFileDescription): only the static date formatter is modelled.
// Ego_r.pdb / FableWin.pdb ?FormatDateTime@CXBConfig@@SIXABU_FILETIME@@PA_W1H@Z (public static __fastcall: the time in ecx, the date
// buffer in edx, the time buffer and the flags pushed; `ret 8`), out of line in retail at 0x00A0D980 (it zeroes both buffers for a
// zero FILETIME). Wide text is unsigned short on the lane (no native wide-character type), so it binds ...PAG1H@Z. The landed 0x0047EEE4 binds it
// as a free ?FormatDateTime@@YIXABU_FILETIME@@PAG1J@Z (NAME_FIXES.tsv, chunk 2013).
#ifndef FABLE_RETAIL_WIN32_FILETIME
#define FABLE_RETAIL_WIN32_FILETIME
#endif
#include "engine/Win32Types.h"
class CXBConfig {
public:
    static void __fastcall FormatDateTime(const _FILETIME& time, unsigned short* date_text, unsigned short* time_text, int flags);   // 0x00A0D980
};
#endif
