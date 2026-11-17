# NF74 notes (round 7, 2026-11-17)

## 0x006C9B70 std::__copy (q10747) -- ported
NF70's held draft (batches/2026-11-17_landing_nf70/held/006c9b70.cpp) matched with a whole local std model including a new
`std::_Nonconst_traits`. The port deletes every local std class: pair / vector / back_insert_iterator / _Rb_tree_iterator /
_Rb_global come from stlport/fable_containers.h + stlport/fable_map.h, and the input-iterator `__copy` is the header's
`FABLE_RETAIL_STLP_COPY_INPUT_NOINLINE` body (NF62's opt-in for the caller 0x006C7370). Existing opt-ins only
(FABLE_RETAIL_STLP_VECTOR, _BACK_INSERTER, _COPY_INPUT_NOINLINE, _VECTOR_OVERFLOW_OUTOFLINE): no overlay header. RELOCATION_MATCH
on the first grade; the only relocation binds 0x006C80E0 as the STLport protected spelling
`?_M_insert_overflow@?$vector@U?$pair@JPAVCOpinionDeed@@@std@@V?$allocator@...@@IAEXPAU...` (NF70's NAME_FIXES.tsv already files
the 0x006C80E0 self respelling; not repeated here). The test was rewritten on the same headers: it declares the instantiation
without a body and links the row's object (the old test re-modelled std and never linked the row).

## 0x008C2550 CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction (nf41:e2) -- ported, RM + PASS (8 grades)
Moved onto the Initialise sibling 0x008C12D0's block plus RushToPos 0x009735D0's CastAssassinRush / PlayCombatAnimation; 17 local
models and the stand-in bodies (GetSubDef, GetValidRushPosition, the CCountedPointer ctor) gone. The ctor binds the queued
`??0CCreatureAction_CastAssassinRush@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@M_NJV?$CCountedPointer@VCCreatureActionBase@@@@@Z`.
Frame 0x178 v 0x16C until GetValidRushPosition's body was visible (retail reuses the dead rush_to_pos words). That body is written
from the landed 0x008C1600 row with Ego_d locals and the shared-header types and is NOT graded at its own address; its phase filter
uses CVectorFilter_FurtherFromPointThan, whose type for that call is unproven (REJECTED.tsv: the XY-vs-3D operator() conflict).
Then NORMALISE_PAREN_SQUARES_ZYX and MAGNITUDE2D_ORIGINAL_SUM put the shared X*X / Y*Y products in retail's slots. Locals: Ego_d names.
NAME_FIXES: 0x008C1600 self -> the Ego_r/FableWin protected `C3DVector&` spelling. RENAMES section 2 lists the old binder spellings;
the PlayCombatAnimation `PAV` old spelling is read from the landed declaration (that row's reloc example is not in reloc_bindings).

## 0x008DDAD0 CModeThunderLightningStorm::CreateLightningBeams (nf44:50) -- ported, RM + PASS (15 grades), one FITTED opt-in
On CreateSkyBeam 0x008DAA80's block; 16 local models gone. Retail is LTCG: the first GetEffectFromString / push_back are calls, the
second expanded (with CCRC::Calc and the map find). Visible inline bodies + `/d2inlT65` give that split (T40 nothing inline, T60 Calc
called, T65 / T68 right, T70 / T80 / default everything inline; T61-T64 not bisected). The last residue, one SIB order
(`[ecx+esi]` at 0x008DDC00), only went with the NF74 overlay opt-in FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_NAMED_ARGS, which is
FITTED (length then text named before the CRC; Ego_d lists only `it` in GetEffectFromString) -- the coordinator should rule. Tried
without effect: an Ego_d-named Calc body, CCRC::CalcFromString, CCharString::GetCRC, T68.

## 0x008E0060 CModeGuildmasterPowerUpShield::OnActivate (nf44:50) -- ported, RM + PASS (6 grades)
On FrameUpdate 0x008E20F0's block; 27 local models and all stand-in bodies gone; real member names LightningBeamParticleEmitters / Guards.
Needed: CThing::GetPBaseDef by value (retail AddRef / Release round GetSubDef), CMODESTANDBASE_ACTIVATE, the visible rough-area
template body (frame 0x160 -> 0x158), a `class CTCMapwho;` forward declaration (the default CTCMapwho.h path needs it), and the NF74
overlay opt-in CASTPHYSICALSHIELD_CTOR (the ctor only). With the destructor implicit the temporary's release binds
??1CCombatActionBase@@UAE@XZ at 0x0062E900, the catalog's name there (a first draft that also declared a virtual
~CCreatureAction_CastPhysicalShieldSpell bound that name to 0x0062E900 instead; dropped, regraded RM, 7 grades in all).

## 0x008F3630 CCreatureAction_DragonRearUpFull::PerformAction (nf41:e110) -- ported, RM + PASS (10 grades)
On TrollThrowProjectile 0x008C6860's block; 29 local models gone. Binds GetSubDef<CSpecialEffectsDef> as the queued nf41:e110
spelling, GetDef<CThingShotDef>, the NThingShot Init(position) overload and AddThingToAvoid (NAME_FIXES: 0x007C0FB0, landed as the
descriptive AddHitThing). FITTED: a `const C3DVector& creature_pos` reference (not an Ego_d local) for retail's single GetPos and
(X, Y, Z + 10) construction. The existing no-code opt-in GFFASTONEOVERSQRT_NEWTON_PAREN gives the last x87 interleave.
nf41:e110 outside this slice: 0x008B9F20 and other 0x00669580 callers were not checked (see APPLIED).

## 0x00B9E950 / 0x00BD1F70 Render2DPrimitive (q10478; nf41:e120 e188, nf44:70 71 73) -- blocked/port, 1 NF74 grade each
Stopped for budget after the four rows above. NF74 added the row-class headers hdr/engine/CEnginePrimitiveManager2DStaticMeshes.h and
CEnginePrimitiveManager2DAnimatedMeshes.h (Ego_r member sets, no own layout) and regraded NF68's ports on them (work/att/): 11 and 13
error lines kept by the grader (not complete lists). The blocker is the 2D internal-primitive classes: engine/CEngineInternalPrimitive2DStaticMesh.h is a flattened pack(1) layout
with no base or methods, so the bodies' MeshInfo / LightArray / Initialise / SetLOD / InitialiseSimulation / IsSimulationRunning
(and SetupMesh2D for the animated one) have nowhere to bind. Next step: a retail branch of those two headers on the mesh-primitive
bases from struct_layouts_egor.tsv (own members from +0xAC), the C3DMeshInfo ISLOADED / GETMESH / BEGINLOADINGTEXTURES /
ARETEXTURESLOADED opt-ins, the NEngineInternal globals header, then the bodies respelled. STATUS.tsv has the exact errors.

## Rule slips
- One st.py STATUS note was passed in double quotes with backticks around `mesh`; bash ran it as command substitution (`mesh:
  command not found`) and the word dropped out. Fixed in STATUS.tsv with python; nothing outside my directory was touched.
- No git, no writes under /home/user/FableDecomp, every vland log and LANDVERIFY_WORK in this directory.

## Final recheck
recheck_final.log: 5 / 5 RELOCATION_MATCH + PASS (006C9B70, 008C2550, 008DDAD0, 008E0060, 008F3630). Its one WARN (RELOC_NAME:
0x008F3630 binds ?AddThingToAvoid@CTCShotDefinitionBase@NThingShot@@QAEXABVCThing@@@Z at 0x007C0FB0, labelled AddHitThing) is the
NAME_FIXES.tsv 0x007C0FB0 entry. Grades in all: 45 (STATUS.tsv grades_used). Header opt-ins: 8 in 7 overlay headers + 2 new
(unverified) headers; hdr/README.md lists them.
