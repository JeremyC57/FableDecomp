# Workflow wf19 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 006b3ff0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Unchanged from the original match: the by-value CCamera temp from GetRenderCamera(1.0f).PeekWorldPos(), and the threshold spelled as extern const float DELTA_NOTIONAL_ZERO. The repair only picked the typed GameTickState spelling of GetRainStrength for the whole TU and showed that LightingFlash and 0x006B2BD0 also match with it.
- OPEN at hand-back: NOTES.md and DOSSIER.md:20 say the landed made-up name `CBufferPairTriple_006b5d80::~CBufferPairTriple_006b5d80` (rebuild/src/compiled/00/6b/CBufferPairTriple006b5d80_CBufferPairTriple006b5d80_006b5d80.cpp) is really ??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ, but $W/NAME_FIXES.tsv has no line for 0x006B5D80 (it has only 0x006B2BD0). The two sources exist (Ego_r 0x006F1D10 and FableWin 0x0244D5F0, plus the ctor 0x008864A0 / dtor 0x006B5D80 pairing on the 500-byte theme local in this row), yet NOTES says the fix is 'not re-proved here'. Add a NAME_FIXES.tsv line with that evidence so the coordinator respells the landed row.

## 00727d00 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The fixes did not change the match. The row now gets GetFullScale from the existing FULLSCALE opt-in instead of declaring it again, and the overlay is rebuilt from the current repo header, so nothing changed in the compiled bytes.
- OPEN at hand-back: The row's local model gets CTCAppearanceMorphBase's nested types wrong, and the PDB, the shared types and the landed sibling all disagree with it. The source nests CMeshMorphSetParts directly in CTCAppearanceMorphBase and types its +0x04 member TextureMorphs as std::vector<CEngineTextureMorph>. Ego_r (struct_layouts_egor.tsv) has CArray<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>, with CMeshMorphSetParts.TextureMorphs typed CEngineTextureMorphSet { CArray<CEngineTextureMorph> Morphs +0 }. The landed sibling rebuild/src/compiled/00/72/CTCRandomAppearanceMorph_GetMeshesUsed_00727c20.cpp already uses CMeshMorphSet::CMeshMorphSetParts with a CEngineTextureMorphSet TextureMorphs. Fix: nest CMeshMorphSetParts inside CMeshMorphSet, model TextureMorphs as CEngineTextureMorphSet with a CArray<CEngineTextureMorph> Morphs member, and pass it->TextureMorphs.Morphs to the CEnginePrimitiveMorphedAnimatingMesh ctor. This is still the same `lea edx,[eax+4]` at 0x00727EA5, so the bytes do not change.

## 007bf790 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged. Only the header overlay was rebased, and the row still grades RELOCATION_MATCH.

## 006bfcd0 -- VERIFIED, author attempts 3

- What made it match: The final add needs the position (ahead + door->GetPos()) as a by-value copy evaluated first, with the offset C3DVector(-F.Y, F.X, 0)*side bound by const reference. Expressed as a file-static inline AddOffset(const C3DVector& offset, C3DVector position) { return position + offset; }.

## 0055d180 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged. Re-basing the CSurface overlay on the current repo header kept the match, because the GETBYTELENGTH declaration does not affect this row.

## 00a8f9e0 -- VERIFIED, author attempts 1

- What made it match: The repair kept the original body unchanged. Only the enum spellings changed: they now match the landed spelling, and the made-up enumerators were dropped.

## 00b206b0 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged apart from the comment. The `C3DVector& moved = change;` alias is now explained in the Shape section: without it, VC7.1 orders the IsNonZero x87 stack operations at 0x00B206F2 differently from retail.

## 007d46d0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: All three repairs kept the byte match. (1) The visible GetAllThings body now copies the landed 0x007D4CE0 row: scan test, then the CThingCreatureBase type-mask test (0xF), then the filter. The source comment names 0x007D4CE0. (2) `this` is now CTCTavernGameCoinGolf : local CTCTavernGameCoinBase (Arrow/Arrow1/Arrow2 at 0xF0/F4/F8, Coins 0xFC, Held 0x108, BoardPos 0x10C) : shared CTCTavernGame (FABLE_RETAIL_CTCTAVERNGAME). The chain is confirmed by retail RTTI in rtti_bases.tsv. (3) NAME_FIXES.tsv maps GetCamera's PCoin to Held and its TableCentre to BoardPos. Evidence: Ego_r type info plus the landed CoinBase rows AddNewCoin 0x008EFA00, UpdateMeshes 0x008EF2F0 and ShoveHaPenny::NewRound 0x007CF7E0. Offsets: Ego_r minus 0x18 up to Coins, minus 0x1C after it, because Ego_r's vector is 16 bytes and retail's is 12. So BoardPos is 296-0x1C = 0x10C, not 296-0x18.
- OPEN at hand-back: The row declares the callee as `class CThingFilter_IsHuman { public: bool operator()(const CThing& thing) const; };` (0x006A7B90), but Ego_r.pdb (egor_pdb_names.tsv 005a3150) and FableWin (FableWin.exe.signatures.tsv 01f95c80) both spell it `??RCThingFilter_IsHuman@@QBE_NABVCThingCreatureBase@@@Z`, i.e. `bool operator()(const CThingCreatureBase&) const`. The parameter type is wrong. I graded the PDB spelling and it is still RELOCATION_MATCH ($W/verify_evidence/f/b.cpp), so the declaration should be respelled. The landed 0x007D4CE0 GetAllThings row and the landed 0x006A7B90 row (`long operator()(CThingCreatureBaseCalls*)`) carry the same wrong spelling, and NAME_FIXES.tsv does not list them.

## 0094a3b0 -- VERIFIED, author attempts 2

- What made it match: Three source-shape facts. (1) The laughing branch repeats the 50/50 SitIdle/TellStory tail as an else-if block of its own; the compiler merges the two copies, which gives retail's hoists at 0x94A604/0x94A628 and the jump into talk case 2. (2) The talking switch stores into `bool result` with breaks and then returns it. (3) The Drink survivor needs both `if (!ThankBarmaid()) return false; return Drink();` and `switch(GFRandom(5)) { case 1: return FallAsleep(); case 0: if (IS_THING_ALIVE(PTankard)) return Drink(); default: return SitIdle(); }`, with case 0 falling through into default.

## 00d17a80 -- DIFFER, author attempts 3

- What made it match: not matched: Same residue as attempts 1 and 2. The body, the spawn and the 15/16 arms match byte for byte. Only the boast vector's destruction at the 5 early returns differs. Retail inlines each site to a different depth. At the for-top return it calls ??_G pair 0x004DA68A(0) in a loop, then calls ~_Vector_base 0x004D9C5D. At the two arm returns ~CCharString is inline and 0x004D9C5D is called. At the expiry and post-loop returns everything is inline. The vector ctor 0x0074F680 is called. Ours inlines every site fully and tail-merges them (705 bytes against 785). Reading the sites at /d2inlT50 shows the inline budget follows a DFS postorder of the CFG. Our compile visits the return path first at each terminate check (order 5,0,1,2,4,3: sites 3 and 4 lose). Retail fits the same postorder with the continue path visited first (0/5, 4, 2/3, 1, then the ctor). Best source is unchanged: differ/00d17a80.cpp.

