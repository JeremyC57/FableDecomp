# rasm1: retail_asm verification notes

## Method

For each parked retail_asm candidate (169 candidates, plus coordinator additions and the landed
`__asm` rows in rebuild/src/compiled):

1. Read the retail bytes for an asm signature. A signature is an instruction VC7.1 cannot emit from
   C++, or a shape it never produces.
2. Try every C/C++ route first:
   - MMX/SSE/SSE2 intrinsics;
   - `/QIfist`;
   - `#pragma intrinsic` and `/Oi`;
   - `/arch:SSE`/`SSE2`;
   - `/Op /Og /G5-/G7`.
   Use both the RTM 3077 toolset and the QFE 4035 toolset.
3. Rows where no C++ form reproduces the bytes are authored as C++ with a minimal `__asm`
   (at most 8 instructions, no call/jmp/ret/jcc, no naked or `_emit`) and a `// Retail-asm:` line.
4. Check every source with `purity.classify` and grade it with q2. Call targets are checked by hand
   with objdump -dr, because q2 ignores relocation identity.
5. Confirm each Leaf symbol is unique with objdump -t on the VC object.
6. Run each test under g++ and under VC7.1 (/Od). Both must print `<addr>_TEST PASS`.

No row used more than 3 grades. Rows whose asm block is over the cap or contains a jcc were graded
as oracles in differ/ and parked for the owner. They were not authored.

## Counts (STATUS.tsv, 178 rows)

| verdict | rows |
|---|---|
| retail_asm_confirmed | 173 |
| not_asm | 5 |
| intrinsics_cpp | 0 |
| compiler_flag_cpp | 0 |
| not_reproduced | 0 |

No row resolved to intrinsics_cpp or compiler_flag_cpp as a whole row. Inside confirmed rows, these
parts are ordinary C++:

- **0xC5D2E0:** the MXCSR save/restore is the xmmintrin.h `_MM_GET_ROUNDING_MODE` /
  `_MM_SET_ROUNDING_MODE` macros.
- **0xC6C000:** `cos`/`sqrt` come from `/Oi`.
- **0xC40F50 and 0xC42F50:** `log10` is `#pragma intrinsic(log10)`.

Breakdown of the 173 confirmed rows:

| group | rows |
|---|---|
| authored here, MATCH or RELOCATION_MATCH (src/, test/, payload.json) | 30 |
| already landed as retail_asm | 17 |
| byte-matched ceil/floor oracles from the prior pass, pending owner | 9 |
| held twins | 2 |
| prior-pass ceil/floor-type block (>8 instr or jne), test PASS, pending owner | 35 |
| prior-pass still-differ: asm helper exact, C++ residue | 42 |
| rasm1 asm oracles matched (differ/), pending owner | 19 |
| rasm1 DIFFER parked with residue notes | 15 |
| LTCG lane gap | 3 |
| needs naked (0x9D8650) | 1 |

The 5 not_asm rows:

- 0x645099 and 0x6D63F0: no asm signature.
- 0xBFEA70: CRT `ftol2.asm`, a MASM object.
- 0xC6AC90 and 0xC6AD10: LTCG register arguments, no asm.

## Families and evidence

**(a) Bare `fistp dword` FloatToLong / ROUND helpers.** 2 or 3 instructions. Landable.

- Evidence (probe/ev_fistp.txt):
  - VC7.1 RTM and QFE compile `(long)x` to a call to `__ftol2`.
  - `/QIfist` emits `fistp qword`, also with `/Op /Og /G5-/G7`.
  - `/arch:SSE2` emits `cvttss2si`.
  - No flag or intrinsic produces a bare `fistp dword` (DB /3), so the source used asm.
- Variants:
  - libpk `FloatToLongTruncate(x) = rne(x - 0.5)`, with `++` when negative. Note that odd integers
    map down: 3.0 gives 2.
  - `RoundHalfDownToLong` with a nested `FloatToLong` avoids the store-back (0xC77B30, 0xC77FC0).

**(b) Ceil/floor block.** 10 instructions: `fnstcw`/`fldcw` with a jne inside. VC7.1 builds an ebp
frame around it when the function has frame locals.

- The out-of-line oracle 0x48D940 MATCHes.
- 9 prior-pass rows are byte-matched.
- Status: PENDING OWNER, because the block is over 8 instructions and has control flow.

**(c) rdtsc.** 0F 31 has no VC7.1 intrinsic. The block is 5 instructions
(`xor eax,eax / xor edx,edx / rdtsc / mov / mov`) or 3 instructions in libpk 0xC52CE0. Landable.

**(d) pushfd/popfd EFLAGS.ID toggle (0xC52AA0) and cpuid (0xA5B850).** Neither has a VC7.1 C++
form. Both oracles match. Status: PENDING OWNER (jcc inside the block).

**(e) libpk 2^x transcendental block.** 15 x87 instructions plus 2 `wait`, working over C locals.
It appears in 0xC40EE0, 0xC77700 and 0xC53B10, and inlined in 0xC6C000.

- Evidence against C++:
  - `#pragma intrinsic(exp)` expands to fldl2e...fscale with no fwait.
  - `pow` becomes a `_CIpow` jmp.
- The catalog name of 0xC53B10, "Pow2_Float", is wrong: the function is Pow10.
- Status: PENDING OWNER (17 instructions).

**(f) Hand SIMD kernels (SSE, SSE2, MMX).** Over the cap and with loops. PENDING OWNER.

- Hallmarks:
  - MASM `ALIGN 16` padding, which the inline assembler emits as `mov edi,edi` / `lea ecx,[ecx]` /
    `lea ebx,[ebx]` without saving edi. VC's own C++ loop padding uses `lea ecx,[ecx]` / `nop`.
  - Fixed registers.
  - In-place `shufps` with memory operands.
  - `push esi/edi` saves only for registers named in the asm.
- Intrinsics probes (probe/intr/00a5c2b0.cpp, 00c5d370.cpp) give a different shape:
  - an `and esp,-16` aligned frame;
  - register copies;
  - `movaps x,y; shufps x,y`.
- So C++ intrinsics cannot reproduce retail here.
- 13 oracles match. Two of them (0xC6D370, 0xC6F0F0) need /GS.

**(g) Naked stack switch (0x9D8650).** A non-naked probe (probe/mts) comes out 42v36: VC adds its own
push ebx/esi/edi prologue. Needs naked, so it is not authorable.

**(h) DirectShow wxutil.h EnlargedUnsignedDivide (0xCB0160).** The published `_X86_` asm block
(`div`, then the remainder-pointer `jz`) matches byte for byte. That makes two sources: the retail
bytes and the public header. Status: PENDING OWNER (jz inside).

## Ready to land: src/ + test/ + payload.json + oracle.tsv (30 rows)

- 1 MATCH: 0x81E550.
- 29 RELOCATION_MATCH: 0x5BC3ED, 0x5CC530, 0x643B9F, 0x9E1C70, 0x9E24B0, 0xA13EB0, 0xA147E0,
  0xA14840, 0xA22530, 0xAF8D50, 0xB05C20, 0xB09AA0, 0xB0A420, 0xB0A4A0, 0xBA7B30, 0xBE6800,
  0xBFDFD0, 0xC3FF70, 0xC40F50, 0xC42F50, 0xC52CE0, 0xC5A500, 0xC6AEF0, 0xC6B8C0, 0xC6BAE0,
  0xC6CEF0, 0xC6DE60, 0xC6EBA0, 0xC77FC0.

All 30 classify as retail_asm, have unique leaves, and pass their tests under g++ and VC7.1.

About oracle.tsv:

- None of the 30 rows is in rebuild/oracles/auto-re-candidates.tsv.
- The bytes come from the chunk oracle TSVs the rows were graded against. 0xBE6800 and 0xBFDFD0 use
  the exe bytes over the rows.tsv length.
- All 30 byte strings were checked equal to the retail Fable.exe.

## Gotchas

- **Twins in one namespace.** 0x5BC3ED and 0x643B9F both define `GFGetGraphicFrame` in an anonymous
  namespace. Each keeps its symbol emitted with an external keep-pointer that has an address suffix
  (`g_keepGFGetGraphicFrame_005BC3ED`). This replaces the parked call wrapper.
- **q2 file names.** q2 needs the file basename to be the address, so probes go in subdirectories.
- **leafchk symbol regex.** objdump prints `(ty   20)` with a variable number of spaces, so match it
  with `\(ty\s+20\)`.
- **glibc log10 in tests.** glibc's `log10` is `noexcept`, which conflicts with a bare declaration.
  Put the declaration under `#ifdef _MSC_VER` in the tests.

## For the owner

1. **Asm blocks over 8 instructions or with jcc.** Should these be graded retail_asm? This covers:
   - the ceil/floor block: 35 rows, plus the 9 byte-matched ones and 0x48D940;
   - the 2^x block;
   - cpuid and pushfd;
   - wxutil;
   - the SIMD kernels.

   All of these have matched oracles in differ/ (or $S/w22/fixnames/asm/src).
2. **Naked for 0x9D8650.**
3. **One symbol for byte-identical twins.** 0xB05E10 and 0xB1AD90 match but share a symbol with
   landed 0xAF9000.
4. **LTCG lane gaps:** 0xC6D410, 0xC6BE60, 0xC74EE0, 0xC6AC90, 0xC6AD10.
5. **Landed naked/emit rows with retail-asm signatures,** which could be regraded:
   - 0x42DF9E: fnstcw. A 3-instruction block is ready, but the surrounding C++ is 518v478.
   - 0x899EF0.
   - 0xA5B850.
   - 0xC8C730.
   - 0x40138C, 0x4013C7, 0xBFE9F9: CRT SEH prolog/epilog.
