struct CPersistContext { void Xfer_004e6258(void* sub); };
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_004e6258(reinterpret_cast<char*>(this) + 0x3c); }