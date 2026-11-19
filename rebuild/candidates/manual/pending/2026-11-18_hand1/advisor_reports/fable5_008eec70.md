## Row 0x008EEC70 Mass::Collide: closed in scratch, RELOCATION_MATCH

The row matches with one grouping parenthesis in `C2DVector::Dot`: `return (X*vector.X+Y*vector.Y);`. That needs a shared-header opt-in, which I could only apply in a scratch overlay, so the coordinator has to land it.

### Original shape and why
- **The cut:** the block after `if(bounce_impulse<0.0f)return false;` starts at 0x008EEEA5 and is cut at 81 tuples. Retail's first region holds `sub esp,8` (0x008EEF2B) but not `mov eax,esp` (0x008EEFA8), so it has exactly one more no-code tuple before the `sub` than the draft.
- **Where the tuple comes from:** `Dot` is expanded exactly once in that region, for the sliding numerator `Right(normal).Dot(vb-va)` at 0x008EEF8C..0x008EEF9E. `operator*`, `Right`, binary minus, `operator+` and `GetVel` each expand two or more times there, so a tuple in any of them would push the `sub` out as well.
- **Why the author's probes showed nothing:** each one parenthesised a class-typed expression, a call result or a plain load. None of those leaves a PAREN tuple (GOTCHAS line 220).
- **Why the header-level exclusion was wrong:** the earlier cuts of the function fall inside pure x87 chains, so moving them by the extra `Dot` tuples changes no bytes.
- **Precedent for the spelling:** it matches the grouped `GetSquaredMagnitude` `(X*X+Y*Y)` and the parenthesised `C3DVector::Dot`. Ego_d shows no named local in `C2DVector::Dot` (only `this` and `vector`), which rules out a named-sum form.

### Candidates (all in `/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w23/fable5/008eec70/`)
| File | Change | Grade |
|---|---|---|
| `c0.cpp` | the draft, as baseline | DIFFER(1129v1129), 2 hunks |
| `c1.cpp` | numerator in the named local, then `sliding_friction_impulse/=...` | DIFFER(1129v1129), same 2 hunks |
| `c2.cpp` | `(vb-va).Dot(Right(normal))` | DIFFER(1131v1129), 8 hunks |
| `c3_DOT_X_GROUP.cpp` | `(X*vector.X)+Y*vector.Y` | DIFFER(1133v1129), 6 hunks |
| `final_dot_group.cpp` (same as `c3_DOT_GROUP.cpp`) | draft plus `#define FABLE_RETAIL_C2DVECTOR_DOT_GROUP` | **RELOCATION_MATCH** |

Seven grades and one scheduler-trace run were used.

Same-TU check: `final_dot_group.cpp` also grades MATCH for 0x008EEA80 `Mass::ApplyImpulse` (which expands `Dot`), 0x008EEA30 `Mass::GetVel` and 0x008EEB40 `Mass::ForceToVel`.

### Best path
1. Add to `rebuild/include/engine/C2DVector.h` the opt-in `FABLE_RETAIL_C2DVECTOR_DOT_GROUP` selecting `float Dot(const C2DVector& vector) const { return (X*vector.X+Y*vector.Y); }`.
   - The patch is `C2DVector_dot_group.patch` in the folder above; the overlay copy is `hdr/engine/C2DVector.h`.
   - Only the `DOT_GROUP` arm is needed. The patch also carries `DOT_X_GROUP`, `DOT_Y_GROUP` and `DOT_NAMED` probe arms; drop them when landing.
2. Land `final_dot_group.cpp` as the row source. Its only change from the draft is the extra define.
3. To reproduce, set `FABLE_HDR_OVERLAY` to the `hdr` folder above; `g.sh <file>` in the same folder does this and prints the diff.

### What to try next
- The landed 0x008EEA80 `Mass::ApplyImpulse` source in `rebuild/src/compiled/00/8e/` does not carry the define. It is the same TU, so add it there for one header text per TU; it re-grades MATCH with it.
- Other rows of `tc_tavern_game_coin_base.obj` that expand `C2DVector::Dot` (for example `CTCTavernGameCoinBase::UpdateCoins`) should start with this define. I did not grade any of them.
- 0x008EEBE0 `Mass::BackplaneFriction` is landed without the define and I did not re-grade it with it.
- Suggested GOTCHAS line: when an 81-tuple cut is one tuple short, look for the inline helper expanded exactly once before the cut and group its float sum; shifts of earlier cuts inside pure x87 chains are byte-neutral, so a header-level tuple is not ruled out by other regions sitting at the cap.