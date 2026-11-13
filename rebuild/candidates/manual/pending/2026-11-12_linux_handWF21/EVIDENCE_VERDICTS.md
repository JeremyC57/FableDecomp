# Workflow wf21 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 004ce1b0 -- VERIFIED, author attempts 1

- What made it match: Three changes in the source fixed it. (1) The person fields are set to zero straight after `CCameraTag tag;` at the top of the function, so retail uses one shared zero in eax when it builds the pair. (2) `position` and `focus` are declared at the top and `view` inside the third-lookup block, which matches the retail frame slots. (3) Lookup 1 is a positive if. Inside it, lookup 2 is an early `return false` and lookup 3 is an if/else that returns false on failure. This puts the failure blocks after the success path, and the lookup-2 exit jumps into the ")" exit with ecx = &third already loaded, as in retail.

## 006a5610 -- VERIFIED, author attempts 1

- What made it match: Four things together. (1) Shared headers with the PDB callee spellings: GetPhysicsTC()->GetRHSet().GetUp() copied by value, GetPos, GetMagnitudeAndNormalise, AccurateNormalise, and GFGetShortestDistance2DSquaredBetweenPointAndLine taking float&, called with ToC2DVector temps. (2) The GF helper's real landed body (0x00A443A0) in the TU as inline __declspec(noinline): only then is t placed in the dead `to` argument slot, which gives the 0x58 frame. (3) Natural nested if blocks (if (d2>eps){... if (A&&B&&C){... if (Z tests) return true;}} return false;), which give retail's frame packing and the shared fstp st(0) exit. (4) The default one-line C3DVector::Dot.

## 007d71e0 -- VERIFIED, author attempts 1

- What made it match: Porting onto the shared headers. Calling random_shuffle once and inlining it once from its STLport body in the file. Counting-loop iterator advances. Loop 1 as a scan whose match branch erases and pushes back the card, then jumps past ++i with goto. A local `other` for the loop 2 partner card. Most important: the REAL _M_insert_overflow body (0x007D5D60) in the file as noinline, so VC7.1 knows push_back never captures the card and keeps it in esi across the memmove.

## 00e83230 -- VERIFIED, author attempts 1

- What made it match: The row first moved onto the shared headers, using the same-TU defines plus FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT. Then arm 1's `ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BOWERSTONE_SLUMS");` went inside a single-site do { } while (0). That made VC7.1 expand the implicit CScriptThing operator= at that site only, as retail does. The other arms still call the COMDAT at 0x008AB980.

## 00b4ac10 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The repairs kept the byte match: the exact-bit float literals, the real base classes (one shared header, one local model), and the comment citing the landed row did not change the code the compiler emits. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf21/00b4ac10/final.cpp (= v20.cpp)
- OPEN at hand-back: The overlay hdr/engine/CEnginePrimitiveManager2DParticleGroup.h (and DOSSIER row 'inline ... no ??0 symbol') says the derived constructor 'has no PDB symbol', but FableWin.exe.signatures.tsv has ??0CEnginePrimitiveManager2DParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z at FableWin 0x02D9D090 (37 bytes, plus thunk 0x018587F7), so retail 0x00B4B690 (`call 0xBBBF40; mov [esi],0x012A22D0; ret 4`) has a real PDB name. The comment needs correcting to cite that symbol, and 0x00B4B690 should be named with it, not called a nameless COMDAT copy.
- OPEN at hand-back: NOTES.md 'Name fixes' gives no evidence for either fix: no correct mangled symbol and no two sources. Both fixes are real: for 0x00BBBF40 the PDB has ??0CEnginePrimitiveManagerParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z in Ego_r 0x00A71660 and FableWin 0x02EA4AD0, while the landed row spells it (CEngine*). For _$E2 0x0122AD60, the landed comment says 0xBA1AC0 is AnimatedMeshes, but 0xBA1AC0 is ??0CEnginePrimitiveManager2DStaticMeshes (labels, vtable 0x012A5160 RTTI) and AnimatedMeshes is 0xBD1F30. NOTES has to carry this evidence, not just 'from DOSSIER, confirmed by match'.

## 00973dc0 -- VERIFIED, author attempts 1

- What made it match: The match from the original port is unchanged. The repairs did not affect the bytes. The constant sits behind a relocation, so changing it does not change the bytes, and the joined spelling GetWorld().GetFrame() produces the same code as the old split statements.

## 00875b10 -- VERIFIED, author attempts 1

- What made it match: The match is unchanged from the author's version: a namespace-scope EMeshType, an inline vertical-probe expression, the switch with cases 5 and 6, and the C3DVector operator*(float) named-result opt-in.

## 00e6b900 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The match was already there. The repair added provenance comments only and changed no code.
- OPEN at hand-back: The hand-back has no NAME_FIXES.tsv for the landed operator== row 0x008AAE20 (rebuild/src/compiled/00/8a/CScriptThing_operator_008aae20.cpp). That row spells slot 78 (+0x138, retail 0x004AA8A0) with the made-up name `IsSameThing` on a local `CScriptThingImp_Retail`, and it defines the function as `CScriptThing_Methods::operator==`. The author's own evidence names slot 78 `?IsEqualTo@CScriptThing@@MBE_NABV1@@Z` (FableWin.pdb _fablewin_api.tsv 017940fa / 018cd700, plus rtti-vtables.tsv CScriptThing 01238c8c slot 78 = 004aa8a0) and `?CScriptThing::operator==` 0x008AAE20 (CScriptThing.h, Ego_r + FableWin). The source comment says the inline operator== body is 'copied from' that landed row, but it is actually a respelling of it, so the comment should say so and a NAME_FIXES line should go back.

## 005ec270 -- VERIFIED, author attempts 1

- What made it match: Same match as the author's: shared headers, the real GetSubDef<CTavernGameDef> body in the TU, case order 5,46,45,44,47, the inline GetGameState() call, and the prompts cleared in source order Alt, No, Yes. The SetState body is not needed.

## 004093e0 -- MATCH_WITH_OPEN_ISSUES, author attempts 3

- What made it match: The block placement already matched: the guarded statement `if (def.IsNull()) Antialiasing = 0;` after the detail if-else. The repair edits are byte-neutral and are graded after each step.
- OPEN at hand-back: Inline body GetDisplayModes is defined for 0x00406880, but that address is already landed as CModeBlock::GetAnimFilter (rebuild/src/compiled/00/40/CModeBlock_GetAnimFilter_00406880.cpp; manifest says NCreatureMode::CModeBlock::GetAnimFilter). The code is the same, `return this+0x80` (an ICF fold), but NAME_FIXES.tsv has no row that settles the one-address-one-name conflict.
- OPEN at hand-back: The stand-in ClampResolution body (0x00406D90) differs in code from its landed row CUserProfileManager_ClampResolution_00406d90.cpp. The row takes `unsigned long&` and uses `while (index >= 0 && DisplayModes[index].Dimensions.X > maxWidth)`. The landed row takes `unsigned long*` and uses the made-up FABLE_FORCEINLINE helper IsModeWiderThan on a local CRetailVector model. NAME_FIXES.tsv asks for the landed row to be respelled at landing, so the coordinator must apply that respelling, or the two copies diverge.
- OPEN at hand-back: The stand-in GetConfigOptionsDefaultsDef body (0x004083E0) differs in code from its landed row CUserProfileManager_GetConfigOptionsDefaultsDef_004083e0.cpp. The row uses CMainGameComponent::Get() / CNewFrontendGameComponent::Get() and `CGameDefinitionManager::Get().CDefinitionManager::GetDef(...)`. The landed row uses the globals g_pMainGameComponent / g_pFrontendRuntime and an unqualified GetDef. NAME_FIXES rows for 0x013B86A0 / 0x013B871C cover this, but the respelling must be applied at landing.
- OPEN at hand-back: TEST: the 'detection overrides beat the def' case (g_MemoryA=800, g_MemoryB=600, expect Resolution 0) cannot catch losing the override. With `if (g_MemoryA > 0) width = g_MemoryA;` deleted, width falls back to the def's 1280, no mode matches, the index runs off the end and is clamped back to 0, so the test still prints PASS ($W/verify_evidence/v2/m2.cpp). Use an override that selects a non-zero mode, e.g. 1280x1024 with a 640x480 def.
- OPEN at hand-back: TEST: no case reaches the fourth refresh-table entry. Changing the pick loop to `while (++i < 3)` still prints PASS ($W/verify_evidence/v2/m3.cpp), because the 90 Hz case is capped back to index 2 by g_MaxRefreshRate=75. Add an uncapped case whose display rate is nearest 85 Hz.
- OPEN at hand-back: SOURCE comment: the Callees line says `ClampVideoOption 0x004068D0 (x4 per arm)` twice in a row (copy/paste duplicate).
- OPEN at hand-back: SOURCE comment: for the carried ClampResolution body, the file names neither the landed row it corresponds to (rebuild/src/compiled/00/40/CUserProfileManager_ClampResolution_00406d90.cpp, whose spelling is different: pointer argument plus the made-up IsModeWiderThan) nor the retail fact that shows the body was visible: no call to 0x00406D90 and the loop is inline at 0x0040959F..0x004095D2. The same is missing for ClampRefreshRate (inline at 0x004096DF..0x004096FA) and GetDisplayModes; the comment only says 'inlined same-TU bodies'.

