# Workflow wf4 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 004b3ce0 -- VERIFIED, author attempts 1

- What made it match: Three source-level facts, none a compiler tie:

1. **Callee-body visibility.** With the real LoadResourcesForQuests 0x004B2510 body and STLport's `_M_insert_overflow` body in the TU, the local array no longer escapes; with the real CBasicString<char>::operator< 0x00429950 body, the persist map's end() is read once.
2. **Loop form.** Both loops are `while (i < size) { ..; ++i; }`. That alone makes VC7.1 call `set::erase(key)` in the scripted arm and expand it in the bare arm, through the new tree-bounds opt-in.
3. **PDB inline accessors.** CScriptInfo::IsMasterScript / GetMasterScriptDataAllocFunc / GetAllocFunc give `call eax` and tip the zero-register colouring.

Smaller pieces: `FABLE_RETAIL_CARRAY`, `master_data` assigned before LoadMasterScriptDataFromGameState, and AddMessage on the World member rather than the accessor. `AllocateQuestID()` (PDB inline) is used for the id; `CurrentQuestID++` grades the same.

Made-up name: `g_QuestScriptsEnabled` (0x01375454), found in no PDB.

Files are in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/004b3ce0/:
- 004b3ce0.cpp
- 004b3ce0_test.cpp
- payload.json
- oracle.tsv
- hdr/README.md
- NOTES.md
- LESSONS.md
- NAME_FIXES.tsv
- LABELS.tsv
- STATUS.tsv
- ATTEMPTS.log

## 00a90040 -- VERIFIED, author attempts 1

- What made it match: The byte match is unchanged: the grouped Cross (COLLISION opt-in) plus the sibling idiom. Reading mesh.Vertices[index] directly instead of the PeekVertex helper gives the same bytes.

## 0064546f -- VERIFIED, author attempts 1

- What made it match: The source is unchanged and still grades RELOCATION_MATCH with the rebased overlays, both in normal mode and with LANDVERIFY_STRICT_HEADERS=1.

## 008565e0 -- VERIFIED, author attempts 1

- What made it match: The match is unchanged. The define was dead: nothing in the body reads it, so removing it left the bytes the same.

## 00ac8ae0 -- VERIFIED, author attempts 1

- What made it match: The move onto the shared headers alone. The old draft's local CArray declared push_back with no body, so VC7.1 treated &edge as escaping and the callee as unknown (frame 0x14, &b held in ebp). With the shared CArray plus FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE and FABLE_RETAIL_STLP_VECTOR (body visible, kept out of line like retail's 0x00AD5CE0), the compiler emits retail's frame 0x1C with &b and b.U[1] spilled and ebp pushed late. No flags or pragmas. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/00ac8ae0/00ac8ae0.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/00ac8ae0/test.cpp.

## 008f10b0 -- VERIFIED, author attempts 1

- What made it match: Plain C++ on the shared headers, no flags, no pragmas. Five facts: (1) the real bodies of GetCardDef 0x008F0B20 and GetPackPosition 0x008F0B80 sit in the TU ahead of the row; VC7.1's auto-inline then makes retail's own choice (the GetCardDef call before the loop stays a call, GetPackPosition and the GetCardDef inside it expand in the loop). (2) The GetSubDef template body is visible but kept out of line (engine/CDefClassBase_GetSubDef.h + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE): frame 0x20, the outer def's slot reused for the height counter, the inner add-ref/release folded in registers. (3) `pos += GetPackPosition()` rather than `pos = pos + ...`. (4) FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE places the count store at 0x008F116B. (5) The height is simply `(float)(53 - (int)i)`, strength-reduced by the compiler. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/008f10b0/008f10b0.cpp; recorded with addmatch.py in payload.json / oracle.tsv in the same directory. Notes for the lander: the row includes "stlport/fable_containers.h" first, because fable_game.h otherwise pulls the repo copy past the overlay. NAME_FIXES.tsv has one entry: 0x005E9B10 is the static ?HideMesh@CTCTavernGame@@SIXPAVCThing@@@Z (Ego_r + FableWin), landed as a free HideMesh(CMeshEntity*). The landed random_shuffle row 0x007D7860 comment calls this+0x134 "Cards"; it is Pack. No made-up names.

## 00addbe0 -- VERIFIED, author attempts 1

- What made it match: Four source facts: (a) six scalar float bounds and scalar centre components stored as Center.X/Y/Z, not C3DVector locals (frame 0x14, maxZ in st0, centre in the dead argument slots); (b) plain `for (i = 0; i < count; ++i)` over spheres[i] with each bound expression spelled twice, which keeps the loop rolled; (c) the radius read through the inline accessor GetRadius() in loop 1, which gives `fld Center.c ; fadd Radius`; (d) the distance spelled `dy*dy + (dx*dx + dz*dz)`. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/00addbe0/C3DBoundingSphere_BuildFromSubSpheres_00addbe0.cpp

## 0065a41c -- VERIFIED, author attempts 1

- What made it match: Ported onto the shared engine headers using the define/include block of the landed same-TU neighbour 0x0065C5BB (UpdateMirroredCrimeData), with the default CDefPointer release (not COUNTREF) and the map number and region number written as named `long` locals rather than one nested expression. Body: `CDefPointer<const CRegionDef> def = GetWorldMap().GetRegion(region).GetPRegionDef(); if (!def.IsNull() && def->UseCrimes) {...}`, `hero->GetTC(&deed_log)`, `bool seen[10]` + memset, `std::set<long>` iterator walk, effects def as a named CDefPointer local with `fine = -1; break;`. Pragmas `optimize("y", off)` + `optimize("s", on)`, flag /GS. Only local model left is CRegionDef (no engine/CRegionDef.h exists): CDefPointeeBase base with `bool UseCrimes` at +0x9D. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/0065a41c/0065a41c.cpp

## 0065aa6f -- VERIFIED, author attempts 1

- What made it match: Unchanged from author pass. Only comments and the hand-back changed, and the bytes still grade RELOCATION_MATCH.

## 00bbcc70 -- VERIFIED, author attempts 1

- What made it match: The port onto the shared headers by itself. The parked draft's flat free SetState(state, value, core) and invented texture holder were model errors. The real spelling is the nested member inlines SetTextureUVModeU/V(1, TEXTURE_UV_WRAP), which give block 2 its `mov ecx,eax` core copy and scoped ebp temp, plus AttachTextureToStage(1, &GetStipleFadeTexture()), whose `texture ? texture->PD3DTexture : 0` is retail's `add ecx,2AACh ; je`. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/00bbcc70/00bbcc70.cpp. Leaf: ?SetupPixelShader@CEnginePrimitiveManagerParticleGroup@@QAEXKABVCPixelShader@@0@Z. Parameter names reg / stipple_shader / normal_shader are descriptive (the PDBs carry none). GetStipleFadeTexture is named by FableWin.pdb only; the second evidence is the Ego_r member StipleFadeTexture at +0x2AAC.

## 007b8b10 -- VERIFIED, author attempts 1

- What made it match: The port onto the shared headers by itself. The old ebp/ebx register-web swap and the missing `mov edi,edi` loop pad came from the draft's hand-written local GetTC / lower-bound; the shared `GetTC(id, &ptc)` (FABLE_RETAIL_CTHING_GETTC_EXPANDED) with the inline LowerBound (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND) gave retail's bytes with no other structural change. The range test is spelled on `chair - 1` twice, and the next start index is read back after SetActionPointAsFree. No flags, no pragmas. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/007b8b10/src.cpp ; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf4/007b8b10/test.cpp ; payload.json and oracle.tsv written by addmatch.py in the same directory.

