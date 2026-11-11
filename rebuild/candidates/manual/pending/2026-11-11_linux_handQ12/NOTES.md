# Chunk Q12 notes (grades with FABLE_HDR_OVERLAY=$A/hdr exported, via tools/gg.sh)

Result: 1 match (0x00D91960, RELOCATION_MATCH on the shared headers), 39 parked in differ/ (5 reworked onto the shared headers or
probed, 34 regraded once as they sit). Every row was regraded as it sat first (bin/regrade.txt); 7 drafts had no `// Leaf:` line,
which was added from the PDB name sets.

## Matched
- 0x00D91960 NScript::CQ_HobbeCaveScript::CHobbeRoaming::Main -- identity: Ego_r.pdb + FableWin.pdb ?Main@CHobbeRoaming@CQ_HobbeCaveScript@NScript@@EAEXXZ,
  "HobbeRoaming" Alloc 0x00D8F4F0. Lever: `while (sacrifice.IsAlive())` (was `for(;;){if(!alive)break;`) + the Q10/Q11 idle tail. Moved from the
  317-line local model onto fable_string.h, CCountedPointer, CScriptThing, CQ_HobbeCaveScript (new opt-in FABLE_RETAIL_CQ_HOBBECAVESCRIPT_HOBBEROAMING),
  CScriptGameResourceObjectScriptedThingBase (OUTOFLINE_DTOR), CGameScriptInterfaceBase, CIsThingAlive, fable_script_distance, STLport vector with
  `template<> std::vector<CScriptThing>::~vector();` (0x008AC970). Row-local inline bodies after the headers: CIsThingAlive ctor/dtor and
  CScriptThing::IsAlive (0x004AB130, devirtualised on the local sacrifice). Callee spellings: SetPredicate (0x00F35B10; the old draft's
  RegisterFilter is wrong -> NAME_FIXES), IsValid (0x00CD23B9; draft said IsScripting).

## Reworked, parked (best source in differ/, residue on line 2)
- 0x008B6AD0 CCreatureAction_MidAttackSide::Initialise: migrated (new engine/CCreatureAction_MidAttackSide.h, CCombatActionBase MELEE/INITIALISE,
  callee SetCombatAnimation 0x008574D0 -- the draft's SetAnimationName was descriptive). One `fxch st(2)` left at 0x008B6BBC (which of normalised Y
  and -facing.X stays as the dead value). Next: c2 x87 allocator probe on that product; Normalise opt-in variants (LOCALS / SQUARE_HELPER) untried.
- 0x00DD9520 CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene: migrated (the PDB name; old draft called it PrisonEscape). One fstp slot.
  Note it needs FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CDEFSTRINGTABLE for CScriptDef.h (without them cl spins to the timeout).
- 0x009DD960 CRenderManager2D::Draw2DLine: migrated (CTVertexRHWColSpecTex1 PDB field names DiffuseColour/SpecularColour/U1/V1, GetD3DCOLOR,
  Draw2DLine/Add2DLine opt-ins). RHW integer-copy residue (see LESSONS). Header debt: 0x0139652C name (bound g_ScreenRHW, probably BIGGEST_RHW_VERTEX_VALUE).
- 0x007BC004 NThingShot::CTCShotDefinitionNymphFun::InitMovement: migrated onto NThingShot_CTCShotDefinitionBase.h (MOVEMENT) + new
  NThingShot_CTCShotDefinitionNymphFun.h. Residue: Randomise result temp slot vs eax copy, and the fild/this-reload order (details on line 2).
- 0x0095BCF0 CAIStateGroup_HoverHornetQueen::Attack: probe only on the local model (Newton grouping moves the `mov ecx,edi` one slot); the repo
  header CAIStateGroup_HoverHornetQueen.h is a flattened generated layout, so migration needs an AI state-group class model first.

## Regraded only (34 rows)
All are local models (or partial ones: 0063d0a0 / 0063d4a0 / 009b9770 already use shared headers; 00666d40 uses _quarantine headers through
macro renames and must be redone). Their line-2 residue quotes the earlier PARKED_INDEX diagnosis. Rows with a local fast normalise (00852e10,
00935580, 00a177a0, 00a18fc0, 00a364e0, 00b45a40, 0063d0a0/0063d4a0 use the header already) are the best next candidates: the 0x008B6AD0
experience says the header Normalise alone can fix the component-product order. Rows with no class header yet: 00852e10 (NCameraModes script
modules), 00873bc0 / 00874790 (CPCMouseLook), the C3DMesh2 trio, the AI state groups (flattened generated headers only).
