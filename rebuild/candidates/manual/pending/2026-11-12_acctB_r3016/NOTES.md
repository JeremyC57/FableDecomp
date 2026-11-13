# Hand chunk 3016 (NUISystem::CTable .. NUISystem::CTable): 1 of 1 byte-exact

## What matched (1)
- **0x00550DC0 NUISystem::CTable::Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*)** (1399 bytes, RELOCATION_MATCH,
  src/00550dc0.cpp + test/00550dc0.cpp; gall.sh RELOCATION_MATCH, tall.sh / tall.py PASS, read-only vland RELOCATION_MATCH PASS with no
  warnings, lv/ deleted). A table drawn as a bastard child of its parent pushes the parent's render position / zoom / colour (combined
  with its own) down to every child sprite and sets each sprite's graphic from the parent's state, then draws as a component.
  Shape lesson (the whole residue of chunks 2006 / 3002 / 3005): write the colour block like the landed sibling
  CComponent::UpdateColour 0x0052F900 -- `CRGBFloatColour parent_colour(parent->GetRenderColour()); CRGBFloatColour colour(Colour);
  CRGBFloatColour render_colour = colour * parent_colour; RenderColour = render_colour.GetRGBColour();` -- and REUSE `colour` in the
  loop (`colour = CRGBFloatColour(Children[i]->GetColour()) * render_colour;`). The table-colour value and the loop colour become one
  packer node (weight 10, first in the 16-byte list), the parent colour joins render_colour's slot ([esp+74h]), the table colour sits
  in the loop colour's slot ([esp+3Ch]), and with one node fewer the slot qsort's density tie of the alpha / blue products comes out
  as retail ([esp+38h] / [esp+34h]). Every instruction was already right; only these frame slots moved (0x00550EFE, 0x00550F53,
  0x00550F59, 0x00550F63 and the reads 0x00550F77 / 7B / 83 / 92, 0x00551080 / 84).
- Grades: g01 baseline (prior/3005 source, DIFFER(1399v1399) as recorded), g02 probe/v1.cpp (same structure, the reused variable named
  `child_colour`; RELOCATION_MATCH), g03 src/00550dc0.cpp via gall.sh (RELOCATION_MATCH). The delivered spelling (probe/c/w1.cpp, sibling
  names) was checked free against v1's own object first: code and frame identical. ~40 free packer traces on top (k.sh / run.sh).

## Parked for Fable 5.1 (0)

## Skipped
None. Extent confirmed by chunks 2006 / 3002 (`ret 14h` 0x00551334 + int3 padding); no SPLITS.tsv.

## Label corrections
None new (NAME_FIXES.tsv header only). Row name in LABELS.tsv (Ego_r 0x005CAB20 / FableWin 0x02292260 name sets).

## Names
All callee / member names unchanged from the prior source (chunk 2006, two sources each). Descriptive (made-up, no original found:
no PDB local names on this machine): the locals `parent_colour`, `colour`, `render_colour`, `child_render_colour`, `position`,
`relative_position`, `render_position`, `zoom`, `relative_zoom`, `sprite` and the parameter names. `colour` / `render_colour` follow
the landed sibling UpdateColour (0x0052F900); `parent_colour` is that row's `parent` renamed because Draw's parameter is `parent`.

## Header debt
None new: hdr/ is identical to the chunk 3005 overlay (hdr/README.md), which still has to land first (it carries the opt-ins this row
uses: FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR, FABLE_RETAIL_CTABLE_DRAW, FABLE_RETAIL_STLP_MAP, ...).

# Working notes

## Setup
- hdr/ = prior/3005/hdr (union of the 3001 / 3002 overlays). probe/base.cpp = prior/3005 differ source.
- g01 = regrade of probe/base.cpp: DIFFER(1399v1399) (as recorded).
- Tools (all free, none diff against retail): pk.sh <src> <tag> (frame-packer trace, agent3011/agent3013 pk_host copy),
  tools/fs/qs.py (agent3011 slot-sort replay; FIXED here: param homes, presort[2] = 5, are excluded from the sort, and the
  check against the real order compares member lists -> "replay == real order: True" on base),
  tools/sim2.py (NEW: full replay = weight walk -> list order -> slot joins -> slot qsort; `--ref node@k` inserts one extra
  reference before weight-walk event k (-1 = at the end); reproduces base exactly, incl. via `--rewalk`),
  tools/sweep1.py (classifies what-ifs: M16 = retail 16-byte membership, O4 = retail alpha/blue order, rest-ok = all other
  slots as base), tools/chk.py + k.sh (trace object vs OUR base object: code diff with frame masked, frame diff, the four key
  stores' offsets).

## Packer facts (base trace tmp/base.pk.json)
- Node ids: P = parent temporary (CRGBFloatColour(parent->GetRenderColour())) n11 (16 bytes, only its R field stays in it,
  [74h] in retail); T = table temporary CRGBFloatColour(Colour) n18 (R field, [3Ch] in retail); colour (product) n22 (R field);
  child_colour n36; loop product temp n35; n34 another 16-byte loop temp. 4-byte scalars: n19 = colour.A (alpha product),
  n20 = colour.B (blue product), n21 = colour.G (green product, shares [2Ch] with n14 = T.B).
- Weight walk (w events, in final scheduled order): P def @21, T def @32, 19 def @33, 20 def @35, 21 @38, T use @39, P use @40,
  colour def @41, 19/20/21 reads @42/44/46 (GetRGBColour), colour @48, loop: 34 @68, 19/20/21 @69/71/73, 34 @74, colour @75 (its
  3rd ref) ...
- 16-byte list order (base): 35, 36, 22 colour, 18 T, 11 P, 34. Joins: T -> colour's slot (latest), P conflicts with T ->
  child_colour's slot. Retail needs colour, P, T (P joins colour, T child_colour) -- or T before colour (T joins child_colour).
- Conflict bitsets in the trace are ARRAY INDICES (map with packend idx), one-sided -> symmetrise (agent3002 sim.py read them as
  node ids: that is why it did not reproduce this function).
- 4-byte tie: n19 / n20 both w3, density 750; slot creation order n19, n20 -> qsort keeps n19 first -> n19 [34h], n20 [38h]; retail
  has n20 (blue) at [34h].

## What-if table (sim2.py, one extra IL reference; FULL = retail's complete slot layout)
| extra reference(s) | result |
|---|---|
| P (n11) at walk pos 0..75 | nothing (P reaches w3 before colour and passes it) |
| P at walk pos >= 76 (after colour's 3rd ref, i.e. in the loop or later) | M16, rest ok |
| T (n18) at walk pos <= 50 (before colour reaches 3) | M16, rest ok |
| blue n20 anywhere | O4, rest ok |
| alpha / green / colour / child_colour / n34 / n35 single | no help |
| late +1 combos of {11,18,19,20,21,22,36,34} up to 4 | FULL only for {P, blue} and {P, T, blue} |
| 2-ref combos at positions {0,39,41,50,76,end} | FULL for P@76/end + blue@any, T@<=50 + blue@any |
So retail's IL has (one extra reference to colour.B) AND (one extra late reference to P, or one early extra reference to T),
with byte-identical code.

## Candidates measured (free traces: k.sh = packer trace object vs OUR base object; never against retail)
| probe | change | result |
|---|---|---|
| c1 | `const CRGBFloatColour& parent_colour = CRGBFloatColour(parent->...)` (lifetime-extended temporary) | code = base, weights unchanged |
| c2 | named `parent_colour` only | 4 code lines (operand order: the outer-name key of r3004/r3005) |
| c3 | named parent + table colour, product copy-initialised | code = base, weights unchanged |
| c4 | both bound to const references | 4 code lines |
| c5 | UpdateColour form with `render_colour` declared then assigned | 1440 bytes (product temp + copy) |
| c6 | table colour bound to a const reference first | 109 lines |
| c7 / c8 / c9 | in-place `colour *= ...` / `colour = colour * parent_colour` | 1380 / 1382 / 1440 bytes |
| d1-d5 | dead reads / self-assignments / dead copies of colour.B (diagnostics) | removed before the packer or code change |
| e1 / e2 | early-return copies of the base Draw call (cross-jump test) | not cross-jumped here (+33 / +64 bytes) |
| f1-f4 | an extra named copy of the product (forwarding test) | real copies (code change) |
| bvA / bvB / bvAB | operator* by-value parameters (overlay PROBE_ diagnostics, removed) | 1503..1655 bytes |
| oSWAP / oSWAPR / oSWAPB | operand order inside operator* (overlay diagnostics, removed) | identical bytes and slots (c2 canonicalises) |
| **v1** | `parent_colour`, `child_colour(Colour)`, `colour = child_colour * parent_colour`; loop reuses `child_colour` | code = base, the 4 key stores at retail offsets -> g02 RELOCATION_MATCH |
| **w1** | the same structure with UpdateColour's names (`colour` reused, product `render_colour`) | identical object to v1 -> src/ (g03) |

## Mechanism (for the record)
- The extra-reference what-ifs above were right about the state retail needs (P ahead of T in the 16-byte list; blue's slot ahead of
  alpha's in the sort), but retail got there differently: one node fewer. With the table colour and the loop colour in one variable,
  the merged node (w10) leads the 16-byte list, P (w2) joins render_colour's slot, and the slot array the qsort sees changes, so the
  alpha / blue density tie (both still w3, density 750) falls out in retail's order (tools/fs/qs.py on tmp/w1.pk.json reproduces it).
- Lesson for "same instructions, frame slots swapped": besides adding references (r3011), try MERGING a temporary into a named
  variable that a later statement reuses (or splitting one); the packer replay (tools/sim2.py) shows the target state, then measure
  variable-reuse spellings with free traces.
