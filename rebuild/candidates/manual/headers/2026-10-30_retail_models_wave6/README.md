# Retail class-model migration, wave 6 (2026-10-30): 1,481 landed rows onto shared retail headers, bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv; REVERTED.tsv (140 entries / 135 rows, each with its error or byte difference); HEADERS.md;
NEXT.md (wave 7 plan; the engine value-class header scope with row counts, Ego_r layouts and proposed shapes; the asm-bake
list); LESSONS.md; bind_add.tsv; logs/asm_bake.tsv.

Headers changed (no new class headers): CGameScriptInterface.h gains public `long GetScriptSpriteBySymbol(const
CCharString&) const` 0x006E77F0 (landed rows called it the invented GetTextIDFromSymbol; proof: the body ignores `this` and
looks a CCharString key up in the map at +0x934 of the PLAYER_GUI_PC definition, which retail's CPlayerGuiDef setup registers
as "ScriptSprites" (string 0x012362CC, Ego_r map<CCharString,long>), and both PDBs have exactly one CGameScriptInterface
method of that shape); the generated CPlayerManager.h gains a `// Retail override` mark and 5 PDB methods (GetPlayer(long)
0x004498C0 / const 0x00449910, GetMainPlayer() 0x00449970 / const 0x00449960, GetMainPlayerNumber() const 0x00449700;
const twins settled by Ego_r address order and the landed bindings). 80 random unchanged CGameScriptInterface includers and
all 20 CPlayerManager includers compile to the same bytes.
Rows: GetMainPlayer accessor rows 14, CGameScriptInterface 191 (incl. the 4 GetPlayer reverts, HeaderLayout/Donor alias rows,
37 GetScriptSpriteBySymbol rows, the 8 float setters: the store target becomes `float`), GRO 44 (16 of them wave 5's
mixed-inlining reverts), CScriptBase / CLevelScriptBase<T> 1,248 (mig_sb.py; the invented SpawnFunction helper in 208 rows
became the real AddSpawnedFunction(f, CCharString(r))). Agent recheck 1,481/1,481 PASS (19 MATCH); pre-regrade compare vs HEAD
1,325 identical + 156 identical bytes with renamed symbols (132 leaves now the PDB UAE spelling). Coordinator independent
spot regrade after cherry-pick onto 2d14dc352 (reloc_bindings merged by set-union): 102/102 PASS; multi-check 0.
**GRO inlining rule found (LESSONS item 1):** VC7.1 decides per call site whether to inline the implicit ScriptedThingBase
dtor by that dtor's size; it reproduces retail's per-site choices (even inline at one exit, called at another, 0x00DF06A0)
once the row's CCountedPointer<T> carries the real retail release dtor. Wave 5's reverts had pointer models with no dtor,
so the inline dtor was tiny and inlined everywhere. Fix is in the rows (scripts/gro_full.py), not the headers; proof row
0x00E0CFF0 DIFFER -> RELOCATION_MATCH; 43 of 55 non-MovieBase rows pass.
Open: MovieBase rows (22 of 23 fail: retail inlines the MovieBase dtor but calls the pointer-dtor copy 0x006E7AB0 inside it);
ctor/dtor temporaries passed as arguments (0x00EB6A00) are out of line in retail, inlined by us (the 50 callers of
0x007E72A0 remain unexplained); 185 non-template CLevelScriptBase rows need their PDB master-data T; 41 rows have
`CGameScriptInterface* Game` where the PDB has CGameScriptInterfaceBase*; 90 local CBaseClassNonCopyable; whether retail
inlines the CLevelScriptBase ctor (78 rows inline it) is unproven.
Reverted 135: CGameScriptInterface 64, GRO 59, CScriptBase 17 (one-parameter std::vector/list, non-template CCountedPointer
or enum typedefs clashing with fable_nscript_forward.h; MovieBase rows); 00ec1ef0's passing CGSI edit was lost with its GRO
revert (back at HEAD). Remaining local models: CGameScriptInterface 220, CScriptBase 456, CLevelScriptBase 303,
ScriptedThingBase 220; CScriptThing 2,503 / CEntityScriptBase 1,122 / CTimer 200 / CIsThingConscious 48 not reached.
Bindings: +GetScriptSpriteBySymbol -> 006e77f0; 3 invented GetMainPlayer spellings retired; GetTextIDFromSymbol stays bound
(rows 00891340 / 00892810 still call it; respelling in NAME_FIXES).
