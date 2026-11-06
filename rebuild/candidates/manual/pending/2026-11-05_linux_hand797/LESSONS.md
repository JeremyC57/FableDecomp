# Chunk 797 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- How you bind a CScriptThing changes how IsAlive is called (0x00DA91A0).
  - `CScriptThing& x = Game->GetNearestWithScriptName(...)` binds the hidden-return temporary to a reference, and VC7.1 keeps its IsAlive calls virtual (`mov eax,[esp+N] ; call [eax+0x12c]`), as retail does.
  - A by-value `CScriptThing x = ...` lets VC7.1 devirtualise both calls into direct `call`s.
- The shape of a virtual call's `this` decides whether VC7.1 hoists it (0x00DA91A0). VC7.1 hoists a shared `lea ecx,[esp+N]` above a `jne` into an exit block that starts with the same lea only when the fall-through starts with that lea. A virtual call starts `mov reg,[esp+N]` first, so no hoist; a direct call starts with the lea, so it hoists.
- In 0x00DADF80, `GetIndex(v.size())` and `GetIndex(v.end() - v.begin())` compile differently on an STLport vector member reached through a reference.
  - `.size()` keeps a dead `add reg,begin` (the vector `this`) and puts the parent in eax.
  - `end() - begin()` puts the parent in ecx and folds the begin load into `sub eax,[ecx+begin]`.
  - Retail is the first with the second's registers (parked).
- In 0x00D18DF0, a user-declared empty dtor, `__forceinline` included, resets the class's own vtable at every expanded site. A compiler-generated dtor does not. When retail expands a MovieBase dtor as `call PImp-dtor ; mov [x],GROBase-vtable ; call ~CBaseObject` with no MovieBase vtable store, the inlined dtor is the implicit one.
- In a 3.3 KB body (0x00D18DF0), VC7.1 calls the COMDAT copies of the implicit MovieBase ctor and dtor (0x006E7B60 / 0x006E7B80) where retail expands them.
  - A `__forceinline` empty ctor fixes the ctor sites and the frame.
  - Declare `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` before the movie header, or C2908 follows.
- In 0x00F0CAA0, `CScriptThing::GetPos` written as a ternary is not inlined. `if (PImp.IsNull()) return NullCoord; return PImp->GetPos();` is inlined, as in the landed precedent.
- The Ego_r.pdb CQ_NewOakValeIntroScript places its two C3DVector members at odd offsets (138 / 153) while the longs stay 4-aligned. The class is byte-packed (`#pragma pack(push, 1)` in the header) with the alignment gaps written out.
- tall.py (VC7.1 test build) fails TCC on tests that use std::string. Write behaviour tests with char buffers and strcat logs.

Review at landing:
- Grade counts: 00DA91A0 took 10 grades, 00DADF80 10 and 00D18DF0 14 (all parked).
- 00E18630 took 4.
- 00E18630 and the parked 00DA91A0 / 00DADF80 use TU-level `__forceinline` bodies:
  - CScriptThing::operator= and ScriptedThingBase::operator= (00E18630)
  - CTimer::GetTime (00DA91A0)
  - plain inline CTimer::GetTime / operator=(long) (00DADF80)
  
  The same spellings are in 16, 1 and 18 landed sources.
- Already-landed misnames:
  - 0x00D15140 (CMainGameComponentInit, really TeleportData of CQ_BountyHuntScript)
  - 0x00F0D470 (SScriptQuery_00f0d470)
  - the local helper `CLandscapeBackgroundPatch_dtor_00cdbc50` in 0x00CDBD00
  - the unprefixed nested class names in CQ_NewOakValeIntroScript::Main 0x00DABAC0
  - AddBadDeed(long) at 0x00DAEA70 and its caller 0x00DBCD00
  
  The fixes are in NAME_FIXES.tsv.
- engine/CScriptGameResourceObjectMovieBase.h FORCEINLINE opt-in: only the parked 00D18DF0 uses it, and its dtor half is the residue (hdr/README.md).
