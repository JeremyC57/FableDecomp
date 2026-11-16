# Identity and exact bindings

Both original PDBs name `?GetGreetDialogueGroupIndex@CTCCreatureOpinionOfHero@@QAEKW4EOpinionAttitudeType@@@Z`. Original FableWin01f47e40 is the full debug donor; its resolved calls and local types reconstruct the control flow. Native callers009299ed,0092ff9c,00960427 target00683c10. Assigned extent1085 ends at0068404d with ret4; next function starts00684050 after alignment.

`EGOR_TYPES.json`/`FABLEWIN_TYPES.json` and `*_MORE_TYPES.json` contain original class/member/constructor/access records. `*_NAME_PROOF.json` supplies exact methods, `*_GLOBAL_PROOF.json` the three static data symbols. `*_CVECTORMAP_PROOF.json` proves the original const/mutable overload signatures; native STLport iterator return is a pair pointer rather than the donor's later iterator wrapper. Real shared STLport lower-bound bodies emit those exact signatures.

`RELOCATION_AUDIT.json` checks every58 relocation operands, including full literal content,0.35f IEEE bits, private const attitude-array symbol, and const/mutable lookup addresses. Historical wrong spellings are reported in NAME_FIXES.tsv; they are not copied into the source to satisfy old bindings.

The three named parameterless header accessor bodies are original PDB methods with donor calls: CTCHeroStats::GetTrainingStatus reads native+114, CTCDrunkenness::IsDrunk reads+0c, CTCHero title accessors read+18. Their constructor declarations are likewise exact public CThing& overloads in BOTH original PDBs and support normal fixture lifetimes. Padding stays explicitly opaque; no local replacement classes are used.

Every new header branch is under FABLE_RETAIL_GREETING_PASS23. Existing profiles remain unchanged; original assertion lines in changed headers are pinned. Existing pack scopes in imported shared headers are retained; no new packing or inline-control scope was added.
