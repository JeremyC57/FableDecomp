# NF244 lessons (one line each)
- A scheduling flip caused by header declarations can be non-additive: bisect every subset (2^n compiles are cheap for n = 4), not one flag at a time, and take the largest matching set.
- Neutral controls (a forward declaration, an unrelated abstract struct, a #define prepended) separate "this declaration" from "any perturbation"; here they all matched, so the flip is specific yet non-additive.
- A derived-class probe TU (struct P : C { P(); }) makes every override of C show up as an UND vtable symbol: a cheap, compiler-made spelling list to check against the PDB dumps.
- The ghidra_out/*_pdb_names.tsv dumps miss folded bodies (`mov al,1 ; ret` overrides, GetStaticMapUpdatePassCount): grep -ca the raw PDBs before calling a name one-source.
- A row whose catalog comment describes a constant ("AI creature") can be wrong about its meaning: read the enum the shared header gives.
