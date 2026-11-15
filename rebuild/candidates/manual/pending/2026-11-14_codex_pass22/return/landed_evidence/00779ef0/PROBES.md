#0x00779EF0 probes

| Attempt | Genuine source strategy | Grade/diagnosis |
|---|---|---|
| sol2_01_original_activation /01b | Initial real header/source assembly | Compile diagnostics only: complete CDefClassBase before defining GetSubDef, and ETCInterfaceType before the driver. Not source strategies. |
| sol2_01c_original_activation | Full original OnInitialActivate on shared types, including donor-proven rend and existing original Ceil helper after per-site ladder | DIFFER555/567; strict0.9700. Missing early threshold/lifetime loads and retained lifetime x87 value; Volume assignment order. |
| sol2_02_named_threshold_and_lifetime | Named per-entry threshold and lifetime before condition; GainFaderBits before Volume assignment | Main567/567 RELOCATION_MATCH; iterator--79/79 and orb GetSubDef182/182. Independent audit found the separately compiled sound helper182 vs native162. |
| sol2_03_original_sound_helper_flags | Carry the real sound GetSubDef specialization with its native size-mode/frame-pointer settings; generic orb helper remains speed-mode | Main567/567 plus reachable helpers79/79,182/182,162/162 all RELOCATION_MATCH. All30 relocations independently bound. |

The40 rounding diagnostics in ROUNDING_ATTEMPTS.md are compiler-route evidence, not40 full-row source strategies. No parking occurred. Standalone full-body fixture864 combinations passes VC71/Od/W3/GR- without/EHsc and g++64C++03. It retains actual GetSubDef body for both concrete template instantiations and a safely backed native tree-header model. Existing shared Ceil helper/twin dual-path check independently passed fixed boundaries and20000 random inputs. Test records: runtime/behavior_tests/SOL2_RESULTS.json and history/00779ef0/dual_rounding_test/RESULT.json. Initial test-oracle comparison exposed VC71 excess intermediate precision in a float-product/ceil expectation; replaced that redundant recomputation with the exact9/14 frame table. No production source was changed for a test.
