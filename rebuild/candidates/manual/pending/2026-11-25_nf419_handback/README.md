# NF419 hand-back (2026-11-25): the `optimize("y", off)` audit of 00/5b-00/5c and the C3D ??_G port

222 audited rows, all class (a): every leaf is byte- and relocation-identical without the pragma (size mode keeps retail's frame);
dropping it only un-frames end-of-TU template / compiler-generated copies (136 copy verdicts NEQ/NOADDR -> EQ, 528 copies gain a
retail home, 0 regressions). Landed as `2026-11-24_landing_nf419_b1..b6` plus `_r` (5 rows relanded, below): 218 rows.
b1 also ships an add-only `engine/CAnimInit.h` change (includer recheck: 23 includers PASS).

b7: C3DAnimFile / C3DMeshFile as two Option S units owned by their private root-chunk constructors (0x00A98F40 owns ??_GC3DAnimFile
0x00A9A730; 0x00A9AE60 owns ??_GC3DMeshFile 0x00A9D2A0), with two add-only opt-ins in engine/C3DAnimFile.h / C3DMeshFile.h
(includer recheck: 7 PASS); the ??_G copies call ??1CChunkedFile@@UAE@XZ and are EQ. 4 self lines applied.
Not yet effective in an address-ordered link: the default constructors 0x00A98E60 / 0x00A9AD80 sort first and still emit the
thunk-calling ??_G copy (T3 `nf419 0x00A98E60, 0x00A9AD80 port`).

Landing notes:
- 2 rows (005bc3ed, 005c36c2) were first held because their RESULTS gave the full anonymous-namespace symbol; the lander accepts the
  `...?A0x` prefix (the hash depends on the build path). Relanded in `_r` with the prefix.
- 3 rows (005bde50, 005bf414, 005bf4ed) were held by Account A's own round-9 claim (NF100), never closed: closed and relanded in `_r`.
- 4 rows stay held by Account B's open 2026-11-20 claims (T3 `nf419 ... held`).
- reserved_check reported 005bfd5e (held) and 005cda50 (landed) as reserved by B's Codex passes 27 / 26; both passes are landed
  and merged (B stopped), so the reservation is stale.

Rulings (advisor decisions, GOTCHAS): r1 accepted (leaf identical: no mutation control possible; 217 of 222 HEAD tests are
self-contained models, booked as debt); r2 accepted (binding the ??_G names exposes copies six landed rows already emit: debt).
