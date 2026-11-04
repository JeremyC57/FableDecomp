# Chunk 760 notes (40 parked rows, 54-297 bytes)

The read-only vland (`vland_result.txt`) has 7 wins (all RELOCATION_MATCH, behaviour PASS) and 33 rows parked in `differ/` with passing tests.

## Results
| addr | row | result | what fixed it / residue |
|---|---|---|---|
| 00cfcca0 | NScript::CQ_AwakeningTheOracleScript::CPretendGuard::Main | RELOCATION_MATCH | after the StartScriptingEntity wait: `if (IsActiveThreadTerminating()) return; do { NewScriptFrame(); } while (!IsActiveThreadTerminating());`. VC7.1 cross-jumps the pair into retail's `jmp` to the loop test. Shared CCountedPointer.h / CPredicateBase.h / CScriptGameResourceObjectScriptedThingBase.h (_OUTOFLINE_DTOR) clear vland's SHARED_HEADER warnings |
| 006399b0 | CTCRegionDisplay::GlobalUpdate | RELOCATION_MATCH | the fade-out tail is written `(remaining / fadeFrames) * 255.0f`; the parentheses stop the tail cross-jump |
| 009bbbf0 | CPersistContext::CheckUnknownSymbols | RELOCATION_MATCH | the spare local is a CParsedItem (12 bytes, String at +8), which gives retail's 8-byte frame gap; the callee is PersistError (const), not Error |
| 00946400 | CAIStateGroup_PrepareMeal::IsRunnable | RELOCATION_MATCH | retail headers plus the CTHINGAICREATURE_HOMEBUILDING overlay. A `CThingAICreature& creature = *PBrain->Creature;` local just before GetFreeActionPoint fixes the ebx/ebp roles; the tail is written `if (..) return true; return false;` (the `&&` return gives `mov eax,1`) |
| 006e2c00 | CTCTalk::OnCreate | RELOCATION_MATCH (1st try) | retail headers: a CDefPointer<const CCreatureDef> local, PeekBaseDef, CDefString::GetString, plus the GetSubDef<CCreatureDef> template body in the TU (the landed 0x005ED4A0 form). VC7.1 then keeps the def in edi and shares its frame word with the string temporary |
| 00a11ae0 | CPhysicsMeshObject::GetBoundingBoxCentre | RELOCATION_MATCH | the landed 0x00A11C20 sibling model; GetCenter goes into a named local and TransformPoint's body is written out, with the Z row as `E23*yy+(E13*xx+E33*zz)+E43` |
| 00f37660 | CGPUScratchBufferTextureManager::AllocteMemoryPage | RELOCATION_MATCH | the shared CArray / STLport vector with FABLE_RETAIL_STLP_VECTOR_C, so vector<long>::operator= is defined in the TU; the copy is the implicit CResourcePage operator= |

Parked rows: the residue and the next idea are on each `differ/<addr>.cpp` line 2 and in `STATUS.tsv`. The best ones:
- 0086be40 (dl=2) and 009f5690 (dl=2): scheduling. Next: move the set into or out of the if, and the time locals.
- 0093d270 (dl=3): a `const float&` GetProximityForNavigation matches (try x1, mirror_x) but contradicts FableWin's QBEMXZ. This needs the owner's access/return-type decision; chunk 732's 0x006AA720 has the same residue.
- 00441ee0 (dl=4): the constant 1 is hoisted into ebx.
- 00683b00 (dl=7): two eax/ecx/edx register choices (back(), the KnownDeeds index).
- 0095d0e0 (dl=9).
- 00878be0 (dl=11): the position.X and sum spills.
- 004b01b0 (dl=12).
- 007e5570 (dl=15): the money/def esi-edi roles follow declaration order.

Rows marked "pass 1 draft grade only" in STATUS got no pass-2 work: 008956b0, 009c9170, 00476e3e, 00477cd2, 00c3a7d0, 005fbfa0, 00877720, 009e3b60, 00ad9770, 008cf7d0, 0092bf50, 00863280, 00b242d0, 00bc2480, 00752e00. These drafts are local models; the next step for each is the retail-header rewrite plus same-TU template bodies (the pattern that won 006e2c00, 00f37660 and 00946400). The drafts for 005fbfa0, 009e3b60, 0092bf50, 00b242d0 and 00752e00 had no `// Leaf:` line, so their pass-1 grades may have measured the largest function. The differ files now carry the Ego_r mangled leaves; regraded, the sizes match the old grades.

## Fixes for GOTCHAS (one line each)
- A same-TU body for a callee that retail compiled in the same TU stops VC7.1 reloading `this` into ecx for the next call; it knows the callee leaves ecx alone. Examples: 0x008F6FC0 for 0x008F70F0, and GetCamPosOffsetXY 0x00878A80 for 0x00878BE0.
- Defining a member template's body in the TU (GetSubDef<T>, vector::operator= through FABLE_RETAIL_STLP_VECTOR_C) lets VC7.1 see that the out slot or local is not captured. The value then stays in a callee-saved register, dead dtors fold away, and frame words are shared (0x006E2C00, 0x00F37660, 0x007E5570).
- An unfolded `x * 0.5f * 0.0f + 1.0f` (a Lerp(1,1,t)) needs `t` as a named float local; inline, VC7.1 folds the constants (0x00878BE0).
- `C3DVector rotated = F(); C3DVector offset = rotated * s;` reads the result from the frame, not through eax (0x00878BE0).
- TransformPoint's Z row as `E23*yy+(E13*xx+E33*zz)+E43` gives retail's E33, E13, E23 order (0x00A11AE0). The parked 0x00B087C0 has the same Z order.
- A `T& x = *PBrain->Creature;` reference local before a call that takes four member references puts &member in ebx and the temp in ebp, as retail does (0x00946400).
- Several drafts had no `// Leaf:` line and were graded on the largest function in the TU. Add the mangled leaf before trusting a draft's grade.

## Header overlays (all new sub-macros; hdrcheck2 default=same for every file)
- engine/CTCBase.h `FABLE_RETAIL_CTCBASE_WORLDSEED`: `unsigned long* GetPWorldSeed() const;` (protected, 0x00686DA0). Used by 006e2c00.
- engine/CTCTalk.h `FABLE_RETAIL_CTCTALK_ONCREATE`: `virtual void OnCreate();` (slot 4). Used by 006e2c00.
- engine/CCreatureDef.h `FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES`: StatGetTypeName, plus `std::vector<CDefString> DialogueVoices` at +0x6C. Used by 006e2c00. The current repo's _quarantine CCreatureDef.h has DialogueVoices opaque at +0x6c, which agrees.
- engine/CThingAICreature.h `FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING`: `CIntelligentPointer<CThingBuilding> HomeBuilding` at +0x168 and a declared `GetHomeBuilding() const`; the includer defines the body once CThingBuilding is complete. Used by 00946400.
- engine/CPhysicsMeshObject.h `FABLE_RETAIL_CPHYSICSMESHOBJECT_CENTRE`: `GetBoundingBoxCentre() const`. Used by 00a11ae0.
- engine/CMap.h `FABLE_RETAIL_CMAP_THEMEPALETTE`: CThemePaletteEntry; typed ThemePaletteEntryList / GameMap / HeightMap; the inline GetThemePaletteIndexFromThemeDefIndex and FindUnusedThemePaletteEntryIndex; ZeroAll / Add declarations. Used by the parked 008f70f0.
- engine/CTCCreatureOpinionOfHero.h `FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS`: the std::list<long> PendingDeeds / KnownDeeds[92] and ClearPendingDeeds. Used by the parked 00683b00.
- stlport/fable_containers.h:
  - `FABLE_RETAIL_STLP_LIST_POPBACK`: list::pop_back, STLport 4.6 `erase(--end())`.
  - `FABLE_STLP_OUTOFLINE_UNINIT_COPY`: the __fastcall noinline non-POD __uninitialized_copy, mirroring the existing _FILL_N.
  - `FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE`: push_back declared only.
  - These are used by parked rows only.
- engine/CCombatManager.h `_PROXIMITY`, engine/CTCCreatureNavigation.h `_NAVIGABLE`, engine/CInputEvent.h `_COPY`: used by parked rows only (0093d270, 009f5690).
- Siblings: none. Every change is a new sub-macro with the default path unchanged, so no landed row compiles differently (no SIBLINGS.tsv).
- The matched rows need these hdr/ overlays: 006e2c00 (CTCBase, CTCTalk, CCreatureDef), 00946400 (CThingAICreature) and 00a11ae0 (CPhysicsMeshObject).

## vland warnings (soft)
- 009bbbf0: RELOC_NAME. The call binds ??1?$_List_base@VCStringParserCommentBoundaries..., but the catalog label at 0x004046B0 is `std::list<...>::~list`. With the shared STLport list, ~list is inline-empty and the out-of-line call is the base destructor. The label spelling is the catalog's; it is not changed here.
- 00946400: RELOC_NAME. 0x00946380 is labelled CAIStateGroup_SitForMeal::GetPCookingPot; the Ego_r spelling is GetFreeCookingPot@CAIStateGroup_PrepareMeal (NAME_FIXES).

## Helpers
None (no __asm / naked / _emit / volatile anywhere).
