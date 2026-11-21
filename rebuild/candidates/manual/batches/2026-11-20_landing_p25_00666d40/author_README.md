# 0x00666D40 CThingCreatureBase::GetHeightZBelowAt (p25R, 2026-11-21)

**Grade:** RELOCATION_MATCH (348/348). Strict vland (LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`, fresh work dir): WIN, behaviour PASS,
no warning (`vland.log`). No flag, no pragma, **no Inferred line**, no NAME_FIXES. Supersedes the bracket in
pending/2026-11-20_p25_open/00666d40 (r1, commit 9b15a3ded).

## What closed it (advisor check 3: to_centre's type, and a by-value call built straight into a local)
Ego_d locals `{ from, to_centre, to, get_height_below_func }` (to_centre: C3DVector 0x2460, bprel -64) and lines 2782..2786 =
27 / 16 / 12 / 12 / 12 B. to_centre is the **offset** from the position to the creature's centre, not a point:
```
C3DVector to_centre(0.0f, 0.0f, radius);   // 2782: VC7.1 /Od 16 B + 3 (float variable) + 4 + 4 (two 0.0f literals) = 27
C3DVector from = pos + to_centre;          // 2783: 16 B both (hidden return slot + operand + this + call)
C3DVector to = from;                       // 2784: 12
from.Z += 1.0f;                            // 2785: 12 (`+= extra_size_area` is 9)
to.Z -= 50.0f;                             // 2786: 12
```
- Full-function map (check 1, `egod_lines/00666d40.odcmp_final.txt`): every other line is equal or off by the known VC10 deltas
  (2771 5 v 7 fld1; 2788 23 = 20 + 3 and 2793 79 = 76 + 3, one float-variable argument each; 2795 10 v 11). So nothing outside the block
  reads to_centre; it is read once, by 2783.
- Calibration of the literal deltas, from landed rows whose neighbouring lines align exactly:
  `C3DVector offset(0.0f, 0.0f, 0.0f)` is 14 B at VC7.1 /Od and 26 in Ego_d (0x0075C420 line 206: +4 per 0.0f literal,
  `push 0` v fldz / push / fstp); `cost.LightCount += 1.0f` is 18 in both (0x00B187D0 line 74: VC10 keeps `fadd [const]` for
  `+= 1.0f`, so 2785 is 12 with the literal and 9 with the variable).
- Same idiom in Ego_d's physics code: CTCPhysicsControlled::UpdateOnSurface has offset locals `up` / `down`
  (`egod_lines/idiom_00725290_UpdateOnSurface.locals.txt`).
- Release: from and to share one pos.Z + radius on the x87 stack (`fld st0; fxch st1; fadd [1.0]`), the shape r1 reached with copies;
  the operator+ form gives the same 348 bytes.
- Functor: the TU's anonymous-namespace CThingFindHeightZBelowAt (Ego_d TPI 0x4EE6C, `egod_lines/CThingFindHeightZBelowAt.tpi.txt`):
  public ctor / operator() const / GetHeight, private `const C3DVector&` From / To and float Height. Ctor and GetHeight are one-line
  in-class bodies (Ego_d lines 146 / 170, 42 / 17 B, `egod_lines/CThingFindHeightZBelowAt.ctor_getheight.txt`); operator() is the landed
  0x00668AB0. The TU-mate 0x0066A1E0 declares the same class (ctor / GetHeight without bodies, since it needs none).

## Header changes (overlay `hdr/`, `hdr.diff`, strictly add-only, no assert in either header)
- `engine/CThingFilter_SolidToThing.h`: opt-in FABLE_RETAIL_CTHINGFILTER_SOLIDTOTHING_CTOR_BODY, the inline constructor body
  (Ego_d ctor lines 60 / 61 / 62 = 15 / 10 / 9 B; retail keeps 0x00661620 out of line and expands it here, 0x00666E74..7C).
- `engine/CThingFilter_PhysicsFlagsSolidToThing.h`: opt-in FABLE_RETAIL_CTHINGFILTER_PHYSICSFLAGSSOLIDTOTHING_CTOR_BODY (Ego_d ctor
  lines 40 / 41 = 29 / 9 B).
- No new header file, so no newly flagged rows.

## Ablation (one change at a time; sources in `evidence/`)
| change | grade |
|---|---|
| final | RELOCATION_MATCH |
| no overlay (both ctors out of line) | DIFFER(339v348) |
| without SOLIDTOTHING_CTOR_BODY (`66_ab1_no_solid_ctor.cpp`) | DIFFER(342v348) |
| without PHYSICSFLAGSSOLIDTOTHING_CTOR_BODY (`66_ab2_no_flags_ctor.cpp`) | DIFFER(343v348) |
| `from.Z += extra_size_area` (`66_a1_extra_size_area.cpp`) | RELOCATION_MATCH (same bytes; rejected on Ego_d 2785, 9 v 12) |
| r1 copies, no to_centre (`66_r1_bracket.cpp`) | RELOCATION_MATCH (rejected: Ego_d 2782 / 2783 27 / 16 v 12 / 9, no to_centre) |
| `to.Z -= 50.0f` before `from.Z += ...` (`66_a4_order.cpp`) | DIFFER(346v348) |
| to_centre as a point, copies (`66_r2_to_centre_point.cpp`, `66_c1_to_centre_point.cpp`) | DIFFER(356v348), DIFFER(347v348) |

## Tests
- `test/00666d40.cpp` (the payload's test, vland behaviour PASS) copies the row's statements and functor class over stand-ins with the
  retail layouts. The row object cannot be linked outside one fixed path: it calls ForEachInRoughArea<..., `anonymous
  namespace'::CThingFindHeightZBelowAt>, whose ?A0x<hash> depends on the compile path (the 0x0055F710 / 0x006F2DE0 precedent).
- `test/00666d40_realobj.cpp` links the REAL row object at mut.py's fixed path (`ANON` = 9b063da1 for $W/tmp/mut/m.cpp). VC7.1 puts its
  own /alternatename for that template symbol in the row object, so the test object must come first on the link line:
  `mutants/mut2.py` is mut.py with that one change.
- Fixtures and checks (both tests): creature image (PhysicsTC +0x60, Radius +0x34), theme / ground height doubles (called once, on `pos`
  itself), a ForEachInRoughArea double that records the box, the three bound filters and the functor, then applies the 0x00668AB0 rule
  to a list of hit heights. 7 cases: theme / ground without things (no search); with things the box is pos -+ (radius + 1), truncated
  (negative coordinates too), SolidToThing(this) with no solid TC, PhysicsFlagsSolidToThing(this) with its physics TC, OverlappingCircle
  (pos, radius), from = pos + (0, 0, radius + 1), to = pos + (0, 0, radius - 50), the functor starts at the map height, and the result
  is the highest valid hit (hits above `from` or below the map height rejected); radius 0.
- **Mutants:** 16/16 KILLED (`mutants/00666d40.out`): 14 in the source, 2 in the overlay ctors.
