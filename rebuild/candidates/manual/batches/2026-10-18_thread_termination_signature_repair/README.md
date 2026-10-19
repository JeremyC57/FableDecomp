# Correct existing warden script termination helper

EED6B0 uses CEntityScriptBase::IsActiveThreadTerminating() const (EgorFA9E70), not a completion predicate. Native F35B30 calls virtual slot3 and tailcalls CScriptBase::IsActiveThreadTerminating CB7940; the latter checks script+2C thread pointer and termination byte+5. Root independently verified all six call relocations and the PDB/native contract. Candidate change is exactly helper rename plus const signature; production control flow remains byte-exact. Full expanded production fixtures pass host C++98/O2/no-elision and VC7.1, covering termination exits and unchanged predicate/string lifetimes. Source-grade3, prior artifacts retained.

Canonical source/test and existing PassPattern replaced without adding catalog entries. Function and byte totals unchanged at34,561/49,568=69.7244%. Dependency fixtures are unit tests, not full-game integration.
