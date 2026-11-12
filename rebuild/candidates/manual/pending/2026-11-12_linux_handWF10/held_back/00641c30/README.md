# 0x00641C30 CTCLight::OnAppearanceDraw -- held back (workflow wf10, 2026-11-12)

RELOCATION_MATCH (490 bytes) and test PASS under VC7.1, but only with a body of CTCLight::DrawLight 0x006414A0 visible earlier in
the same file (retail CTCLight.cpp defines DrawLight first; with its body visible VC7.1 knows the `const C3DVector&` argument does
not escape and keeps the fallback position in registers through the if / else). Without the body the row is DIFFER(502v490).
Two reasons it cannot land:
1. The DrawLight body here is an UNPROVEN reconstruction (1612 bytes against retail's 1596). Rule: only a callee's real,
   byte-proven body may sit in a row file.
2. DrawLight 0x006414A0 is RESERVED by the other account (pending/2026-11-11_acctB/RESERVED.tsv), so this lane may not author it.

To land: wait for 0x006414A0 to land from the other lane, then copy that landed row's real body ahead of this row and regrade
(as 0x004B4260 was landed after 0x004B3CE0, landing en). The four overlay headers in hdr/ exist for the DrawLight body and are NOT
landed. The DrawLight reconstruction in 00641c30.cpp may be useful to whoever authors 0x006414A0; it is evidence, not a row.
