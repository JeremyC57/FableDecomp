# 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate (bwf2 author, 2026-11-12) -- RELOCATION_MATCH (strict headers), test PASS, addmatch done
- What made it match: whole function written from OUTLINE.md in one go (call sequence matched retail at the first grade except inline
  decisions); then (1) CPlayer.h included before CThingPlayerCreature.h so the inline CPlayer::GetPPlayerCharacter body is defined
  (it is guarded by FABLE_ENGINE_CPLAYER_H); (2) CEnginePrimitiveHandle::Clear as FABLE_FORCEINLINE (new overlay opt-in
  _CLEAR_FORCEINLINE; c2 inline trace: plain inline Clear scores 74..108 against threshold 74 at static frequency 1/4 and 1/8, and no
  /d2inlT works because IS_THING_ALIVE(PContainer) #1 scores 103 and must stay a call); (3) `ForceClear = false; Amount = 0;` written
  before the two box copies; (4) `long gold = 0;` before the hero lookup; (5) `--FullSizeTimer;` before `OldItemRHSet = CurrentItemRHSet;`;
  (6) FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR (ctor body visible, c2 still calls 0x0041C930) with a plain `if (PSoundManager)`: PSoundManager is
  then read once into edi and the GainFaderBits store stays ahead of the Get2DEmitter vtable load. RESET_ADOPT is defined (harmless).
- Headers/flags: no Flags line, no pragma. Overlay $W/hdr (see hdr/README.md). Defines: the scout stub list + FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT,
  _CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE, _CSOUNDINIT_INLINE_CTOR; engine/GFLimit.h left out (GFLimit<MMM> declared body-less, call 0x0048DAE0).
- Call targets verified against retail (123 direct calls, const v non-const LowerBound included) with work/cseq.py.
- fg.py false hunks: `mov [esp+x], 0` lines show as hunks because ours has 00000000 bytes (norm() turns 0 into REL); ignore them.
- NAME_FIX (from the dossier, not re-proved here): 0x004BBE10 is ?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z (landed leaf `Set`).
