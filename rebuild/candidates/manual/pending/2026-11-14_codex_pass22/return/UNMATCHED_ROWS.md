# Reserved-row progress

Checked 2026-11-15T11:07:36.939498+00:00.

**87 of100 reservations match;92,329 unique complete-function bytes.** All93 valid function reservations have been worked;6 remain unmatched. Seven invalid fragments start inside already-landed functions. None of the valid functions is untouched.

| Address | Function | Compiled / native bytes | Remaining difference | Documented strategy families |
|---|---|---:|---|---:|
| 00b6fa40 | CEngineWaterRenderer::RenderSeaMidground | 2019/2019 | Three instructions: reversed TEST operands, OR destination and corresponding store. 3 unmasked bytes. | 18 |
| 00b70230 | CEngineWaterRenderer::RenderSeaBackground | 2138/2138 | Same three-instruction TEST/OR/store pattern as Midground. 3 unmasked bytes. | 23 |
| 00bf0b20 | CEnginePrimitiveRenderUtil::SetupOutlineStencilStates | 1290/1290 | One blue-channel load is scheduled in a different position. 8 unmasked bytes. | 28 |
| 00f3e120 | CTriangleStripifier2::FindBestStartVertex | 1411/1411 | Two pairs of loads reversed; three actually reached bool-container bodies also differ. 8 unmasked bytes. | 42 |
| 0064bfba | NPlayerGui::CDrawHighlightingTargetingCursor::Draw | 2217/2223 | Four disassembly regions; height/dimension call order, six bytes short. | 56 |
| 00675e20 | CCreatureDef::Transfer | 1912/1912 | 34 disassembly regions, principally scratch-register choices. New94 exactly reconstructs called bool-input helper114 but root unchanged. 184 unmasked bytes. | 21 |

## Invalid supplied starts

| Assigned start | Actual function start | Actual length | Disposition |
|---|---|---:|---|
| 005239cd | 005239c0 | 379 | Already-landed enclosing function; no new duplicate credit |
| 005eadd6 | 005eadc0 | 354 | Already-landed enclosing function; no new duplicate credit |
| 005eaf46 | 005eaf30 | 326 | Already-landed enclosing function; no new duplicate credit |
| 005eb08d | 005eb080 | 256 | Already-landed enclosing function; no new duplicate credit |
| 0074b77c | 0074b770 | 395 | Already-landed enclosing function; no new duplicate credit |
| 007c977c | 007c9770 | 300 | Already-landed enclosing function; no new duplicate credit |
| 00c0bf9b | 00c0bf90 | 78 | Already-landed enclosing function; no new duplicate credit |

Source families are conservative documented strategy groups, not compiler invocation totals. Matching functions need not exhaust ten attempts. Invalid fragments receive boundary investigation instead of fabricated function implementations.

The byte difference counts above exclude relocation operands and require equal target lengths. They do not certify callee identity or behavioral correctness by themselves. F3 still has three helper-body debts; its eight root-byte differences are not its only landing obstacle.
