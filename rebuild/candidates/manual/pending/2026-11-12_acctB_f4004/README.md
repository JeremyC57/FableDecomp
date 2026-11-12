# acctB hand-back, longest-row lane 4004 (1 row matched: 23,755 bytes; second row passed on)

Authored by a Fable 5.2 agent (the owner's "longest reserved rows" lane), re-verified by the acctB local coordinator.
Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00F2D6E0 `?Transfer@CScriptDef@@UAEXAAVCPersistContext@@@Z` | RELOCATION_MATCH 23755v23755, test PASS (g++ and VC7.1) | no flags, no pragma, no asm; 19 whole-function grades (`NOTES.md` / STATUS); see the FITTED items below before landing |
| 0x00F2A250 CScriptDef::operator= | not attempted | no source written (context); outline in `NOTES.md`. A fresh agent is on it (chunk 4005) with this hand-back's headers |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; three `WARN RELOC_NAME`, item 5). Purity and
padded-struct greps are clean.

The source is 944 plain statements over the 943 members (`context.Transfer("Name", Name);`, `TransferVector`,
`TransferVectorOfSubComponents`; TemplePrayerFactorHighest is transferred twice, as in retail). All 6,409 instructions
are equal and the agent paired 965 relocation symbols one to one with retail.

## Review items for landing -- what is PROVEN and what is FITTED

The whole residue was which copies of the inline helpers c2 expands: retail expands the two-argument `Transfer<T>` at
statements 1..451 and calls it at 452..944, and inlines `GetDefaultVal<long>` only at 0x00F30E97 and 0x00F30EF7. The row
has an `// Inline-budget:` line with the measured c2 numbers (budget rule at c2.dll 0x107284A3; the agent's `sim.py`
replays the arithmetic and reproduces the c2 trace at all 944 decisions).

1. **Proven by retail's layout:** the function is an INLINE virtual emitted through the vtable (retail's COMDAT order is
   this row, then TransferVector<CDefString> 0x00F333B0, the wrapper virtuals, then 0x00F334E0, which is the order c1
   gives an inline virtual compiled before the templates it first uses). Written non-inline it is 30,307 or 25,820 bytes.
2. **FITTED -- emission trigger.** The row file ends with `CScriptDef::CScriptDef() {}` so the vtable, and with it the
   inline virtual, is emitted. That is a stand-in: retail's real constructor is the landed 0x00F28F90 in the same TU
   (precedent for the device: landed row 0x00F29530). Pair-landing this row with the real constructor body would remove
   the stand-in; not tried.
3. **FITTED -- `template void CPersistContext::TransferVector<CCharString>(...)` explicit instantiation ahead of the
   row.** It stands for "retail's TU compiled 0x0049B8D0 before this function" (moves the cut from statement 394 to
   450); which earlier retail function did that is not known.
4. **FITTED -- `CPersistTraits<float>` as an explicit class specialisation with `const float DefaultVal`**
   (`FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS`, exclusive with `..._FLOAT_TRANSFERIN_BODY` by `#error`). It makes
   GetDefaultVal<float> c1 size 49, refused where the long one (48) is accepted, which gives the exact bytes. The class
   specialisation has PDB support (its TransferBinaryOut takes `const float&`); the `const` on the member is the only
   shape measured that gives 49 and has no second source.
5. **Name fixes** (`NAME_FIXES.tsv`, 6; the three vland warnings are among them): 0x007E37C0 is
   `??$GetDefaultVal@VCDefString@@@CPersistContext@@KI?AVCDefString@@XZ`, landed as `??0CDefString@@QAE@XZ` (same 9
   bytes; all 35 callers are this row's expanded Transfer<CDefString>; the same body at 0x0042D9D0 is bound as the
   constructor); 0x00F333B0 is `TransferVector<CDefString>`, landed as `TransferVector<long>` (which is 0x005815D6);
   0x004107E0 is `Transfer<float>(name, value)` (landed file says long); 0x007E3AA0 is `Transfer<CDefString>(name,
   value)` (landed line 1 says `long&`); 0x0040E160 / 0x0040E250 are `GetDefaultVal<long>` / `GetDefaultVal<float>`
   (catalog has descriptive names; whether other identities share those tiny bodies was not examined).
6. **Headers** (`hdr/README.md`; default text unchanged): `engine/CPersistContext.h` opt-ins
   `FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY` (shared TransferVector body + protected
   `TransferVectorSaveText` 0x009BAC80) and `FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY`; `engine/CPersistTraits.h`
   opt-in (item 4); new `engine/CPersistContext_TransferableVector.h`. The row must include the two overlay headers
   first. **Not done:** the landed TransferVector instance rows (0x00F333B0, 0x0049B8D0, 0x00569FE0, 0x005815D6) were not
   regraded against the new shared body (they do not define the macro). Header debt: the four
   `CPersistContext_TransferableVector<T>` overrides and `GFSerialiseVectorBinaryIn/Out` are declared only.

## Behaviour test

`rows/00f2d6e0.test.cpp` is generated (agent-side `tools/mktest.py`) from the retail disassembly: per statement the key
string retail pushes, the kind of the retail callee and the member the header places at the retail offset (looked up by
offset, not by key). 944 events, 943 distinct members, the duplicate at 478 / 479, every member at its type default after
clear mode, and a save / load round trip. 12 of 12 single-edit mutations fail it (agent's measurement).

## How to land

`rows/00f2d6e0.cpp` + `rows/00f2d6e0.test.cpp` with the three `hdr/engine/` files, then `payload.json` through your
usual vland -- after deciding items 2..4. `LESSONS.md`: c2's two inline rules (cost rule for callees already compiled in
the TU, budget rule for callees not yet compiled), what an inline virtual emitted through the vtable does to them, and
that a template body that is accepted but never expands still costs budget.
