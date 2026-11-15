# Main verification: accepted locally

Main independently rebuilt main_review_01 with RTM13.10.3077:2446/2446 exact masked bytes,805instructions. All35 reachable helpers2363bytes and287 relocation operands independently checked;43 original engine names and4 module-local static names have both original PDB records. Every named emitted code section and relocation inventory agrees with the worker object. Existing compiler pragmas are now guarded with _MSC_VER; this hygiene change did not change any code.

Main reran4096 complete target cases under VC71 /Od /W3 /GR- without EH and GCC64C++03. Fixture target text is exact, with controlled external dependencies; it does not simulate the full game. Main reran18 ABI assertions and header token comparison: 131 successful profiles; 5 prerequisite failures on both trees disclosed: current:00675e20, current:009a3c20, current:00beae50, current:01214fe0, current:01215000. These are not counted as successful compilation checks.

All67 accepted targets plus every named emitted C++ body/relocation inventory pass on merged_headers_28. The three custom EH graphs are additionally rechecked, including the vector parent33sections95operands. Merge only four owned header old/new pairs. No Git commit or remote write.

Attempt accounting:12 genuine source families, diagnostic variants and review rebuilds logged separately in PROBES.md/history; no minimum-attempt assertion is needed for a matched row.
