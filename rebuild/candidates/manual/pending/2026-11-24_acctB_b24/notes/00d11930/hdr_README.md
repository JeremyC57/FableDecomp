# hdr/ overlay -- 0x00D11930 CAssassinMarker::Main (agent B24A4, 2026-11-24)

| header | opt-in macro | what it adds | evidence |
|---|---|---|---|
| engine/CQ_BanditCampBossBattleScript.h (today's repo header, full copy) | FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER | the nested `class CAssassinMarker : public CEntityScriptBase` (Alloc 0x00D048C0, GetParentScript slot 3 0x00D04960, Main slot 1 0x00D11930, ParentClass +0x14, PMasterData +0x18) + `sizeof == 0x1c` static assert. Additive: the default branch is unchanged (the header already forward-declares `class CAssassinMarker;`). | chunk 863/868's block (pending/2026-11-08_linux_hand868/hdr), re-applied onto today's header only; Ego_r.pdb CAssassinMarker 28 bytes; landed Alloc 0x00D048C0 allocates 0x1c and stores vtable 0x012C8B80 (slot 1 = 0x00D11930); both PDBs name ?Main@CAssassinMarker@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ |

No other overlay file. Old hand868 copies of fable_string.h / fable_definition_table.h / stlport/fable_containers.h are NOT used (the row
matches on today's repo headers).

## The MovieBase / GROBase header question (the brief's Step 2) -- answer: none found; the row matches with the family's flag (EXTRA_RULES owner decision 2026-11-08: per-row /d2inlS + scoped inline_depth)
- The implicit ~CScriptGameResourceObjectMovieBase with retail's text costs sz 25 / c1 19 in c2 (call ~CP(this+8) 8 + inlined
  `virtual ~CScriptGameResourceObjectBase() {}` 11 + 6). Every form measured at <= 19 drops the 0x0126008C store or leaves ~GROBase
  called (probe table in ../00d11930/NOTES.md). FableWin debug sizes agree with today's text for the whole chain (75 / 75 / 28 / 19).
- The ctor needs the same effect (implicit ??0MovieBase = ??0STB = c1 20, retail expands the movie ctor where it calls the STB ctors):
  today's FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR opt-in stays.
- So the 21 objects of Account A's audit keep their NEQ ??1MovieBase / ??1CIsThingAlive copies (not leaf-referenced: debt); this
  row's object has the same two NEQ copies (pick-any check in NOTES.md), and no new leaf-referenced NEQ.
