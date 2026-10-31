# Chunk 613 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- "Frame slot assignment" residues on vector maths can be a missing user copy constructor: with
  `C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}` and the points as NAMED C3DVector copies (not scalars), VC7.1 homes
  the whole 12-byte object (one word may stay on the x87 stack) and the frame shrank 0x38 -> 0x30 to retail's (0x00760C40, 99 -> 9
  lines). The same copy ctor put 0x00744B50's wind copy above the direction as retail.
- An inline helper that rescales its own by-value parameter (`C3DVector operator/(float f) const { f = 1.0f / f; ... }`) homes the
  reciprocal in a dead argument slot and multiplies from memory (`fstp [slot] ; fmul [slot]`); a separate `float inv` local stays
  on the x87 stack (`fst ; fmul st(1)`) (0x00760C40).
- A zero store scheduled between two halves of a struct copy is the declaration order: `C3DVector first = *it; float covered = 0;
  C3DVector prev = *it;` (0x00760C40, last 2 lines).
- Landed sibling first, again: 0x00EAD9F0 matched on the first grade with the landed CV_MurderTwistScript::MagicRuneProcess model
  (NScript, retail FABLE_RETAIL_NSCRIPT headers, `PImp->IsEqualTo(*other.PImp)`) after two sweeps called the Info-reload register a gap.
- Re-prove virtual identities through every referencing vtable (`xref.py <addr>`): 0x007BEE40 sits in 14 vtables including the base's,
  so it is the base's UpdateFacingDirection, not a Throw override.
- Residues no lever moved here (park after 2-3 grades): one Bind3 pointer store one instruction early (0x0063E590), CRightHandedSet
  temp packed over a by-value call-result temp (0x007BC004; naming the result fixes the frame but copies via lea), x87 commutative
  operand canonicalisation in sums/cross products (0x007BEE40, 0x00654438), an empty comparator temporary given its own frame
  byte (0x00744FB0), equal-use esi/edi colouring (0x007FE990, 0x00939670).

Review at landing:
- 0x00EAD9F0 emits COMDATs for the inline ~CScriptThing / vtable ??_7CScriptThing (as the landed NScript siblings do); only the
  GemsFoundProcess leaf is graded.
- NAME_FIXES.tsv: respell the landed 0x0093AE80 (IsCurfewTime -> IsSociableTime const, GetVillage -> GetPVillage const),
  0x00938F70 (IsSitOutsideTime / GFGetPVillageInterface), 0x007C21A0 slot 36 name, 0x00744D00 (CTCPerceiveAlert::FindAlertEvent ->
  CTCPerceiveThing::ReactToAlerts), 0x007449B0 (OnDirectlyAwareOfPerceivedThing -> SetLastAwareInfo), 0x004197B0 (T stand-in),
  0x007BEFB0 (GetPRandomSeed -> GetWorldSeed), the Normalise / ApplyAcceleration / GetPosition callers.
- 0x00654438 (retail_asm grade) is parked in retail_asm/differ/ (it carries the inline-asm ceil); no retail_asm payload this chunk.
- Rows above 6 grades: 0x0063E590 (8), 0x007BC004 (8), 0x007BEE40 (9), 0x00744B50 (9), 0x00760C40 (6).
- Parked 0x007BC004 still declares GetWorld/GetWorldMap on the derived class (mangles NymphFun::GetWorld); they belong on CTCBase
  (IBE / IAE) once a CTCBase base is modelled.
