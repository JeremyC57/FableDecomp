// Leaf: ?TransferBinaryOut@CAnimComponentStrikeSpeedMultiplier@@QAEXPAVCDataOutputStream@@@Z
class CDataOutputStream { public: void Write_00993ee0(void* d); };
struct CAnimComponentStrikeSpeedMultiplier { char pad[8]; void* f8; void TransferBinaryOut(CDataOutputStream* s); };
void CAnimComponentStrikeSpeedMultiplier::TransferBinaryOut(CDataOutputStream* s){ s->Write_00993ee0(this->f8); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?TransferBinaryOut@CAnimComponentStrikeSpeedMultiplier@@UBEXAAVCDataOutputStream@@@Z); evidence: RTTI vtable slot + PDB name set.
