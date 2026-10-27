# Chunk 496 (agent496, 2026-10-27): 37 of 40 single functions byte-exact (8 retail_asm)

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00C57380..0x00C727B0: GameCODA / libpk audio,
no PDB or RTTI). One Opus 5.5 agent at high effort.

- **Landed:** 29 C++ rows (payload.json: 14 MATCH, 15 RELOCATION_MATCH) and 8 retail_asm rows (payload_retail_asm.json:
  7 MATCH, 1 RELOCATION_MATCH) with `vland.py --land`; every test PASS, no RELOC_IDENTITY. The retail_asm rows are the
  three CMixBuffer float mix kernels that inline libpk's fld/fistp FloatToLong helper (0x00C57F20, 0x00C57F60,
  0x00C57FC0) and five MMX/SSE2 buffer clears (0x00C6D560, 0x00C6F300, 0x00C700D0, 0x00C701E0, 0x00C70BC0), each with a
  `// Retail-asm:` line citing the retail addresses and instructions.
- **Parked (1, pending/2026-10-27_linux_hand496/differ/):** 0x00C5E2C0 WriteBuffer (102v102, register allocation only;
  six forms gave identical bytes).
- **Skipped (2):** 0x00C57490 and 0x00C5EE20 each hold a 5-byte `jmp [eax+14h]` thunk and a second function; catalog
  re-split needed.
- **Identities:** 7 proven (six CMixBuffer kernels the landed model names, the CTreePointer_00c555e0 ctor binding;
  ghidra_out/labels_chunk496_2026-10-27.tsv), 16 probable, 15 descriptive.

Label corrections (NOTES.md): 0x00C5CEB0 CAudioRingBuffer::Reset; 0x00C72560 CAudioContext::SourcePause; landed
0x00C5A1D0 `CTCStealth_OnCreate` is CSoftwareVoice slot 1 (Setup); five landed ctors carry a propagated
"C3DAnimationUtil" label (0x00C573E0, 0x00C56D30, 0x00C55070, 0x00C550D0, 0x00C72600). Review: CAudioSource is landed on
two vtables (0x012B8AE0, 0x012BEE40); landed CDSoundStreamBuffer rows count slots from 0x012B8D90 though the ctor stores
0x012B8D98; g_pCodaSystem has struct and class spellings; landed PassToSlave declares slot 1's first parameter `void*`
where both bodies test a byte.
