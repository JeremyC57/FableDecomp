# Family pass 6 (agent955, 2026-10-27): 591 of 600 rows byte-exact

150 shape families of four instances from `famscan.py 1024` over the 2026-10-27 catalog (FAMILIES.tsv: L001-L150).
One Opus 5.5 agent at high effort matched each representative, generated every instance with its own identity from
its own bytes and callers, and graded every instance.

- **Landed:** 583 C++ rows (payload.json) and 8 retail_asm rows (payload_retail_asm.json: L129 x87
  MillibelsToAmplitude, L134 MMX MixSamples / ScaleAccumulate16, each with a `// Retail-asm:` evidence line), with
  `vland.py --land` after the land5 conversions. verify.log: 80 MATCH, 511 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY. Families: 147 complete, 3 with parked rows.
- **Identities:** 401 proven, 101 probable, 98 descriptive (NOTES.md lists the probable and descriptive ones); the 393
  proven identities of landed rows are in ghidra_out/labels_familypass6_2026-10-27.tsv.
- **Parked (9 rows, pending/2026-10-27_linux_hand955/differ/):** L013 (4, CreateMenuDesc family: component pointer
  loaded into eax then copied to esi, retail loads esi directly; 11 grades), L063 (4, StopSound family: retail loads the
  component into eax before the argument push), L062 0x01223CB0 (dynamic initializer storing 5.0f into .bss float
  0x013BC3AC; the global is unrecovered and VC7.1 RTM folds every constant initializer tried).

Review at landing (LESSONS.md and NOTES.md have all of them): the 9 RELOC_CONFLICT warnings in verify.log are landed
bindings this pass shows to be wrong -- GetPControlledCreature 0x00487DC0 vs PeekPControlledCreature 0x00487DD0
(landed 0x00896EE0), `??0CDefClassBase` is 0x0042D9E0 not the CParentDefClassBase ctor 0x00430370 (landed 0x004550AC),
`__uninitialized_copy<CDataBankTextEntry*>` is 0x009CA4C0 (landed 0x00A43B20); GetMainPlayer const 0x00449960 /
non-const 0x00449970; the `$E2` labels on 0x007899C0 / 0x007FFFA0 are shape-copied and wrong; protected
CThingSearchTools members 0x0049C6F0 / 0x0049C700 are bound under the CThingSearchTools_Calls stand-in; one access
spelling for STLport `_M_fill_insert` / `_M_insert_overflow` is still to settle repo-wide; 228 SHARED_HEADER_AVAILABLE
warnings (header debt).
