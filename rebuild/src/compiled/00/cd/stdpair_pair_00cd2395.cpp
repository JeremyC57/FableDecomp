// std::pair<int const, CCharString>::pair(const pair&) -- STLport 4.6 _pair.h copy constructor
// Leaf: ??0?$pair@$$CBHVCCharString@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2395, 29 bytes (speed TU): the first member copied inline, then CCharString(const CCharString&) 0x0099EC30; caller 0x00CD2C5B (NF61 _M_create_node).
// NF64 (round 7, q10680): an explicit instantiation of the stlport/fable_containers.h pair constructor (STLport 4.6 _pair.h); the landed
//   caller binds this spelling (see the entry). Was the generated stand-in `SPairLongCSecond_0099ec30::SPairLongCSecond_0099ec30`. Descriptive-by-library (retail is STLport 4.6; both
//   PDBs are Dinkumware builds).
#include "fable_string.h"
#include "stlport/fable_containers.h"
template std::pair<int const, CCharString>::pair(const std::pair<int const, CCharString>&);
