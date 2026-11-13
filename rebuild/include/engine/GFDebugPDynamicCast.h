#ifndef FABLE_ENGINE_GFDEBUGPDYNAMICCAST_H
#define FABLE_ENGINE_GFDEBUGPDYNAMICCAST_H
// FableWin.pdb ??$GFDebugPDynamicCast@...@@YI... (one debug COMDAT per From / To pair, e.g. 0x0189CDB0 CTCBase* -> CTCInGameMenu*): the
// checked pointer cast of the debug build. The release path is the plain static cast (no retail copy; every caller expands it).
// Its own header so that rows outside the CThing family share the one definition (engine/CThing.h includes it).
template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
#endif
