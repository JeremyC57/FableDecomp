# 0x007BD2D1 NThingShot::CTCShotDefinitionThrow::OnSubShotTrigger (p25C)

**Grade:** RELOCATION_MATCH (598/598). The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, empty overlay `hdr/`; `vland.log`), and so does the real-object behaviour test.
- **Draft:** pending/2026-11-11_linux_handQ8/differ/007bd2d1.cpp. It used local models; on record DIFFER(599v598).
- **Mutants:** 20 of 20 killed (`mutants/`).
- **Oracle:** 598 bytes, the catalog length (`oracle.tsv`).
- **Flags:** none.
- **Pragmas:** the TU-mode pair `optimize("y", off)` + `("s", on)`, as in the landed siblings 0x007BCF7F / 0x007BD845. Line 2 names both.
- **Header changes:** none. `hdr/` and `hdr.diff` are empty, and newly_flagged_rows.tsv has the header line only.

## Source

The source uses shared headers only, with existing opt-ins. Every local name and block comes from Ego_d.pdb (`egod_lines/007bd2d1.locals.txt`); the C13 lines are in `egod_lines/007bd2d1.tsv`.

**Identity:** `?OnSubShotTrigger@CTCShotDefinitionThrow@NThingShot@@MAEXXZ` (Ego_r.pdb / FableWin.pdb). It is protected virtual, in the Throw vtable 0x0123CDC4.

Points that decide bytes:
- **`class_type_name`.** It is Ego_d's `const CCharString&`, bound to the CDefString -> CCharString conversion temporary. A by-value copy is not elided.
- **`rhs_to_use`.** It sits in its own block, as in Ego_d. It is a copy of `GetParentThing().GetPhysicsTC()->GetRHSet()`. CRightHandedSet's copy constructor is compiler-generated, and retail calls it out of line at 0x0049CF80, from 0x007BD3EC. Evidence:
  - Ego_d method list 0x9B49 marks it `public compiler-generated`.
  - The landed 0x005D8EF0 GetRHSetFromOrientation matches with this implicit copy called out of line at default flags.
  - The row object carries the implicit copy as its own COMDAT `??0CRightHandedSet@@QAE@ABV0@@Z`, 46 bytes. Its body is retail 0x0049CF80 (43 bytes: `mov eax,ecx ; mov ecx,[esp+4]` ... six dword moves ... `ret 4`) plus this TU's `push ebp / mov ebp,esp / pop ebp` frame. The linker kept a frameless TU's copy, so cmpcomdat reports DIFF on the frame bytes only.
- **`main_target_pos`.** It is unused, but retail still calls GetShootAtPos into its slot (0x007BD339).

## Inline budget (line 2) and ablation

There is no flag. Line 2 names the TU-mode pragmas and three existing header opt-ins that decide expanded vs called.

| change from the matching source | grade |
|---|---|
| without `#pragma optimize("s", on)` | DIFFER(625v598) |
| without `#pragma optimize("y", off)` | RELOCATION_MATCH: neutral here, kept for the siblings' TU pairing |
| without FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (plain inline operator==, called) | DIFFER(567v598) |
| without it, plus /d2inlS30 | DIFFER(624v598): the operator expands, but so does the implicit RHS copy |
| with it, plus /d2inlS30 | DIFFER(624v598): the RHS copy expands |
| without FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (GetDef declaration only) | DIFFER(614v598): CDefPointer null tests come back, and pshot_def is reloaded |
| without FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION (the header's inline forwarder) | DIFFER(598v598) |

**Why the operator== opt-in and not a flag.** In the trace (tools/trace.sh), the c2 size inliner flips the two callees in the wrong order:
- At the default settings, both `??8CCharString@@QBE_NPBD@Z` (sz 39) and `??0CRightHandedSet@@QAE@ABV0@@Z` (sz 46) are called.
- From /d2inlS21..S24 up, the RHS copy expands. The operator expands only from S25.
- /d2inlT0..T200 changes neither.

So no /d2inl value gives retail's pair: operator== expanded (0x007BD3A6..0x007BD3D4, the null test plus the intrinsic strcmp against "" plus CBasicString<char>::operator== 0x004115A0) and the RHS copy called. FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ is the existing per-TU opt-in for exactly this residue; 75 landed rows use it.

An earlier match (/d2inlS30 plus a new declaration-only user copy constructor opt-in in CRightHandedSet.h) was dropped. Ego_d marks that constructor compiler-generated, so the declaration would have been an invented member.

## Inferred (fitted choices, marked in the source)

| choice | alternative | grade |
|---|---|---|
| Init floats as member reads `DamageMultiplier, ShotBuildupTime` | FableWin accessors GetDamageMultiplier() / GetShotBuildupTime() (diagnostic overlay) | DIFFER(613v598) |
| `PShooter.Get()`, `PShotAbilityDef` | FableWin GetPShooter() / GetCombatAbilityDef() | neutral. The shared header has neither, so the members stay. |
| `GetParentThing()` at the Create argument | `ParentThing->` | DIFFER(598v598): the ParentThing load at 0x007BD414 is scheduled after the pushes |
| `GetParentThing()` at the two other sites | `ParentThing->` | neutral |
| `SubShotMaxAngle * -1.0f` | unary minus | DIFFER(594v598). Retail has `fmul [0x0122DEE0]`, and that constant is -1.0. |
| `2.0f * max / (nshots - 1)` | `max * 2.0f / ...` | neutral |

## Test

`test/007bd2d1.cpp` links the real row object, including the row's TU COMDATs: GetDef<CThingShotDef> with its expanded SetDynamicCast and Release, and the implicit CRightHandedSet copy. Every remaining external is a recording __fastcall stub bound by /alternatename. The copy handed to Create is checked to be an equal set at a different address. The runs:
- no sub-shot def;
- three OBJECT sub-shots with a failing Create in the middle (angles, target positions, Init arguments, the by-value CDefPointer reference);
- a non-OBJECT class;
- a null class-name string;
- zero sub-shots;
- the last reference released through Delete;
- five sub-shots;
- def index 0. GetDef looks up only index > 0, so the `>= 0` row reads ObjectDef through null and faults (caught with __try). This kills the `> 0` boundary mutant.

## Notes for the coordinator

- **Alternative match.** A /d2inlS30 variant with a new declaration-only `CRightHandedSet(const CRightHandedSet&)` opt-in also matched; it was dropped as an invented member (see above). It is kept unbatched at r1/tmp/imp/old_s30.cpp and r1/tmp/hdr_copyoutofline_unused/.
- **Landed local models.** The landed 0x0049CF80 row and the global_E*ORIENTATION rows (0x0121AC30 / 0x0121ACD0 / 0x0121AD20 / 0x0121AD50) declare the copy as a user constructor in local CRightHandedSet classes. Ego_d marks it compiler-generated. This is not a name fix, but it bears on how those rows model it.
