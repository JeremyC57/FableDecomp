// CGameEventDataPackage::GetUHUGE() -- reads the next eight bytes of the unpacked event data and returns them as an unsigned 64-bit value (edx:eax)
// Leaf: ?GetUHUGE@CGameEventDataPackage@@QAE_KXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00628E80, length 25 bytes, thiscall, plain `ret`.
// Name: FableWin.pdb 0x023D5E10 ?GetUHUGE@CGameEventDataPackage@@QAE_KXZ: of the package's typed readers (FableWin GetULONG / GetUBYTE /
//   GetFloat / GetSLONG / GetUHUGE / GetUWORD; none in Ego_r.pdb, which inlines them) it is the only 8-byte one, and the body is
//   GetData(8, &value) on `this` returning the 64-bit value in edx:eax. Callee ?GetData@CGameEventDataPackage@@QAEXJPAX@Z 0x009F18F0
//   (Ego_r.pdb 0x00B454F0 / FableWin.pdb 0x030A2200; engine/CGameEventDataPackage.h). NF95 2026-11-19 (q11320): was the stand-in static
//   CGameEvent::ReadData8(CGameEvent*) returning a two-word struct, calling a made-up CGameEvent::ReadBytes (catalog label AddUBYTE,
//   propagated). No landed caller (no direct reference in .text).
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETUHUGE
#include "engine/CGameEventDataPackage.h"
unsigned __int64 CGameEventDataPackage::GetUHUGE()
{
    unsigned __int64 value;
    GetData(8, &value);
    return value;
}
