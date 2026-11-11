# Chunk 2027 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00DCD350: a dead `mov eax,[ecx+78h] ; mov edx,[esi+28h]` (or a dead `cmp edx,ecx`) right before a generated-script terminate check is
  `if (a == b) { if (IsActiveThreadTerminating()) return ...; } else { same check }`: c2 merges the one-check arms and drops the branch but
  keeps the operand loads. Writing the real statements in both arms does not merge (2850 v 1833 bytes).
- 0x00DCD350: in a generated-script Main, `CScriptThing x = Game->CreateCreature(...)` builds the result in place, while `CScriptThing x;`
  + assignment in each if / else arm gives the out-of-line CScriptThing() 0x006E7B40 + operator= 0x008AB980 + temporary destructor.
- 0x007C6E50: `bool b = d.GetSquaredMagnitude() <= r * r;` gives retail's late `mov byte ptr [esp+13h],1` and `test ah,1 ; je`; the
  `bool b = true; if (d2 > r*r) b = false;` form gives `test ah,5 ; jp` and an early store.
- 0x007C6E50: an implicit `T*` -> CIntelligentPointer<T> conversion in `member = f();` re-reads the temporary's slot for operator=
  (`lea edx,[esp+40h] ; push edx`); the explicit `CIntelligentPointer<CThing>(f())` pushes the constructor's eax.
- 0x007C6E50: FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY lowers the effect-table CDefPointer locals' priority from -10 to -13
  (c2prio), which lets a -12 candidate (the turncoat pointer) take ebp first; the PTarget address CSE then splits into ebx / `lea ecx` /
  edi as in retail. Check c2prio's colouring order before spending grades on a register residue.
- 0x007C6E50: the same TU expands CIntelligentPointer<CThing>(CThing*) / operator= inline in StartTurncoatingEffects 0x007C6AA0 but calls
  the 0x004A96F0 / 0x004A9760 COMDATs in FrameUpdate (budget split); the declared-only CTOR_OUTOFLINE / ASSIGN_OUTOFLINE opt-ins give the
  calls.
- 0x007C6E50: an if / else-if chain over two bools (`!A && !B && C`, `A && !B`, `!A && B`) compiles to retail's threaded tests (the
  third arm re-tests A at 0x007C7274); the absorb arm placed last needs `if (!x->IsTurncoated()) { ... } else { absorb }`.
- c2prio: a split piece's spill counts a copy / argument push as cost 0 and a memory-operand base as cost 2; a 2-ref piece of
  {load base, copy} has spill 2 and is coloured (copy bonus to the copy target's register), while {copy, push} has spill 0 and is
  skipped (codegen temp from the rotation, reused for the copy). The patched copy in tools/c2p/c2prio.py prints spill / refs / key at
  every choose / skip.

Review at landing:
- 0x00DCD350 needs hdr/engine/CQ_OrchardFarmRaidScript.h (new). MakeTeamMemberComment 0x00DCDA80 is called under its PDB spelling
  (QAE_N ABVCCharString AAVCScriptThing W4ERaidComment); the landed 0x00DCDA80 row defines a different spelling (NAME_FIXES.tsv): respell it
  at landing or vland reports RELOC_NAME for this caller.
- Landed CTCTurncoatSpell rows call 0x0072AC40 as `CThing::FadeOutEffect()` and 0x0072AB70 as a free `CreateParticleEffect`, and
  StartTurncoatingEffects names 0x00669580 / 0x00669D30 `CSpecialAbilitiesTurncoatSpellDef::GetSubDef` / `CEffectTableDef::Lookup`
  (NAME_FIXES.tsv).
- 0x007C6E50 parked after 4 grades of pass 1; passes 2-3 were not graded because every further spelling checked with c2prio (16) left
  the deciding allocation unchanged -- the next step is c2 split-piece instrumentation, not spellings.
