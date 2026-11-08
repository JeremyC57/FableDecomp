# Chunk 877 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00839940) A debug-draw `CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(h, CEnginePrimitiveLine(...), 0)`
  loads the engine and its vtable (`mov edx,[G]; mov eax,[edx+28h]; mov esi,[eax+2Ch]; mov ebx,[esi]`) BEFORE the argument pushes only
  when the engine is a NAMED reference local (`CIEngine& engine = ...;`) and the vector operand a named copy
  (`C3DVector direction = hit.GetHitDirection();` -> `fld [edi+0Eh]; mov ecx,[edi+16h]` with Z copied as an int): 295 -> 267 masked diff lines.
- (0x00839940) VC7.1's per-function inline budget in a 5 KB OnHit: retail expands HasTC everywhere but calls std::bitset<274>::test
  0x00410DE0 out of line at the six late sites. Plain spelling outlines only the last two; /d2inlS15..17 outlines HasTC itself
  (?HasTC@CThing call) instead; `__forceinline` HasTC + /d2inlS15 gives four of six test() calls but then expands GetTCDriver<CTCDExplosion>
  (retail calls 0x007512C0). The budget is cumulative: expanding GetTCDriver early is what pushes two more late sites out of line, so
  retail spends more budget earlier than our model (candidates: STLport bitset/CCharString::operator== inline costs).
- (q.py) Never grade two probes of the SAME address concurrently, even different sources: q.py reuses `$S/wine/q/<addr>/`, the second
  job reads the first job's obj (identical results, or OBJDUMP_ERR). Same for any call-diff script that reads that obj: run it right
  after its own grade.
- (0x00839940) Return-tail layout is a cross-jumping consequence of the release-block scheduling: retail's normal end
  (`dec esi; mov edx,esi; test edx,edx; mov [eax],esi`) differs from the battle-charge tail (`dec esi; mov [eax],esi; mov edx,esi`), so
  the electrocute `return` merges into the battle-charge tail (0x0083AD60); ours merges it into the normal end. Not a source-order issue.

Review at landing:
- 0x00839940 took 45 grades (several void: concurrent same-address runs, marked in ATTEMPTS.log); parked.
- 0x00804020 has no compiling body (0 grades): it needs ~7 STLport algorithm instances with anonymous-namespace binder functors;
  budget a dedicated single-row worker.
- Landed 0x0083AF60 (CThing_GetPBaseDef_0083af60.cpp) is CHitParameters::GetPHitMaterialDef (FableWin.pdb; +0x4C); landed 0x0083AE30
  CHitThing ctor takes `const CCreatureStatsDef&` (PDB), not `CThing*`; landed callers of 0x0057A56C spell InformOfEvent with a pointer
  (PDB: const CBase&). See NAME_FIXES.tsv.
- 0x00815270 is called by OnHit on a CHitLocation (`IsToPlayGetHitResponse`); the landed name CTCShotDefinitionBase::CanBeHitByMeleeStrike
  came from a shape generator -- check for COMDAT folding before respelling.
