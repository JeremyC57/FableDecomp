# NF198 lessons (one line each)
- Check the catalog before trusting a queue claim: q11973's "SetAsGameStatePersisted" was refuted by the catalog's own name, the Ego_r bitfield at the stored bit, and the callee (UpdateThingScanInclusion).
- A queue "correct_symbol" can be a guess: grep all three PDB name sets for it first (q11919's AddComponent@CComponentList is in none; the address is CDisplayManager::AddDeviceResetCallback).
- A `push 0` before a thiscall accessor can be the NEXT call's argument, not the accessor's (0x0048DA62: the slot-1 bool, modelled before as GetCharacters(0)).
- Self-signature families (pointer -> PDB reference) respell in place with identical bytes; walk the exe-scan referencers recursively (derived ctors -> their Alloc factories) so every address ends on one spelling.
- A CCountedPointer<T>(T*) self row ports in one grade by explicit instantiation of the header ctor (the 0x007A4210 recipe), once every caller already binds the template spelling.
- A `// Vendor-proof:` line names the vendor function: a propagated catalog label beside it is a cheap two-source rename (strmbase CAMThread).
- pass.py-style helpers keyed on bare lowercase addresses miss 0x-prefixed STATUS keys: normalise keys.
- Naming a helper script dis.py shadows the stdlib `dis` that capstone imports (circular import); use a distinct name.
