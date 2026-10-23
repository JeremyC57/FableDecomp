struct CPersistContext { void Xfer_004798ab(void* sub); };
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_004798ab(reinterpret_cast<char*>(this) + 0x3c); }