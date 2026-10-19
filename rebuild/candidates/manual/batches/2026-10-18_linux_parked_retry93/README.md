# Recheck dialogue camera positions after movement

00713010 (302 bytes): actual NTCScriptedControl::CActionTalkBase owner, const camera-tag/helper signature and output argument order recovered from PDB and native code. No generated owner/tag header exists. Native looking-from/at fields at74/80 and flags8c/8d/8e reflect the four-byte smaller message array. Literal thresholds at12654e8/ec audited as0.25/0.8; ordered strict comparisons preserve equality and NaN behavior. Grouping vertical checks and keeping the target-horizontal check separate recovers the native branch targets.

Main independently passed optimized host and VC7.1 byte/behavior checks after expanding tests for both positions, all four movement conditions, negative/equal vertical thresholds and unordered values. Fixture also verifies native offsets, all three initial gates, initialized outputs and argument order. Ten HIGH grades plus three XHIGH grades; root verification changes only test coverage. All callee and pool relocations audited.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
