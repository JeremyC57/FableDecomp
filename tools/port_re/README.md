# `tools/port_re/` — reverse-engineering helpers for the portable port

Used to reconstruct functions for `rebuild/modern/` without VC7.1. Everything reads the user's own
`Fable.exe`; no retail bytes or decompiler output are committed.

| File | Purpose |
|---|---|
| `ApplyManifest.java` | Ghidra post-script: create every function in `rebuild/manifest/functions.tsv`, set namespace, name and calling convention |
| `ApplyTypes.java` | Ghidra script: load `ghidra_out/struct_layouts_egor.tsv` as structures and turn matching namespaces into classes so `this` is typed |
| `ExportDecomp.java` | Ghidra script: resumable bulk decompile of an address list to `<out>/<addr[0:4]>/<addr>.c` |
| `scan_functions.py` | Profile every catalogued function from an objdump listing: size, direct/indirect calls, writable-global and constant references |
| `disasm_function.py` | Intel-syntax disassembly of catalogued functions by address |
| `read_constants.py` | Read float/double/u32 constants at image addresses |

```sh
# Ghidra 11.4+ headless (the upstream DB used 12.1; either works for these scripts)
analyzeHeadless <proj> FableTLC -import Fable.exe -scriptPath tools/port_re \
  -postScript ApplyManifest.java rebuild/manifest/functions.tsv
analyzeHeadless <proj> FableTLC -process Fable.exe -noanalysis -scriptPath tools/port_re \
  -postScript ApplyTypes.java ghidra_out/struct_layouts_egor.tsv
analyzeHeadless <proj> FableTLC -process Fable.exe -noanalysis -readOnly -scriptPath tools/port_re \
  -postScript ExportDecomp.java addresses.txt work/decomp

objdump -d -M intel --no-show-raw-insn --start-address=0x401000 --stop-address=0x122d000 Fable.exe > text.asm
FABLE_TEXT_ASM=text.asm python3 tools/port_re/scan_functions.py      # writes funcs.json
FABLE_EXE=Fable.exe python3 tools/port_re/disasm_function.py 00a88b60
```

Porting rule for bit-exact results: keep x87 register values in `double`, round with
`fable::x87::store` exactly where retail does `fstp DWORD`, and keep the retail operation order;
then add an oracle test in `rebuild/modern/oracle/tests`.
