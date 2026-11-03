#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H
// Ego_r TPI CDefString class, size4, private TablePos+0; native parent-def
// constructors initialise this member at +25 to -1.
class CCharString;
#ifndef FABLE_RETAIL_CDEFSTRINGTABLE
#error engine/CDefString.h needs the retail CDefStringTable: define FABLE_RETAIL_CDEFSTRINGTABLE (and FABLE_RETAIL_CCHARSTRING) before including it
#endif
#include "fable_definition_table.h"
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
#ifdef FABLE_RETAIL_CDEFSTRING_GETSTRING
 // Retail opt-in (2026-11-03, chunk 720 row 0x0065B854 CTCVillage::AddThingToVillage): GetString is out of line in retail
 // (0x00415DD0 ?GetString@CDefString@@QBE?AVCCharString@@XZ; FableWin.pdb donor order puts ??B 0x018C20E0 = retail 0x00415D70
 // before GetString 0x018C2120 = 0x00415DD0, the Codex pass 12 respell), and the inline conversion forwards to it: the track-name
 // compare (the track node's TrackName against the def's PatrolTrackName) builds the conversion temporary at [ebp-4], calls 0x00415DD0 and re-reads
 // the temporary from its slot (0x0065B951..0x0065B95C), the implicit-conversion shape; an explicit GetString() call reuses eax.
 operator CCharString()const{return GetString();}
 CCharString GetString()const;                                       // 0x00415DD0
#else
 operator CCharString()const{return GDefStringTable.GetString(TablePos);}
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#endif
private:
 long TablePos;
};
#endif
