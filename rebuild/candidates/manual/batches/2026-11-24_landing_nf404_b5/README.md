# Landing nf404 b5: the GameCODA CSalOutputKS family, 16 rows as one unit

Names come from two sources:
- the GameCODA 2.10 SDK CODAApi.lib member SalOutputKS.obj, cited by path and section index only. No SDK file or dump is in the repo.
- the table's existing `??0CSalOutputKS -> 00c5ed90` line (witness 00c3f9d0).

Member destructors and section order match retail.

| Address | Name |
|---|---|
| 0x00C5ECA0 | `??1CSalOutputKS@@UAE@XZ` (was the stand-in CWound_00c5eca0) |
| 0x00C5ED90 | `??0CSalOutputKS@@QAE@XZ` |
| 0x00C5E3D0 | `?Close@CSalFile@@QAEXXZ` (pick-any re-row) |
| 0x00C3C830 | `??1CCodaObjectBase@@UAE@XZ` (pick-any re-row; retail's destructor expands it inline) |
| 0x00C5EBD0 / 0x00C5EC00 | `??1CSalFile` / `??_GCSalFile` |
| 0x00C5EC50 / 0x00C5EC80 | `??1CSalKsPin` / `??_GCSalKsPin` |
| 0x00C5E440 / 0x00C5E4A0 | `??_GCSalEvent` / `??_GCSalThread` |
| 0x01216360..0x01216394 | the six unwind funclets (source unchanged; self lines carry the parent symbol) |

TABLE_CHANGES replaces the six stand-in `??_7*_00c5eca0` vtable lines with `??_7CSalOutputKS`, `CSalFile`, `CSalKsPin`, `CSalEvent`, `CSalThread` and `CSalMutex`. Afterwards no source or oracle names a stand-in vtable (advisor blocker 1).

Headers, all add-only and off by default:
- `FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS` (engine/CSalOutputKS.h)
- `FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR` (engine/CCodaObjectBase.h)
- the nested scaffolding switch `FABLE_RETAIL_CSALOUTPUTKS_KSPIN_DTOR_DECL_ONLY`

How the 30-byte `??_GCSalKsPin` was settled (advisor blocker 2, R1):
- HEAD emitted no `??_G` for the class, so the 53-byte form was new.
- Its only masked retail match is `??_GCSalFile`, so it was residue.
- NF404's bounded pass tried every in-file spelling, and all gave 53 bytes. A file that sees `~CSalKsPin` as declared only emits retail's 30-byte copy.
- The decl-only switch is used only by 0x00C5ED90 and 0x00C5EC80. The files that destroy a CSalKsPin keep the inline body, as retail requires.
- Accepted on the precedent of the per-file `*_DECL_ONLY` opt-ins already landed (STLP_VECTOR_BASE_DTOR, STLP_TREE_DTOR, STLP_TREE_MERASE, CCOUNTEDPOINTER_RELEASEDATA). It is marked as scaffolding in the header and in both rows.

NF404 gates:
- vland HEAD/new: 16/16 RELOCATION_MATCH PASS
- pk: 0 NEQ, 0 reach flips. With `--addrs`, every addressed copy is EQ at its home, including the 30-byte `??_GCSalKsPin` at 0x00C5EC80.
- tier binning: no residue apart from the file-local helpers
- /FORCE link with 18 neighbours: duplicates 1 -> 0
- census of both headers over all 57,158 sources and tests: 0 differ

Coordinator checks:
- The first two landing runs landed nothing. The first crashed because the funclet rows were missing; it was reverted to HEAD. The second was held by stale locks.
- The third run landed 16/16, 0 LITERAL_DIFF; the wrapper's includer recheck passed 21/21.
- 15 changed self lines were applied, with column 4 aligned (funclets keep their Unwind leaf_given).
- Baselines OK.
