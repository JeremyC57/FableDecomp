# Landing alias 1 (Account A, 2026-11-23): 29 alias-class rows onto the real member of their shared-header class (route 2b, stopped)

Script: `tools/decomp_pipeline/linux_lane/lane_tools/alias_header.py`, landed with `land_scripted.py`. Route 2b of
`docs/journal/2026-11/2026-11-23_naming_finish_plan.md`.

- **Shape.** A row defines member M on an alias class `struct X_Methods : X` (X_Retail, X_Transfer ...) derived from the real class X of
  `engine/X.h`, and the retail RTTI vftable placement names `X::M` at the row's address.
- **Edit.**
  - `X_Methods::M` becomes `X::M`, with the target's constness;
  - M's declaration leaves the alias class;
  - when the header declares M only under one plain class-model `#ifdef`, that switch is defined before the row's include. No header is
    edited.
  - After the landing, 25 alias classes left empty and used nowhere else were removed, and 9 stale "stand-in" comments were reworded. The
    recheck below covers that tidy-up.
- **Proof per row.** The graded symbol is exactly the RTTI symbol, objsec is ALL_EQ under the one rename (`.drectve` included), and the
  grade is MATCH / RELOCATION_MATCH PASS.
- **Run `ah3`.** About 500 RTTI alias rows; 144 skipped as signature ports (parameter types differ from the PDB), 122 generated.
  - **29 accepted, 93 rejected:** 84 SRC_FAIL (alias bodies that use members the real class model names otherwise), 6 `.drectve`
    differences, 2 relocation differences, 1 LINK_FAIL.
  - **The route stops here.** 24% is far below the finish plan's 60% stop line. The 29 accepted rows each carry the full proof, so they
    land. The 93 rejects are queued as `alias1` header ports, and 237 alias rows whose member is absent from the header stay T3.

## Recheck
`logs/recheck.log` (landed tree after the tidy-up, 29 rows): 29 MATCH / RELOCATION_MATCH PASS, graded symbol = RTTI symbol.
