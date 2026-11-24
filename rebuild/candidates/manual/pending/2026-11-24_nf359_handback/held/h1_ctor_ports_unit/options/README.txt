NF359 option texts, HELD (none is a candidate; every RESULTS line is HOLD). Each file is the exact text that was compiled and measured
(work/exp/*.cpp), copied unchanged; the header comment of the A texts is 0x0045C5F6's (only the Leaf line differs), so a landing would
rewrite the comment block. Grades (exp.py = verify_and_land.parity_of on the catalog oracle, row flags): all RELOCATION_MATCH.
A_allocform/   the landed precedent form (0x004304E0 / 0x004317D0 / 0x00432C40): 0x0045C5F6's source with the Leaf moved. Each object is
               section-for-section identical to 0x0045C5F6's object (evidence/setdiff_v_0045c5f6.txt), so each adds a strong
               ?Alloc@CThingNoiseDef (LNK2005; 4 strong Allocs with 0045c5c1.cpp, the 0x00432C40-shaped Alloc row). Excluded by the brief
               ("a new row must not define Alloc a second time") unless ruled otherwise.
O_outofline/   ctor defined out of line (strong, sel 1). 00455ef0.cpp needs a header opt-in FABLE_RETAIL_CTHINGNOISEDEF_CTOR_DECL
               (declare `CThingNoiseDef();` instead of the inline `{ }`), probed through an OVERLAY only (work/exp/ov/; no header is
               handed back, so no equiv run). 0044c1bb.cpp needs no header change (CTCNoiseDef.h's default text declares the ctor) beyond
               the landed _IMPLICIT_DTOR opt-in. Keeps the LNK2005 against 0x0045C5F6's pick-any copies and contradicts the inline model.
N_nf86_one_row/ the landed nf86 / nf88 per-TU anonymous-namespace helper (GOTCHAS), measured on ONE row only (not a unit proof: the helper
               hash is per compile path, so 0x0044C1BB / 0x0045C5DA variants would need their own compiles and a whole-unit link).
               Evidence only.
