# Workflow wf5 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 009adae0 -- VERIFIED, author attempts 1

- What made it match: The overlay rebase is only a header change, so the bytes are the same as before. The match still comes from the inline CDefClassBase::SetAsTemplate(false) setter together with the InstantiateDef idiom.

## 00499aa0 -- VERIFIED, author attempts 1

- What made it match: Moving the row off its local structs onto the shared engine headers, and spelling both strings as implicit-conversion temporaries: `PFont = GFGetSystemManager()->GetFontManager()->GetFont("ENG_ARIAL_16");` and `RenderProgress("Initialising progress display", -1.0f, false, renderFlag);`. With the real CCountedPointer operator= / ShareData / ReleaseData bodies from engine/CCountedPointer.h, VC7.1 calls the operator= COMDAT (0x00419463) with the returned pointer and expands the temporary's destructor, keeping its `Data = 0 ; Info = 0` stores. The old 410v418 residue came from the local model's bodiless operator=, not a compiler tie. Other fixes: IsEditorActive (was IsDebugDisplayForced) reached through CMainGameComponent::Get(), and CFontManager (was CFontBankBank) reached through GetFontManager(). No new header opt-in, no pragma, no compiler flag.

## 00ef4320 -- VERIFIED, author attempts 1

- What made it match: Shared headers (CPersonalScript_EmptyScript.h entity opt-in, CGameScriptInterfaceBase.h, CIsThingAlive.h with the inline ctor defined in the row and the dtor left out of line), corrected callee names SetPredicate / IsActiveThreadTerminating, FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY, and flag /d2inlT28. With the copy implicit, the T23..T28 budget expands the two Set* argument copies and calls 0x004ABE90 for the predicate's Thing and the two Reset* arguments, as retail does. T28 is also inside the T28..T36 window recorded for sibling CTestPerson::Main 0x00EF4B40, so one budget fits both. Control flow is unchanged from the old draft. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00ef4320/00ef4320.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00ef4320/test.cpp; payload.json and oracle.tsv written by addmatch.py in the same directory.

## 00a11d10 -- VERIFIED, author attempts 1

- What made it match: The mesh-space point is a named C3DVector local filled through the two-argument inline CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) const (FableWin.pdb name, three plain `E1k*x + E2k*y + E3k*z + E4k` rows, forced inline), not `inverse * pos` through operator*. Body: `if (!Mesh->GetNavigationContainmentVolumes().empty()) { CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix); C3DVector local_pos; inverse.TransformPoint(pos, local_pos); return Mesh->GetNavigationLevelAt(local_pos); } return 0;` with the shared header's SetInverse body unchanged. That alone gives retail's 1,3,2 order in the determinant and the stack-held translation sum. Final files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11d10/00a11d10.cpp and /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11d10/00a11d10_test.cpp; payload.json, oracle.tsv, NOTES.md, LESSONS.md, STATUS.tsv, LABELS.tsv, NAME_FIXES.tsv and hdr/README.md are in the same directory.

## 007f53b0 -- VERIFIED, author attempts 1

- What made it match: The source did not change. The rebased overlay keeps the only opt-in this row needs (MULTIMAP_CONSTFIND) and no longer reverts the landed TREE_BOUNDS_OUTOFLINE blocks.

## 009692c0 -- VERIFIED, author attempts 1

- What made it match: Moving onto the shared engine headers fixed the frame (CCreatureAction_PlayAnimation sizeof 0xB4 with implicit dtor), callee signatures (const-ref WalkToPoint / AddMessage / PlayAnimation / PerformActionAndWait), and register choices (temporaries and inline GetCreature() in the AddMessage call, GetWorld().GetMessageEventManager() as one expression). The last residue, the first exit block being sunk to the end of the function, went away when the early exits were written as two separate `if` statements with identical bodies (`if (!WalkToPoint(JugPos, JugRHSet)) {...}` then `if (GetPaymentCancelledMessage()) {...}`) instead of one `||` condition: VC7.1 merges them into the in-place block at 0x9692F0.

## 0094d780 -- VERIFIED, author attempts 1

- What made it match: Unchanged: the shared headers with the real GetDef/GetSubDef template bodies visible and out of line (noinline opt-ins), plus the overlay opt-ins TROLLBASE and CEXPLOSIONDEF_MEMBERS.

## 0064c869 -- VERIFIED, author attempts 1

- What made it match: Two things. First, the port onto the shared headers with the proven identities (CPlayer& from GetPlayer, const CThingPlayerCreature* hero from GetPControlledCreature, CThing::PeekTC(TCI_TARGETING) expanded, `GetPTargetedThing() != PTargetedThings[i]`, PeekDisplayEngine().IsLetterBoxMode(), SetAsDrawing(true), `pos.Z = pos.Z + GetPhysicsTC()->GetRadius() + 0.35f`, GetMultiplayerColour(player.GetNumber())). Second, the Q32 lesson: IS_THING_ALIVE's real body visible but kept out of line (engine/ThingAlive.h, FABLE_RETAIL_IS_THING_ALIVE + FABLE_RETAIL_IS_THING_ALIVE_NOINLINE, plus FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT and _DEREFERENCE for the CIntelligentPointer<CThing> instantiation). With the body visible the hero stays in edi and its home word [ebp-10h] is shared with PeekTC's key and the gui def reference, as in retail. Source carries #pragma optimize("y", off) + ("s", on).

## 0095e0b0 -- VERIFIED, author attempts 1

- What made it match: Three things. (1) The port onto the shared headers (real class with the PTarget member, shared CCreatureActionBase / Aim / Fire models, ThingAlive.h IS_THING_ALIVE, inline copy ctor via FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) fixed the 0xCC frame, and the mid-function switch from inline to out-of-line for IS_THING_ALIVE, the pointer copy and the action dtors came by itself. (2) The esi/edi swap was the exit structure: each failed step is its own `{ Terminate(); return; }` instead of nested ifs falling into one Terminate(). (3) The fire result is a named local (`mov ebx,eax`); the aim result is not named. CCreatureAction_ReloadMissileWeapon has no engine/ header, so it is modelled in the row file from the PDB members (implicit dtor, PAmmo +0xA8, sizeof 0xB0). No made-up names. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/0095e0b0/0095e0b0.cpp

## 00a11f20 -- VERIFIED, author attempts 1

- What made it match: Shared headers only (no local models): `float scaled_radius = radius * OO_Scale;` first, an in-place `CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix);` with the header body unchanged, then `C3DVector local; inverse.TransformPoint(pos, local);` with the two-argument TransformPoint force-inlined as one plain left-to-right sum per component, and the real callee `Mesh->HasBoundingBoxIntersectedFromTo(scaled_radius, local, local)` (0x00A17690). The context of the inline transform decided the operand order of the E11 products and the first-two-term order of the det / E43 / Z-row sums; source term order inside a sum never mattered. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11f20/00a11f20.cpp (source), /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11f20/00a11f20_test.cpp (test), /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11f20/payload.json, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11f20/oracle.tsv, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11f20/NOTES.md, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf5/00a11f20/hdr/README.md. The stale parked draft is still in differ/00a11f20.cpp.

