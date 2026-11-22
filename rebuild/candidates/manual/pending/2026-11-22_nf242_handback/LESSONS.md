# NF242 lessons (one line each)
- A local-model vtable census from sources is not enough: compile the emitters and map each slot onto the retail vtable (tools/vtmap.py by RTTI name); a base model missing the intermediate class's overrides binds the base names at the derived addresses in every slot it does not override.
- When the slot bodies are identical (`xor al,al ; ret` x3), the slot order comes from overrides (a class that overrides only one query) and PDB code order, not from the bytes.
- Fixing one slot of a local-model vtable can move a name to another wrong address (slot 9 here moved IsAllowing from 0x00739EA0 to 0x0073DA00): build the name -> addresses table before and after, for every name the edit touches.
- An existing intermediate-class opt-in (CModeStandBase PASS22_VTABLE) can introduce a new split in a row whose own class overrides one of its slots: check each row's retail slot owner before defining it.
- A linked test of a vtable emitter needs a stub for every UND symbol; protected string internals can be filled through a never-constructed derived accessor class.
- Changing pure-virtual declarations in a widely included interface can perturb VC7.1 scheduling in an unrelated large row (0x0058578B: a 2-instruction swap): recheck every equiv-differing row with the compiler, and probe with the existing opt-ins at HEAD to tell "the declaration" from "the edit".
