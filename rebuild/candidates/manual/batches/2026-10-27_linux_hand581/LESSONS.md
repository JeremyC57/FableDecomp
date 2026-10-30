# Chunk 581 lessons

GOTCHAS one-liners:
- A CScriptThing slot wrapper that retail expands inline (e.g. GetPos with the g_ZeroVector null arm) needs `__forceinline` when defined out of class after the shared CScriptThing.h; a plain `inline` stays a direct call (0x00E1F0C0).
- The retail CScriptThing temporary's inline destructor and the inline member assignment `Exit = Game->...` come from `__forceinline CScriptThing::~CScriptThing() { }` and `__forceinline operator=` { PImp = other.PImp; } after the header, plus an includer CCountedPointer with `__forceinline` release dtor and source-first operator= (0x00EC1CD0).
- `(1.0f - scale) * (int expr)` gives `fild/fmulp`; retail's `fimul [esp+x]` needs the int in a named local (`long gold = ...;`) and the def pointer read into a local before rand() (0x00EC1CD0, as 0x00E1FEB0).
- Wait loops on a local CScriptThing: `((const CScriptThing*)&maze)->IsAlive()` keeps the vcall and the hoisted `lea ecx` (0x00E268C0, precedent 0x00E06050).
- A spawned watcher whose loop exits go on to a shared tail: an unrotated `do { ...; NewScriptFrame(); } while (!IsActiveThreadTerminating()); return; finished:` with `goto finished` exits (0x00E99260).
- vector<T>::_M_fill_insert for a def whose element copy ctor is IMPLICIT (CArenaWaveDef, CHeroSoulsCreatureDef): VC7.1 inlines the copy at all 3 sites; retail inlines only x_copy and calls it out of line in the raw-end loops. inline_depth 0-4, auto_inline(off), a user inline copy ctor, a same-TU out-of-line copy, plain-inline __uninitialized_copy and _Construct variants, and LTCG do not reproduce this (0x00F281E0 / 0x00F277F0 parked).
- 0x00F33EF0 (implicit CFireHeartPatternDef::operator=) matched only after the wave-1 CDefClassBase.h declared operator= 0x00431F10. Without it the implicit operator= is inlined (629v538).

Review at landing:
- These rows have local stand-ins for classes with a retail vtable:
  - CScriptDef (opaque, 0x00EC1CD0); pGlobalScriptDef is a plain pointer, really CDefPointer<const CScriptDef>.
  - CQ_SunnyvaleMasterData (opaque, 0x00E99260).
  - CSpawnedFunc<CQR_EscortTraderScript> (local template, vtable 0x012E12F4, 0x00E1F0C0).
- NAME_FIXES.tsv lists 6 addresses. Respell the landed 0x00EC1EF0 Main (Leaf line `Main`, global struct), 0x00EFF1C0 AddQuestsToGuild and 0x00E1F3A0 EscortTrader into NScript. Rebind 0x00EC1B70 (CRandomPopulationEntity) and the junk 0x00CB7940 / 0x00CB7E50 spellings.
- The landed OnPredicateFail 0x00EC1C20 comment calls type 2 a guard; Init proves 1 = guard, 2 = bandit.
- 0x00EFF130 was converted from a local model to the shared NScript headers after the wave-1 note. The old local version is in probe/ only.
- vland (read-only) has 8 wins, no RELOC_IDENTITY, and only RELOC_NAME warnings where catalog labels are wrong:
  - 0x00CB8930 is CScriptBase::PostAddScriptedEntities, not CTCVillage::OnInitialActivate.
  - 0x00F25DD0 / 0x00F25F10 are the CArenaCreatureDef copy ctor / operator=.
  - 0x00F26740 / 0x00F34830 are std::uninitialized_fill_n.
  - 0x00F34860 is vector<CFeatDef>::_M_insert_overflow.
  - 0x00462E78 is vector<bool>::operator=.
