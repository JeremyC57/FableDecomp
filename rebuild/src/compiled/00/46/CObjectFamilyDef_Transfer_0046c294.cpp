struct CPersistContext { void Xfer_0046c2a4(void* sub); };
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_0046c2a4(reinterpret_cast<char*>(this) + 0x3c); }