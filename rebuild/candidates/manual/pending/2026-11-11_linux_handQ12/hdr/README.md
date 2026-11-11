# Chunk Q12 header overlay (grade with FABLE_HDR_OVERLAY=<this dir>)

Copies of repo headers (rebuild/include, HEAD at the start of the run) with new `#ifdef FABLE_RETAIL_*` opt-ins; every default path
is byte-identical to the repo copy, and the new headers are only reached by an explicit include.

| File | Opt-in / status | Used by | What it adds |
|---|---|---|---|
| engine/CQ_HobbeCaveScript.h | FABLE_RETAIL_CQ_HOBBECAVESCRIPT_HOBBEROAMING | src/00d91960 | nested entity script CHobbeRoaming (Ego_r.pdb 28 bytes: ParentClass +0x14 `CQ_HobbeCaveScript*`, PMasterData +0x18), Main private (EAEXXZ), Init / OnPredicateFail / GetParentScript declarations; sizeof 0x1c assert |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ | differ/008b6ad0 | Dot as running statements Y, X, Z (0x008B6BBC product order; one fxch still left) |
| engine/CRGBColour.h | FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR | differ/009dd960 | Ego_r/FableWin ?GetD3DCOLOR@CRGBColour@@QBEKXZ, inline A,R,G,B pack |
| engine/CRenderManager2D.h | FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DLINE | differ/009dd960 | Draw2DLine (public) + private Add2DLine 0x009DC2C0 declarations |
| engine/NThingShot_CTCShotDefinitionNymphFun.h | new header | differ/007bc004 | NymphFun : CTCShotDefinitionProjectile (RTTI), public InitMovement, TrapCreated +0xa4, sizeof 0xa8 |
| engine/CCreatureAction_MidAttackSide.h | new header | differ/008b6ad0 | MidAttackSide : CCombatActionBase (RTTI), Initialise, sizeof 0x114 |

Only the CQ_HobbeCaveScript.h opt-in is needed by a src/ row.
