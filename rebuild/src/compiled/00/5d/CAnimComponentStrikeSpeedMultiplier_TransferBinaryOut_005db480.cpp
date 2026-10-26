class CDataOutputStream { public: void Write_00993ee0(void* d); };
struct S { char pad[8]; void* f8; void TransferBinaryOut(CDataOutputStream* s); };
void S::TransferBinaryOut(CDataOutputStream* s){ s->Write_00993ee0(this->f8); }