struct CPersistContext { void Xfer_004e5601(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_004e5601(reinterpret_cast<char*>(this) + 0x28); }