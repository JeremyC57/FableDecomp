#ifndef FABLE_ENGINE_GFDEBUGPDYNAMICCAST_H
#define FABLE_ENGINE_GFDEBUGPDYNAMICCAST_H
// FableWin.pdb ??$GFDebugPDynamicCast@...@@YI... (one debug COMDAT per From / To pair, e.g. 0x0189CDB0 CTCBase* -> CTCInGameMenu*): the
// checked pointer cast of the debug build. The release path is the plain static cast (no retail copy; every caller expands it).
// Its own header so that rows outside the CThing family share the one definition (engine/CThing.h includes it).
#ifdef FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT
// Retail opt-in (wfB3, row 0x00BA2350): the debug body's one ASSERT kept as the release dead arm (fable_assert.h). FableWin.pdb's
// GFDebugPDynamicCast COMDATs are ~90 /Od instructions longer than the cast alone (tools fwsize "assert?" gap = one expanded ASSERT);
// the dead arm emits no bytes but its record ids order the row's inline SetRenderState operands (measured: NOTES.md wfB3).
// Inferred: one assert; its argument (the cast value) is FITTED.
#include "fable_assert.h"
template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { FABLE_RETAIL_ASSERT(value); return static_cast<To>(value); }
#else
template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
#endif
#endif
