# NF122 overlay headers

Every opt-in below is a declaration inside a new `#ifdef` block with a member-specific macro. Without the macro each header is
token-identical to its repo copy (orig/ holds the base copies, byte-equal to the repo at the start of this round).

## engine/NScreenEffect_CScreenEffectPriorityStack.h
- Copy of rebuild/include/engine/NScreenEffect_CScreenEffectPriorityStack.h (base in orig/engine/).
- Adds `FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_DELETESCREENFILTERENTRY` in the default (non-FULL) class, right after
  ScreenFilterSetFlags: `void DeleteScreenFilterEntry(void* handle, float fadeTime);` (0x008818A0, Ego_r 0x008C2430 / FableWin
  0x02A59240 ?DeleteScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEXPAXM@Z). Declaration only, no layout change.
- pin_asserts.py: `engine/NScreenEffect_CScreenEffectPriorityStack.h pinned 0` (the header has no FABLE_STATIC_ASSERT lines).
- Landed includers (grep of rebuild/src/compiled for this header or engine/CEnvironmentSpellEffectInterpolator.h, which includes it):
  0x0079C880 0x0079D7E0 0x0079DC80 0x0079CF30 0x0079D7C0 0x00881A40 0x00883D20 0x00882EF0 0x00882000 0x008826E0 0x00881690
  0x00880F50 0x00883860 0x006B3FF0 0x006B83E0. None defines the new macro, so the opt-in changes none of them.
- NF120 / NF121 overlays have no copy of this header (checked). Used by rows/006b83f0.cpp.

## engine/CTCRegionFollower.h
- Copy of rebuild/include/engine/CTCRegionFollower.h (base in orig/engine/).
- Adds, after the FABLE_RETAIL_CTCREGIONFOLLOWER class block, a partial mode
  `#if defined(FABLE_RETAIL_CTCREGIONFOLLOWER_FOLLOWERS_ONLY) && !defined(FABLE_RETAIL_CTCREGIONFOLLOWER)`: `class CTCRegionFollower` with only
  `void AddFollower(CThing&);` (0x006AEDC0) and `void RemoveFollower(CThing&);` (0x006AEBA0), no base, no members (name binding only;
  round-9 rule: a partial shared-header mode that lets a row drop a local declaration). Ego_r / FableWin
  ?AddFollower@CTCRegionFollower@@QAEXAAVCThing@@@Z and ?RemoveFollower@CTCRegionFollower@@QAEXAAVCThing@@@Z.
- Used by rows/0048a070.cpp, rows/009212c0.cpp, rows/008a0b10.cpp (each still models CThing / ETCInterfaceType locally, so they cannot
  include engine/CTCBase.h, which the full mode needs).
- pin_asserts.py: `total pinned 0` (no FABLE_STATIC_ASSERT lines in this header).
- Landed includers: 0x004A3740 0x006AED80 0x006D53C0 0x009609B0. None defines FOLLOWERS_ONLY (and the block is skipped whenever
  FABLE_RETAIL_CTCREGIONFOLLOWER is defined), so the opt-in changes none of them. NF120 / NF121 have no copy of this header (checked).

## engine/CGameTimeManager.h
- Copy of rebuild/include/engine/CGameTimeManager.h (base in orig/engine/).
- Adds `FABLE_RETAIL_CGAMETIMEMANAGER_ISDAY` in the FABLE_RETAIL_CGAMETIMEMANAGER class (after the ISTIMEBETWEEN opt-in, before
  `private:`): `bool IsDay() const;` (0x006BB2C0; Ego_r 0x006F6A60 / FableWin 0x02460170 ?IsDay@CGameTimeManager@@QBE_NXZ).
  Declaration only, no layout change.
- pin_asserts.py: `engine/CGameTimeManager.h pinned 0` (its one FABLE_STATIC_ASSERT, in the FULL branch, sits above the insertion and
  did not move).
- Landed includers: 006414a0 00641c30 005ce0e6 004a21f0 004a6e30 007448f0 00957f20 006d0830 006d1e30 00946690 0065be6c 0065aa6f 0097fa00 0072f0c0 0072f0d0 0072f170 0049f4c0 006c1250 005d1f0c 00918880 0088fe40 0092c520 0092d600 0092c560 0092da20 0092c4e0 
  None defines the new macro, so the opt-in changes none of them. NF120 / NF121 have no copy of this header (checked).
- Used by rows/006bb2c0.cpp, rows/0065c781.cpp, rows/00893ec0.cpp, rows/00911f80.cpp, rows/00946630.cpp.

## engine/CTCDParticleEmitter.h
- Copy of rebuild/include/engine/CTCDParticleEmitter.h (base in orig/engine/).
- Adds a partial mode as a new `#elif defined(FABLE_RETAIL_CTCDPARTICLEEMITTER_METHODS_ONLY)` arm between the FULL branch's
  `FABLE_STATIC_ASSERT(sizeof(CTCDParticleEmitter)==72);` and the default `#else`: `class CTCDParticleEmitter` with only
  `static CThing* __fastcall Create(long, long, float, const C3DVector&, bool);` (0x006E11F0), `SetEmitterOrientation(const CRightHandedSet&)`
  (0x006E0480), `SetAlwaysUpdate(bool)` (0x006E0590) and `AttachToThing(CThing&, long, const CCharString&, long, float)` (0x006E0BE0),
  no base, no members (name binding only; Ego_r / FableWin spellings in the header comment). Without the macro the default arm is
  taken exactly as before (token-identical; FULL still wins when defined).
- Used by rows/0089f910.cpp, rows/0089f9e0.cpp, rows/0080d0b0.cpp (each models CThing / ETCInterfaceType locally).
- pin_asserts.py: `total pinned 0` (the header's only assert sits above the insertion).
- Landed includers: 008c0fb0 008c4b80 008c2ad0 00750e30 007a1b20 0087f3a0 007c7c40 007c1000 007ccf50 007c4430 007cce00 007c6e50 007cd800 007cd390 007ce7a0 008e16a0 008e0a10 008e0060 008e3760 008e24f0 008e20f0 008e3770 007b0360 006e1d50 006e1d40 006e1d30 007f24e0 007f26b0 007f2dc0 007f3ad0 007f23d0 007941d0 0079cc80 004a3740 00766660 00740730 00742f00 00742e40 008daa80 008da410 008dc720 008ddad0 008f3630 008f7950 006dfe50 008548b0 0072e130 00722740 006b3ff0 00735fd0 008b9f20 008b9e50 004c9920 
  None defines METHODS_ONLY, so the opt-in changes none of them. NF120 / NF121 have no copy of this header (checked).
