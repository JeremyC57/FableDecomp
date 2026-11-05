# Chunk 782 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Frame slots follow scope.** In 0x007941D0, the frame (0x24) and the slot sharing came only from mixing scopes:
  - small / large ring indices at function scope, zeroed before the escaping GetSubDef call;
  - the hand / dark-faces indices in their own `{}` blocks, so they take the large slot and their temporaries take the small slot.

  All locals at function scope give a frame of 0x28; every index in a block gives 0x20. When the built frame is 4 bytes off,
  re-scope the address-taken locals before trying anything else.
- **Make member-template bodies visible.** Write a member template's body (e.g. `CDefClassBase::GetSubDef<T>`, the landed
  0x00669580 body) in the TU whenever retail's TU would have seen it from the header. VC7.1 then treats the `&effects` out-pointer
  as uncaptured and keeps the def in a register across later calls (0x007941D0 edi). Declaration-only, it re-reads the slot at
  every use and the CDefPointer release turns into load / dec / store.
- **Same-TU noinline body for out-param callees.** When a non-template out-of-line callee takes a local's address
  (CSpecialEffectsDef::GetEffectFromString 0x00669D30), an `inline __declspec(noinline)` same-TU body in the shared header (opt-in)
  lets VC7.1 keep that local in a register afterwards. Retail 0x0079476B holds the hand index in edi for both CreateParticleEffect
  calls (EXTRA_RULES chunk 422 confirmed again, no pragma).
- **Unconditional GetTC.** `CTCDParticleEmitter* e = 0; thing->GetTC(TCI_PARTICLE_EMITTER, &e); e->Call()` gives retail's
  `xor reg,reg` before the HasTC test and an unconditional call. Adding `if (e)` gives the `cmp ecx,ebx; je` form; 0x007941D0 has
  both, the right hand null-checked and the left hand not.
- **Two GetTC lookup shapes in one TU.** `HasTC(id)` + the forced-inline `GetTC(ETCInterfaceType)` reference (Components.Find
  straight on the map: `lea ecx,[edi+44h]`, end read `[edi+48h]`, constant key compare) coexists with the
  FABLE_RETAIL_CTHING_GETTC_EXPANDED pointer-local `GetTC(id, &p)` / `PeekTC(id, &p)` shapes (`add edi,44h ... mov edi,[edi+4]`).
- **Inline-budget site choice.** VC7.1's inline decisions per call site for `~CScriptThing` / `~vector` / `~CTimer` at many exits
  (0x00DFD600) could not be steered by inline / __forceinline / out-of-line spellings. Retail expands `~CScriptThing` at exactly
  one site (the break) and merges the trader-alive returns into one exit block. Parked as an inline-budget residue.

Review at landing:
- 0x007941D0 needed 8 grades (more than 4): fixing the frame took scope changes and two same-TU bodies (see STATUS.tsv a..h).
- Local models next to missing headers: CTCSpecialAbilities, CTCDParticleEmitter and CTCUnholyPower are row-local in 0x007941D0
  because no engine header exists. No local model sits next to an existing header.
- Errors in already-landed sources:
  - rebuild/src/compiled/00/79/CTCDivineWrath_PlaySoundW_007940f0.cpp is really CTCUnholyPower::PlaySoundW(const CCharString&, bool);
    its class and parameter types are wrong (NAME_FIXES.tsv).
  - CCRC_CalcAndLookup_00669d30.cpp is CSpecialEffectsDef::GetEffectFromString.
  - CRetailVectorCScriptThingRetail00dfde30_*.cpp is a placeholder for vector<CScriptThing>'s copy ctor.
  - The landed callers of 0x0072AB70 / 0x006E11F0 / 0x006E0BE0 / 0x006E0590 / 0x006B3800 / 0x00669580 / 0x009AD410 use stand-in
    spellings (NAME_FIXES.tsv).
- Catalog: 0x00B5F090 and 0x00E00DB0 carry other functions' names (STANDIN_LEAF warnings in vland; LABELS.tsv).
