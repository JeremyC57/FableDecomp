class CPersistContext { public: void Xfer_004e8997(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_004e8997(reinterpret_cast<char*>(this) + 0x28); }