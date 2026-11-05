// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.
// Leaf: _CKrakenTentacleDef_Copy
extern "C" void ForwardTarget_004e1494();
extern "C" void CKrakenTentacleDef_Copy() { ForwardTarget_004e1494(); }
// Stand-in (owner rule 2026-11-05): CKrakenTentacleDef_Copy for CKrakenTentacleDef::Copy; RTTI vtable slot ?Copy@CKrakenTentacleDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (egor/fablewin).
