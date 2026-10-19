# 2026-10-19 symbol conflicts: the two symbols the bindings table bound to two retail targets

Found by scanning `rebuild/oracles/reloc_bindings.tsv` (1,916 targets carry more than one symbol name, a whole-image
canonical-name sweep still to do; only 2 symbols pointed at two different targets, which breaks a link outright).

| address | symbol | wrong binding | fix |
|---|---|---|---|
| 00ac52f0 | `?WriteLong@CDataOutputStreamMethods@@QAEXJ@Z` | 0x993EA0 (that is WriteULong) | renamed WriteULong(unsigned long) 0x993EA0 / WriteLong(long) 0x993EB0 |
| 00905360 | `??0CScriptBase@@QAE@XZ` | 0x99A2D0 (CBaseClass ctor; 0xCB8110 is the real CScriptBase ctor) | CScriptThing root renamed CBaseClass; action root `CBase` 0x99A2F0 renamed CBaseClassNonCopyable (RTTI vtable 0x129A7C4) |

Both re-graded RELOCATION_MATCH + PASS through `verify_and_land.py --recheck-landed` with `reloc_identity.py` on
(verify.log). Remaining warnings are RELOC_NAME on propagated labels; corrections appended to
`ghidra_out/labels_identity_corrections_2026-10-19.tsv` (0x99A2D0/E0/F0, 0x993EA0, 0x4C7E50).

Open (whole-image): `??0CBase@@QAE@XZ` is still used by ~100 landed files for 0x99A2F0 and by others for 0x99A2D0;
the canonical names are CBaseClass (0x1231710) and CBaseClassNonCopyable (0x129A7C4). Sweep with a rename + recheck pass.
