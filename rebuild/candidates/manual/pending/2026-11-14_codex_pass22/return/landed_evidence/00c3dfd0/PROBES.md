# 0x00C3DFD0 — CCodaContext::Create

One genuine whole-body family. Selected `sol03_latest06_union`: 642/642 bytes; seven matched C++ bodies total 1027 code bytes, plus 54 compiler EH code bytes and 60 EH data bytes; all 67 relocations exact.

| Tag | Result | Source-strategy accounting |
|---|---|---|
| sol00_original_arrays | COMPILE_FAIL | Family 1: original four typed new[] arrays, real APIs and allocation operators, natural /EHsc (retail has EH prologue; all supplied factory neighbours use it). Missing calling-convention header exposed. |
| sol01_header_type_include | 642/642 match | Same source; conditional type-header inclusion repair only, excluded. |
| sol02_final_union | 642/642 match | Same body, final metadata, constructor-guard compatibility and union with prior reverb headers; regrade excluded. |
| sol03_latest06_union | 642/642 match | Same source rebased to latest main merged_headers_06; regrade excluded. |

Five RTM structural invocations = four target compiler invocations plus one 8-size/75-offset layout build. Three behavior builds (GCC fixture first failed its erroneous expected total 2400 after actually checking 1200 cases; fixed to 1200; GCC and VC then pass). Six other-row regression compilations are separate and do not inflate this row's attempts. No scheduler, register, aliasing or inline-budget variants were needed after the first genuine family matched.

Four strict-disassembly differences are grade.py's presentation of `__except_list`: it displays a fake relocation address in FS operands. Every actual compiled/native operand is zero, checked explicitly; raw masked bytes match. These are not residual instruction differences. EH map/handler and all generated vector-deleting bodies were separately audited rather than ignored.

Official SDK release Create is 501 bytes with the no-EH array-constructor helper and the later context base/layout. It is identity/type/call-graph evidence, not falsely claimed to be a 642-byte retail binary match. The reconstructed source with /EHsc emits the entire retail body and metadata exactly.

`FINAL_UNION_GRADES.json` records six prior Sol rows preserving masked bytes and all 293 emitted C++ sections/relocation names under the preceding merged04-based union plus current changes. The final latest06 rebase preserves the same active source/header tokens and all old profiles; root will run its final all-row merged sweep. Every immutable target source/header snapshot is retained under history/00c3dfd0/<tag>.
