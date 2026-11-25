# NF449 hand-back (2026-11-25): T1 special-member / operator caller respells

NF449's own summary is in `AGENT_README.md`. `NOTES.md` has its method and gates, and `RESIDUE.tsv` (598 lines) what is left.

## Landed: 76 caller rows
| Batch | Rows | What changed |
|---|---:|---|
| s1 | 43 | Own bodiless model ctor goes from `T*` to the PDB `U&`. Targets: CTCBase ctor 0x00686800, 0x0069F0F0, 0x006DF730, 0x00843610. |
| s2 | 8 | Same form. Targets: NInventory::CTCInventoryBase ctor, CThingPhysical(unsigned long), CFileDataInputStream(CAFile&). |
| d1 | 14 | A destructor stand-in becomes a qualified `Q::~Q()` call on a bodiless class (r2, r3, q2). |
| r1g | 11 | Ruling r449a, granted. |

- **Cross-batch rewitness lines:** 3, applied afterwards.
- **Tracker:** 4 addresses split_fixable → agree_pdb, 12 partly closed, none worse.
- **Coordinator checks:** 0 HEAD-payload mismatches before landing; baselines unchanged.

## Rulings (coordinator, 2026-11-25)
- **r449a: granted.** A row's own global model of class X is wrapped in its PDB namespace (`namespace N { ... } using N::X;`). This is a pure spelling fix: there is no layout change, and objgate shows only the ctor spelling moves.
- **r449b: declined for now.** A caller may not bind the PDB spelling of a target that has not landed (0x0084E420 NCameraModes::CBase ctor), because the link gate cannot see that name defined. `s2_hold` (2 rows) waits for the target to land.
- **r449c: granted under the owner's best-guess rule (2026-11-25).** A destructor name found only in FableWin.pdb is a one-source name.
  - The target address is registered in `rebuild/oracles/descriptive_names.tsv` as `one-source:fablewin-pdb` and gated with descriptive_check. The tracker then counts it as best guess, not proven.
  - The same applies to NF442's b3_hold six.
- **r449d: declined.** A row that passes the strict check at HEAD must not start failing it with HEADER_POLICY because of a bodiless class whose engine header exists. Port 0x004C3360 / 0x00AF7C80 onto C3DAnimationInterpolator.h / the NParticleEngine CDecal header instead.

## Held
- `r1_hold` (4 rows): RESCOPE also renames a derived class's vftable symbol, so objgate is NOT_EQ.
- `s2_hold` (2 rows): r449b.
- `d1_hold` (2 rows): r449d.

## Resume point
1. DTORX: rows that model the destructor's own class (9 CAInputProcess, 3 CBaseClass).
2. The SIGCTOR groups "declared more than once" (10) and "no construct site" (9).
3. The r449c registrations.
