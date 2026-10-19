# Parked-row retry batch 6

Both candidates passed independent canonical VC7.1/Wine parity and actual-source behavior tests under VC7.1 and g++. Final landing evidence is in landing_evidence.txt.

- 00c3e580 — CScriptDefinitionManager::Shutdown, 191 bytes. Corrected the propagated CQuadTreeElement label using the landed manager destructor caller and retail cleanup accesses. Tests cover service destruction order, buffer virtual release and direct-delete branches, the exact allocation base passed to delete, cleared/retained members, and null fields. Prefix counts use an explicit 32-bit word.
- 00850410 — CPhysicsForceModelBase constructor, 199 bytes. Recovered three force-submodel initializer calls and trailing defaults. Tests execute the exact constructor and check each submodel receiver, call order, scalar parameters, owner/state links, flags, and defaults. The test stub uses 32-bit stores within its 0x24-byte storage on both host and target.

No matching shared class headers exist for these two retail models. Sources, tests, and short parity spans are preserved in payload.json and oracle.tsv.
