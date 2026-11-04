# Landing 2026-11-04w: chunk 760 (retry of 40 parked rows off retail's length, 54-297 bytes, C++ only), 7 landed

## Result
- **Landed 7 of 40** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - **NScript::CQ_AwakeningTheOracleScript::CPretendGuard::Main (0x00CFCCA0):** `if (IsActiveThreadTerminating()) return;` then a
    do/while, which VC7.1 cross-jumps into retail's `jmp`. It uses the shared CCountedPointer / CPredicateBase / ScriptedThingBase
    headers.
  - **CTCRegionDisplay::GlobalUpdate (0x006399B0):** the parenthesised `(remaining / fadeFrames) * 255.0f` stops a tail cross-jump.
  - **CPersistContext::CheckUnknownSymbols (0x009BBBF0):** the spare local is a CParsedItem, which gives retail's frame gap. The
    callee is PersistError.
  - **CAIStateGroup_PrepareMeal::IsRunnable (0x00946400):** retail headers, a `CThingAICreature& creature` local and an
    `if (..) return true; return false;` tail.
  - **CTCTalk::OnCreate (0x006E2C00):** retail headers plus the GetSubDef<CCreatureDef> template body in the TU.
  - **CPhysicsMeshObject::GetBoundingBoxCentre (0x00A11AE0):** the landed 0x00A11C20 sibling model.
  - **CGPUScratchBufferTextureManager::AllocteMemoryPage (0x00F37660):** the shared vector with FABLE_RETAIL_STLP_VECTOR_C, so
    `vector::operator=` is defined in the TU.
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile`.
- **Parked 33** (`pending/2026-11-04_linux_hand760/differ/`). **25 of them got fewer than 10 grades**, 15 of those only one
  (`STATUS.tsv`). They are queued for follow-up chunk 763.
  - **0x0093D270:** a `const float&` GetProximityForNavigation gives RELOCATION_MATCH, but FableWin spells the getter by value.
    By the names-right rule the row stays parked on the PDB spelling.

## Headers (13 changed, each behind a new opt-in sub-macro; none changed in the repo since the chunk base)
- For the matched rows: FABLE_RETAIL_CTCBASE_WORLDSEED, _CTCTALK_ONCREATE, _CCREATUREDEF_DIALOGUEVOICES,
  _CTHINGAICREATURE_HOMEBUILDING and _CPHYSICSMESHOBJECT_CENTRE.
- For parked drafts: _CMAP_THEMEPALETTE, _CTCCREATUREOPINIONOFHERO_DEEDS, _STLP_LIST_POPBACK, FABLE_STLP_OUTOFLINE_UNINIT_COPY,
  _STLP_VECTOR_PUSHBACK_OUTOFLINE, _CCOMBATMANAGER_PROXIMITY, _CTCCREATURENAVIGATION_NAVIGABLE and _CINPUTEVENT_COPY.
- hdrcheck2: `default=same` for every file. `ppcmp.py`: all 3,968 includers same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04w.tsv` (12).
  - Four of them qualify bare or mangled catalog names that vland wrote. Those were 0x00946400 `?IsRunnable@..`, 0x009BBBF0,
    0x00CFCCA0 and 0x006399B0.
  - `finish_landing.sh` now does this automatically from the manifest module.
- **Queued PRIORITY** (`land0411w`, 17), among others:
  - 0x009BAA90 PersistError;
  - 0x00946380 CAIStateGroup_PrepareMeal::GetFreeCookingPot;
  - 0x00878A80 / 0x00878BE0 GetCamPosOffsetXY / GetPosFromParams (private const);
  - 0x00686DA0 CTCBase::GetPWorldSeed;
  - 0x00487DC0 GetPControlledCreature;
  - 0x009F5690, a CInputEventPackage constructor (not Save).
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0, after the four labels above.

## Coverage
46,699 genuine of 49,568; 5,819,992 function bytes (56.94%).
