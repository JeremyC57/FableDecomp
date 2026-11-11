# Chunk H40g (Fable 5.2, seventh pass) -- 23 of the longest parked rows: hand-back notes

Every grade was made with `FABLE_HDR_OVERLAY=$A/hdr`. Result: **no row matched**; src/ is empty, payload.json carries 0 rows.
3 grades in all (ATTEMPTS.log): 0x00C4F600 x1, 0x00507C30 x2. Nothing written under the repo, no git command run, one compile at a time.
Mid-pass the coordinator relayed: do more hand authoring (0x0078FD60 / 0x00507C30 first), and the expanded asm rules
(docs/pipeline/ASM_RULES.md read; no row of this pass needed or used anything past plain C++).

## What was done
- **0x00507C30 CWorldMap::LoadFromFile -- AUTHORED (no source before), DIFFER(6489v7661), 2 grades.** differ/00507c30.cpp is a complete
  hand transcription of the 7661-byte body on the shared headers (CStringParser, CWorldMap, CRegion, CMapInfo, CAFile, CLevelLoader,
  CThingManager, CEngineManager, NProgressDisplay) plus six small overlay opt-ins (hdr/README.md). Facts read off the listing:
  - no EH frame; the 13 malformed-file exits are `throw CGenericException()` (throw info 0x013692F8), whose uninitialised 1-byte
    temporary shares the loadedOnPlayerProximity slot [esp+13h];
  - wide literals: L".bwd" 0x01244D98 (binary world), L".gtg" 0x01244BDC (compiled global things), L".gtng" 0x01244BB4; profile
    names "Load .wld file", "Init thing maps", "Load GTNG", "Load global things", "Load region graph";
  - the profile / progress markers build a NAMED CCharString and pass its address (0x00507C9F..0x00507CB7: `lea ecx,[esp+2Ch]`
    again after the constructor) -- reproduced by two inline helpers taking `const char*` (second grade);
  - keyword handlers and their targets are listed in the draft (24 keywords; DisplayName and RegionName share one body);
  - callee names bound from Ego_r.pdb by argument shape are in LABELS.tsv (second source still missing: UNVERIFIED).
  **Residue (differ lines 2-3):** retail tests the token's PStringData once, keeps it in esi and lays the 24-keyword chain out
  twice (null token: `repe cmpsb` against ""; else out-of-line CBasicString<char>::operator==(const char*) 0x004115A0). Ours
  reloads the token and re-tests after each failed compare, so it has one chain and is ~1.2 KB short; frame and `this` register
  follow. Four no-grade compiles did not produce the second chain: `#pragma optimize("a", on)`, `/Oa`, an assigned token, the
  visible operator== body (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE). Next ideas are in differ line 3.
- **0x00C4F600 CEAXManager::SetEAXProperty -- 1 baseline grade, unchanged (85 lines).** Re-read: in ROOM / ROOMHF / ROOMLF the
  rotating register is the reload of `data` (retail edx, eax, ecx; ours ecx, edx, eax), one step per case; ALLPARAMETERS before
  them uses fixed registers in both builds. No new spelling was graded (H40f's 20 dead ends stand).

## Not reached (drafts untouched, state as H40f)
All other 21 rows, including the three other "closest" rows (0x00E50FD0, 0x00D0F640, 0x00DB97A0), the three header-debt rows
(0x00603000, 0x00502E90, 0x006096E0) and **0x0078FD60 CTCDivineWrath::FrameUpdate, which still has no source**. The pass went
into authoring 0x00507C30 (reading 2038 instructions, binding ~50 callees, six opt-ins); that one row used the session.

## Header debt
- New (differ only): hdr/README.md "Chunk H40g additions". In-row declarations still to move to headers: CGenericException,
  NProfileTimer, GetLevelsDirectory 0x0041A060, the mode flags, CStringParser::ReadLineAsString 0x009BA390.
- Open as before: hand729 (0x00603000), hand879 (0x00502E90), hand883 (0x006096E0).

## Rule slips
- The brief's order (four closest rows first, a grade per idea, every row graded) was not kept: after the coordinator's message
  the pass was spent on one authoring task, and 21 rows got no grade.
- differ/00507c30.cpp declares five things in the row (listed above) and calls ReadAsStringUntilWhitespace where retail calls
  0x009BA390: it is a differ draft, not landable as it sits.
- differ/00e40e80.cpp and differ/00e50fd0.cpp still carry bare `__forceinline`; differ/00c4f600.cpp still has row-local padded models.
