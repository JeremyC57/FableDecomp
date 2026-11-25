// CAnimInit::CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority) -- Frame zeroed, then Init with the seven arguments
// Leaf: ??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B33F4, length 49 bytes, thiscall `ret 1Ch`.
// Identity (agentNF54 name fix; was the descriptive stand-in CRumbleEffect_005b33f4::Initialise over a local CRumbleEffectBase): FableWin.pdb
//   0x01D8BC30 ??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z, the inline argument constructor of engine/CAnimInit.h
//   (FABLE_RETAIL_CANIMINIT_CTORS): the member CAnimFrameInfo Frame (+0x18 Current / +0x1C Last) is built first (`fldz ; fstp` x2,
//   the "zero two floats before the base call" the old note read as ruling out a constructor), then the body calls
//   ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z 0x0070C050 (Ego_r.pdb / FableWin.pdb) with the seven arguments, and `this` is
//   returned. Its callers build CAnimInit temporaries for animation requests (CTCCreatureExpression::UpdateBlinking 0x00728FF0,
//   CModeStandBase::UpdateIdleAnim 0x00862780, both passing an anim index and the entry's GetAnimateThingFlags()).
// Shape: the size-optimised frame-pointer TU (`optimize("s", on)`) that emits the inline constructor out of line; the
//   header's CTOR_OUTOFLINE opt-in (agentNF54 overlay) declares it and this row defines it with the header's body.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: this row's object is byte-identical without it (size mode keeps the
//   leaf's frame; T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMINIT_CTOR_OUTOFLINE
#include "engine/CAnimInit.h"

CAnimInit::CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority)
{
    Init(type, typeFlags, anim, updateMode, looping, flags, priority);
}
