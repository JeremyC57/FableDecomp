#ifndef FABLE_ENGINE_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_H
#define FABLE_ENGINE_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_H
// Retail model (hand-written 2026-10-30, class-model migration wave 5; opt-in FABLE_RETAIL_BASE_OBJECT). RTTI
// .?AVCScriptGameResourceObjectMovieBase@@ : CScriptGameResourceObjectBase (3 bases). Vtable 0x01260EF4, 2 slots: slot 0
// scalar deleting dtor 0x006E7BD0, slot 1 inherited CBaseObject::Validate 0x0099A400 (the next dword is data).
// Layout (retail bytes, Ego_r.pdb agrees, 0x10): PImp CCountedPointer<CScriptGameResourceObjectMovieBase> +0x08 (Data) /
// +0x0C (Info): the ctor 0x006E7B60 zeroes [esi+8] / [esi+0xC]; the dtor releases the Info at +0xC.
// Members: ctor 0x006E7B60 (`call 0x0099A380; mov [esi],0x01260EF4; zero +8/+C`: the GROBase ctor inline), dtor 0x006E7B80
// (PImp's CCountedPointer dtor inline, then the GROBase dtor inline), Release 0x00CF8850 (`add ecx,8; call
// CCountedPointer::Reset(0) 0x008AB2B0`), StartMovieSequence takes it by reference (CGameScriptInterface slot 370).
// Ctor and dtor are the implicit ones: retail expands both inline at most landed call sites (the ctor's vtable store
// 0x01260EF4 and the dtor's CCountedPointer release 0x006E7AB0 + GROBase vtable store), and calls the COMDAT copies
// 0x006E7B60 / 0x006E7B80 at others; the PDB spellings ??0..QAE@XZ / ??1..UAE@XZ are the implicit members' manglings.
// Contract: the includer defines CCountedPointer<T> before the include (PImp is by value; its inline bodies are the row's).
// Per-site expansion (wave 8, scripts/mbexp.py + mbsites.py): retail destroys a MovieBase three ways, often mixed in one
//   function: fully expanded, expanded with a call to the PImp dtor 0x006E7AB0, or a call to this dtor 0x006E7B80. With the
//   implicit dtor and a plain inline (not __forceinline) CCountedPointer release, VC7.1 itself gives the retail mix in 15 of
//   the 24 rows that forced it with #pragma inline_depth (byte-identical; 14 kept, wave 8 family MBX). unproven: why the
//   single-site rows that call 0x006E7AB0 (e.g. 0x00DF2D30) are not expanded: VC7.1 expands them fully (385 v 349 bytes).
// unproven: SetImplementation (both PDBs: ?SetImplementation@CScriptGameResourceObjectMovieBase@@QAEXPAV1@@Z). Candidate
// 0x008AB970 `add ecx,8; jmp 0x008AB2B0` (the MovieBase CCountedPointer reset Release calls; the STB twin 0x008AB960 is
// bound as STB::SetImplementation) is unbound, so the address is not claimed here.
#ifndef FABLE_RETAIL_BASE_OBJECT
#error engine/CScriptGameResourceObjectMovieBase.h is the retail CBaseObject-family model: define FABLE_RETAIL_BASE_OBJECT first
#else
#include "rebuild_abi.h"
#include "engine/CScriptGameResourceObjectBase.h"
template <class T> class CCountedPointer;
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase {
public:
    void Release();                                                      // 0x00CF8850
    void SetImplementation(CScriptGameResourceObjectMovieBase*);         // unproven: candidate 0x008AB970
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;            // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CScriptGameResourceObjectMovieBase) == 0x10);
#endif // FABLE_RETAIL_BASE_OBJECT
#endif // FABLE_ENGINE_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_H
