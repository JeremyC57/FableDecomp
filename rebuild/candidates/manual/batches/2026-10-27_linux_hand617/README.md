# Chunk 617 (fourteenth parked-row chunk from the sweep ranking, 10 same-length rows 584-619 bytes): 2 of 10 landed

Landed (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0): 0x0096BCB0 CThingSearchTools::GetAllThingsInRoughArea<CThing,
vector<CThing*>, CThingFilter_HasDefGlobalIndex> and 0x00919780 GetAllThingsInRoughArea<CThingGameObject, vector<CThingGameObject*>,
CThingFilter_HasTC> (TypeMask 7). The catalog and every parked attempt called both GetPBestThingInRoughArea; copying the landed
sibling 0x008E3EB0 and changing only the template arguments matched both -- check a parked template row's identity against landed
siblings first. Parked 8 (pending/.../differ/), two improved: 00f3abf0 CalcCompressionScript (explicit guard + do-while sinks the
pointer load below the guard; one `mov ebx,edi` before the jle left), 006ad700 ControlledMove (Y,Z,X named compound dot product
fixed the term order; the x87 operand form of one product left); 0072c430 ParentThing in a rotating scratch register where retail
reuses the dead this, 00555a90 child-loop colouring, 0063e6f0 list head kept in esi across the search call (whole-program), 00621a20
list node / vptr colouring, 00965640 ebx/ebp swap, 006a4fb0 final Normalise stores not interleaved with the epilogue pops. Inert
levers for ebx/ebp swaps: named pointer locals and self aliases, named filter locals, Normalise as r*X or through a component
pointer. 29 NAME_FIXES + 5 review rows queued (prefix c617: GetWorld 0x004C7AA0 protected IAE not PeekWorld, GetBuildingThing,
SetActionPointAsFree, FinishCurrentAction, the CTCCarrying reference forms, RemoveMode QAEXK, GetSubDef<CCarryableDef>,
HideComponent / UnhideComponent IAE, GetFocalPos, GetHeadPos, PeekThingSearchTools, GetQuestCards, ~CPhysicsMeshObject UAE).
