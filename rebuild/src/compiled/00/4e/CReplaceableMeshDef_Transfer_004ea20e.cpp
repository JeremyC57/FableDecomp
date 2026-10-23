struct CPersistContext { void Xfer_004ea21e(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_004ea21e(reinterpret_cast<char*>(this) + 0x28); }