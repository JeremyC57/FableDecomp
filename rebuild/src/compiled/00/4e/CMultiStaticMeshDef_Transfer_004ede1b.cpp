struct CPersistContext { void Xfer_004ede2b(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_004ede2b(reinterpret_cast<char*>(this) + 0x28); }