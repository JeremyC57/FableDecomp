# chunk 852 notes (agent852)

All six rows are parked in differ/ with tests; none matched. Header debt: the two opt-ins in hdr/ (see hdr/README.md).
Callee symbols were checked against rebuild/oracles/reloc_bindings.tsv by hand (GetMagnitudeAndNormalise 0x00A14480,
CDefStringTable::GetString 0x009D49B0, IsStopping 0x008FD010, GetPDefinitionManager 0x008FCFB0: one symbol, one address);
the three row symbols themselves are not bound anywhere. No conflict found.

## 0x00D07E50 NScript::CQ_BanditCampBossBattleScript::CCampHostage::Main (name proven)
- Proof: the only .rdata reference is slot 1 of the CCampHostage vtable 0x012C8F8C (slot 0 sdd 0x00D07E30, 2 Init 0x00D07DD0,
  3 GetParentScript 0x00D07DC0, all landed under CCampHostage); Ego_r.pdb + FableWin.pdb ?Main@CCampHostage@...@@EAEXXZ.
- Shape: an alive-filter predicate (named `{ CIsThingAlive filter(Me); SetPredicate(filter); }` block plus a row-local inline
  ~CIsThingAlive: the temporary form calls the out-of-line dtor), the start-scripting loop, a CTimer shout loop with a 4-way
  switch (THIRD / FOURTH alternate after the first pass), then the hero-talk and guard-too-close SAY blocks under a speech movie
  and a CWideScreenMagicPauseEntities. CTimer: inline ctor/dtor, __forceinline operator= / GetTime.
- Residue (25 lines, documented blocker): the MovieBase dtor is expanded by retail at three exit pads where VC7.1 calls the
  COMDAT. Tried outside GOTCHAS: MOVIEBASE_FORCEINLINE_CTOR on this row, dropping the pause FORCEINLINE_CTOR, extra braces
  round each movie block (119), STB OUTOFLINE_DTOR. T80 meter (diagnostic only) gives 21.

## 0x00D067A0 NScript::CQ_BanditCampBossBattleScript::CBCGameMaster::Main (name proven)
- Proof: slot 1 of the CBCGameMaster vtable 0x012C8DE4 (2 Init 0x00D06760, 3 GetParentScript 0x00D06750, 4 OnPersist
  0x00D0EE10, 5 OnPredicateFail 0x00D06770); Ego_r.pdb + FableWin.pdb name; Ego_r size 36 = the retail Alloc new size;
  GivenPass +0x20 (Init clears it, Main reads it).
- Residue (52 lines): caller-size inline budget only -- the same source is RELOCATION_MATCH under the S15 / S20 meter
  (diagnostic). Documented park. Tried outside GOTCHAS: TRetVoid returns, plain-inline pause ctor (317), explicit CCharString temps (413).

## 0x00677970 CPersistTraits<CReactionFrequencyTraitsArray>::TransferIn (name proven; label + NAME_FIXES)
- Not the COpinionTransientOffset list the brief guessed: ret 0Ch (three arguments), it news CReactionFrequencyTraits_LinearTime
  (0x006FF9A0) / _PeriodicFrequency (0x006FED30) / _ControlledCount (0x006FEE20) into LowHigh[0][n] / LowHigh[1][n] (+0x13C =
  79*4, the Ego_r `CReactionFrequencyTraits* LowHigh[2][79]`), and sits beside the landed TransferBinaryOut 0x00677840 /
  TransferBinaryIn 0x006778A0 of the same traits. The placeholder came from the copied error string.
- Key spellings: positive `if (SkipPastNextItemIfIs(".Set")) {...} else PersistError(...)` (the negated test gave neg/sbb);
  the reaction index read through the generic inline CPersistTraits<T>::TransferIn (CPersistTraitsSLONG then the cast) gives
  retail's index copy; CPersistTraits<float> with an empty user ctor (no zeroing of the temporary) fixed the size (2273 v 2272).
- Residue: register allocation (context in ebx and temporaries in the dead argument slots in retail). Descriptive: the index
  enum EOpinionReactionType (79 entries, name unproven).

## 0x00E01900 NScript::CQ_TraderEscortScript::MakeTraderComment (PDB name)
- Shape decoded in full (see the row's header comment). Key spellings: the first two IsAlive tests go through the vtable
  (a const-reference helper), the later ones call 0x004AB130; operator== / != inline as PImp->IsEqualTo (slot 78); the
  single-comment path expands GetDataString in place (inline body, VC7.1 picks the same site); the timer-exit terminate test
  ignores its result (`IsActiveThreadTerminating(); return false;`).
- Residue: retail expands CScriptThing::operator= at the first assignment only; VC7.1 never expands the plain inline
  operator= (or CCountedPointer::operator=) here even in a cut-down body, __forceinline expands all four sites.
- Fact for later: CTCEntityEvents::GetEvent is ??$GetEvent@VCEventVillagerInteraction@NEntityEvents@@VCIsEventOfType@2@@CTCEntityEvents@@QBE_NPAPAV..@ABVCIsEventOfType@2@@Z
  (name sets): the 0x19 temporary is a CIsEventOfType filter -- relevant to 0x00981410.

## 0x00981410 CAIStateGroupBase::AIAction_InteractWith (PDB name)
- Types proven from Ego_r layouts + RTTI: CEventVillagerInteraction (vtable 0x012995A8), CVillagerInteractionsDef (Animation1
  +0x3C, Animation2 +0x40, Distance +0x44 in retail), CIntelligentPointer<CThingAICreature> (0x0123E7E0), CAnimFilterAnyGroup.
- Callee label candidates (one source only, not in LABELS): 0x009824E0 = ??$GetDef@VCVillagerInteractionsDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCVillagerInteractionsDef@@@@@Z
  (current label CDefinitionManager::GetDef_009824e0), 0x00982610 = the GetEvent above (current CTCEntityEvents::GetEvent).
- Residue: first full pass (890); see the row's Grade / Next idea lines. Descriptive local names: idle, flag, my_events.

## 0x00BB2540 CEngineClothRenderer::RenderPolygons (PDB name)
- Shape: render data created on first use (two vertex buffers, failure releases the CCountedPointer), CRenderStateScopedCaptureBlock,
  CalculateVertexNormals, vertex fill (position, normal negated for the back copy, UV), triangle + quad indices with the winding
  swapped per side, SetWorldTransform, SetupShaderPointLights, SetPolyCullMode(0), inline PrepareForRender + DrawBuffersIndexed.
- Descriptive: parameter names unused1 / unused2 (bool / unsigned long never read on this path), CClothVertex.
- Residue: first full pass (834); frame and loop scheduling.
