# NF102 lessons (one line each)
- Map every respelled call by address and store offset, not by the landed helper name: three landed names in 0x00C54190 were swapped against the SDK (LF/HF levels, LF/HF decay ratios), and q.py masks call targets, so only binds.py on the object shows a swap.
- An SDK caller of the same length with relocations at the same offsets is a cheap second source for every callee name at once (CSalSwReverb::Update named all 16 CHLRvbParams setters at retail 0x00C54190's call sites).
- symsec.py (section COMDAT flag per lib member) shows which SDK members are header inlines vs. .cpp bodies before writing an opt-in; in a /Gy release lib everything is a COMDAT, so read the Dbg lib for out-of-line copies instead.
- VC7.1 lays overloaded virtuals in reverse declaration order: engine/CReverbFns.h's ScaleSamples / ScaleAddSamples overloads land on exactly the landed CDspOps slots 1..7, so ring-wrapper ports need no slot shims.
- A large all-or-nothing family should start with its leaves and the advisor's risky ports in work/ (one q.py each) and record per-row state in a FAMILY.tsv, so a resumed agent continues without re-deriving the closure.
- Never name a helper script after a stdlib module (dis.py broke the capstone import); run lane helpers with `python3 -I`.
