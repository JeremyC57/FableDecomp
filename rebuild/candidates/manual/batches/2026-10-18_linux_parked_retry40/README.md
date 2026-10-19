# Spotlight constants and light-global buffer updates

009899b0 (164): actual vertex-shader layout header and COM stdcall slot94. Matrix upload precedes attenuation and colour; fixture mutates layout, device and input colour in callbacks to verify fresh reads and order. One grade.

0098b2c0 (201): native light-global array and dirty-flag offsets, resize default(0,0,0,1), reloaded array base and flag0x400. Exact-source fixtures exercise empty/growth/in-place writes, replaced storage, untouched entries/default gaps and existing flags. One grade.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
