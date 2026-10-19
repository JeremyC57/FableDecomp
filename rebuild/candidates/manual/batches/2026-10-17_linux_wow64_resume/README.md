# WoW64 lane resumption — two byte-exact reconstructions

2026-10-17: VC7.1 13.10.4035 under repaired Wine 11 WoW64, with genuine C++ sources and behavior fixtures.

| Address | Function | Parity | Behavior |
|---|---|---|---|
| `006b74b0` | `CGameMap::GetMemoryFootprint` | MATCH | PASS |
| `006e71b0` | `CGameScriptInterface::IsActiveThreadTerminating` | RELOCATION_MATCH | PASS |

The map query uses PDB-named SizeX/SizeY and the seven-byte CGameMapCell; retail layout is confirmed by the
landed constructor at 0x006B7760. Its test covers empty, one-cell, rectangular and larger grids.
The script query uses the existing engine interface header and the runner global/stop byte independently
established by Rest at 0x006E71F0 and NewScriptFrame at 0x006E7410. The fixture changes both the stop flag and
active runner, and checks adjacent state is preserved. Tests compile the actual candidate implementation.

Validation command (source `work/codex-lane/wine/env.sh` first):
```
VC71_QFE=$FABLE_VC71 python tools/decomp_pipeline/verify_and_land.py rebuild/candidates/manual/batches/2026-10-17_linux_wow64_resume/payload.json rebuild/candidates/manual/batches/2026-10-17_linux_wow64_resume/oracle.tsv --qfe --recheck-landed
```

Wine repair and provenance: `docs/journal/2026-10/CODEX_RESUME_2026-10-17.md`.
