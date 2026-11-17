# NF55 lessons (round 7)

- Check the interface number before trusting a stand-in's class name: CTCThunderCombat (0x0096C4F0) and MazeBoss (0x009547D0,
  0x00954E30) were all interface 0xad = 173 = CTCBoss (engine/CTCBoss.h StatGetInterfaceType). The method names then follow from
  the Ego_r run of that class (GetBossPhase .. ResetStrikesSinceHitHero at retail 0x007AED10 .. 0x007AED50).
- Retail placement = PDB source order is a cheap second source for a self-row rename: look at the two landed neighbours and the
  Ego_r / FableWin donor run (GetFOV between GetTimeBasedVisibilityMultiplier and the ctor; FadeOrKillEffect(CThing*) after the
  CIntelligentPointer& overload; IsAnimationFinished between GetMeshBank and AnimsCanBeIgnored).
- A binder's own PDB signature settles overload claims: CThingFilter_IsInTargetingRangeAndFOV is QBE_NABVCThing (const CThing&),
  so the IsInFOV call inside it is the const CThing& overload whatever FableWin's ordering suggests.
- Flat-branch opt-ins are the low-cost fix when a row uses a header's flattened (default) branch and only needs one method:
  CTCCombat FLAT_RESETCOMBO, CTCPerceiveThing FLAT_GETFOV. Put the opt-in right after the class's first `public:`.
- When a shared header lacks only a static helper (CTCSpecialAbilities::FadeOrKillEffect), including it under its existing
  opt-ins (FULL + TURNCOAT) next to a row's other local models works as long as the header does not pull a class the row models
  locally; CTCBase retail only forward-declares CThing. Replace a local `enum ETCInterfaceType { };` with engine/ETCInterfaceType.h
  BEFORE engine/CVectorMap.h, or CTCBase.h fails (C2146).
- CTCBoss.h needs engine/C3DVector.h first (else C2501 / negative-subscript static assert), and FABLE_RETAIL_C3DVECTOR_PACKED when
  the row reads PhaseStartPos (+0x22, not +0x24).
- STLport ports: `ContainerItems.clear()` calling the out-of-line range erase needs FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE;
  a push_back whose overflow call stays out of line with retail's register use needs FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE;
  STLport for_each is declared in stlport/fable_algorithm.h (__fastcall, hidden-pointer functor return). Both rows matched on the
  first grade, so these are worth trying before any same-TU stand-in body.
- A by-value inline converting ctor (CIntelligentPointer(T*)) evaluates an address-taken argument before the inlined base ctor;
  retail code that loads it after the base ctor came from a reference-taking spelling. If both PDBs say by value, park the row
  (0x00799900) instead of inventing a `T* const&` overload.
- Anonymous-namespace hashes (?A0x...) differ per TU and cannot be produced from source; a binder that differs from the claim only
  in the hash is not-a-binder (reloc_rename.py and EXTRA_RULES match by prefix).
- When a self-row rename changes access (QAE -> AAE/IAE) or constness, grep every landed caller: some bind through the shared header
  (already right), others through local models (NAME_FIXES), e.g. StopTurncoatingEffects (FrameUpdate vs OnKill / FinalCleanup).
- An expanded inline body re-defined in another row's TU (0x00745120 defines GetFOV) becomes a duplicate strong symbol once the
  out-of-line self row is re-authored under the real name: report it instead of renaming either.
- A row whose local input-process class repeats the base layout inline can derive from the shared retail
  engine/CInputProcessGameBase.h instead: drop the duplicated base members and the slot-0 placeholder virtual (CAInputProcess's
  virtual destructor is slot 0) and keep the later placeholders so slot numbers do not move (0x00687FD0, 0x006894C0: first grade).
- Before hand-back, run callers.py over every section-1 target: reloc_bindings.tsv keeps one caller per symbol, so a name grep
  misses landed callers that still bind a made-up spelling (0x00954410 GetCurrentPhase).
