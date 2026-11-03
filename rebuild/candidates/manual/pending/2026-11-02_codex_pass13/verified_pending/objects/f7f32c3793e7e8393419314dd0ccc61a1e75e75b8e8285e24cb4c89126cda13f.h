#ifndef FABLE_SCRIPT_TEXT_H
#define FABLE_SCRIPT_TEXT_H
// Original PUBLIC32 names in both PDBs; native 0099f830 and 0099f690.
#include "fable_string.h"
CCharString FABLE_FASTCALL GFIntToCharString(long);
CCharString FABLE_FASTCALL operator+(const char*,const CCharString&);
CCharString FABLE_FASTCALL operator+(const CCharString&,const CCharString&); // Original PDB overload; native0099f570.
#endif
