#ifndef FABLE_SCRIPT_TEXT_H
#define FABLE_SCRIPT_TEXT_H
// Original PUBLIC32 names in both PDBs; native 0099f830 and 0099f690.
#include "fable_string.h"
CCharString FABLE_FASTCALL GFIntToCharString(long);
CCharString FABLE_FASTCALL operator+(const char*,const CCharString&);
CCharString FABLE_FASTCALL operator+(const CCharString&,const CCharString&); // Original PDB overload; native0099f570.
#ifdef FABLE_RETAIL_GFFLOATTOCHARSTRING
// e01 opt-in (rows 0x007DA400 / 0x007DA520): Ego_r.pdb / FableWin.pdb ?GFFloatToCharString@@YI?AVCCharString@@NJ@Z (0x009A08B0; Ego_d
// parameter names n / precision; fastcall: hidden result in ecx, precision in edx, the double on the stack). Declaration only.
CCharString FABLE_FASTCALL GFFloatToCharString(double n, long precision);   // 0x009A08B0
#endif
#endif
