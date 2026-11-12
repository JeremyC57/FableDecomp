# Chunk 2053 header overlay (row 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction)

13 files, all under `engine/`. Every change to a repo header is an opt-in `FABLE_RETAIL_*` block; the default text of every
repo header is unchanged (checked: none of the new macros occurs anywhere in rebuild/src/compiled, rebuild/include or
rebuild/tests; `g++ -E -P` of each changed header against its repo copy is identical with no defines and with every macro the
repo copy mentions defined; the scout's sweep over every define set of the landed includers found 0 differences). No landed
row needs a regrade.

The overlay holds only what this row uses. The chunk-2051 headers that were carried along in this directory (CWorld.h,
CCreatureActionBase.h, the front-end set, fable_retail_banks.h ...) are not needed by the row (it defines none of their
opt-ins; the row grades RELOCATION_MATCH without them, grades.log line 58) and were removed; they are in
rebuild/candidates/manual/pending/2026-11-12_acctB_c2051/hdr/. The variant texts tried while fitting the vector helpers
(DOT_P2 / P3 / P4 / P5 / PAIR / NAMED, MAGNITUDE_G1 / G2 / G3 / PAIR / RUNNING / ONELINE, SQUAREDMAGNITUDE_RUNNING / NAMED) are
gone; a copy of that sweep overlay is kept in ../scout/hdr/ for reference only.

## New headers (no repo copy, no landed includers)
- `engine/CTCRotate.h` -- CTCRotate over CTCBase, Ego_r.pdb layout (56 bytes; Rotating +0x28). Inline `SetRotating(bool)`
  (FableWin.pdb ?SetRotating@CTCRotate@@QAEX_N@Z, debug copy 10i/22b = our /Od text; retail `mov byte ptr [edi+28h],1`
  0x0087FC11) and the out-of-line `SetRotationInfo(const C3DVector&, float, float)` 0x0072A530 (Ego_r + FableWin; landed row).
- `engine/NEntityEvents_CEventKicked.h` -- NEntityEvents::CEventBase (WorldFrame +4, inline ctor and inline virtual dtor) and
  CEventKicked (vtable 0x01277B5C by RTTI: slot 0 0x0087FEA0, slot 1 GetType 0x0087E760). FableWin.pdb
  ??0CEventKicked@NEntityEvents@@QAE@XZ 12i/31b and both destructors = our /Od texts. Not combinable with
  engine/NEntityEvents_CEventHitBase.h (that header declares CEventBase's destructor out of line).

## Opt-in blocks in copies of repo headers
Shared inline BODY text (the only two; both decide x87 operand order where the row expands them):
- `C3DVector.h` **FABLE_RETAIL_C3DVECTOR_MAGNITUDE_XY_THEN_Z**: `float GetMagnitude()const{float squared=(X*X+Y*Y)+Z*Z;return
  (float)sqrt(squared);}`. PROVEN by debug size plus the grade: 26i/66b at plain /Od = FableWin.pdb
  ?GetMagnitude@C3DVector@@QBEMXZ 26i/66b (the repo default text is 13i/31b), and the row grades RELOCATION_MATCH only with it (repo default GetMagnitude: DIFFER,
  md.py equal count 710 against 715 for the match, grades.log line 61; the same-size text without the parentheses
  `X*X+Y*Y+Z*Z`: DIFFER 710, line 49).
- `C3DVector.h` **FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z**: `const float Dot(const C3DVector& other)const{float
  dot=(X*other.X+Y*other.Y);dot+=Z*other.Z;return dot;}`. FITTED: 23i/60b at /Od against FableWin 23i/59b (fwsize counts it
  SAME; the repo default one-line sum is 21i/51b; the landed running-sum GetSquaredMagnitude2D shows the same +1 byte, 18i/44b v 18i/43b). Of 26 natural Dot texts the
  scout measured, the only one that is exactly 59 bytes (a reference-alias text) grades DIFFER (711, line 52). The same text
  without the parentheses grades DIFFER (714, line 42), the repo default one-line sum DIFFER (711, line 62). Only the row
  grade picks it. The match is not on an edge: it holds with 1, 2 and 4 extra named locals in the row (lines 63-65,
  diagnostics only).
  Both blocks sit ahead of the existing `#elif` chains (DOT_GROUPED / DOT_ORDERED_XYZ, MAGNITUDE_SQRTF), which are unchanged.

Declarations and layouts (no shared inline body changed):
- `CCombatAction_KickThingOnGround.h` **FABLE_RETAIL_CCOMBATACTION_KICKTHINGONGROUND**: the class over the retail
  CCombatActionBase (FABLE_RETAIL_CCOMBATACTIONBASE_MELEE, 0x114 bytes) with the Ego_r.pdb members MoveToPos +0x114, Movement
  +0x120, MaxRunningSpeed +0x12C (PDB offsets minus 0x18; the landed ctor 0x0087FD20, Initialise 0x0087E860 and GetNextPos
  0x0087E770 read them there) and the PDB virtuals. sizeof 0x130 asserted.
- `CKickableDef.h` **FABLE_RETAIL_CKICKABLEDEF**: the sub-def over the retail CDefClassBase with the Ego_r.pdb members
  (BaseDistanceToTravel +0x28 .. IgnoreNavigationTest +0x50, sizeof 0x54; OpinionDeedType / OpinionDeedTypeToEnemies typed
  EOpinionDeedType) and the inline `StatGetTypeName()` that GetSubDef<CKickableDef> needs.
- `CThingShotDef.h` **FABLE_RETAIL_CTHINGSHOTDEF_CLASSNAME**: member ShotDefinitionClassName typed CDefString (same offset
  +0x60 and size; retail `lea ecx,[ebx+60h] ; call CDefString::GetString` 0x0087F7F9).
- `CHitParameters.h` **FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR**: declarations of the (CHitLocation*, CThing*, CThing*)
  constructor 0x0087FF60 (Ego_r + FableWin; landed row) and AddDamageWithAttribute 0x00868CA0 (Ego_r + FableWin).
- `CThingCreatureBase.h` **FABLE_RETAIL_CTHINGCREATUREBASE_SETATTACKEDBYCREATURE**: declaration of
  ?SetAttackedByCreature@CThingCreatureBase@@QAEXPAV1@@Z 0x006624F0 (Ego_r + FableWin; landed row).
- `CThingObject.h` **FABLE_RETAIL_CTHINGOBJECT_RESETDIEOFFTIMER**: declaration of ?ResetDieOffTimer@CThingObject@@QAEXXZ
  0x00702CE0 (Ego_r + FableWin; landed row).
- `CThingDeadCreature.h` **FABLE_RETAIL_CTHINGDEADCREATURE_GETANIMATIONFILTER**: inline `CAnimFilterBase& GetAnimationFilter()
  const { return *PAnimFilter; }` (FableWin.pdb only, debug copy 10i/25b = our /Od text; retail reads +0xD0 in place,
  0x0087F686). Needs the existing _ANIMATION opt-in.
- `CTCAnimatingObject.h` **FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION**: declaration of PlayAnimation(long, long, bool)
  0x007F9010 (Ego_r + FableWin) and the inline `StatGetInterfaceType()` (TCI_ANIMATING_OBJECT 0x83; FableWin 5i/10b = ours).
- `CTCDParticleEmitter.h` **FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE**: declaration of the static __fastcall
  Create(long, const C3DVector&, bool) 0x006E0880 (Ego_r + FableWin) in the short (non-FULL) branch, with its
  `class C3DVector;` forward declaration inside the same opt-in.
- `NThingShot_CTCShotDefinitionBase.h` **FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION**: declaration of the
  `const C3DVector&` overload of Init, 0x007BF790 (Ego_r + FableWin list two overloads, `ABVC3DVector@@` and `PBVCThing@@`;
  the body at 0x007BF790 copies three dwords from that argument into this+0x51, 0x007BF895..0x007BF8AC: the vector overload).
  Needs _INIT.

## Header debt seen, not touched (not needed by this row)
fwsize DIFF against FableWin for repo default texts: C3DVector operator- / operator+ 28i/70b v 34i/90b and operator*(float)
25i/62b v 31i/82b (the named-result pattern of the C2DVector lesson), C3DVector::GetSquaredMagnitude 21i v 23i,
C3DVector::ToC2DVector 16i v 18i, C2DVector::GetSquaredMagnitude 16i v 18i, CThing::IsDead 14i v 10i. The row matches with the
repo texts of all of them.
