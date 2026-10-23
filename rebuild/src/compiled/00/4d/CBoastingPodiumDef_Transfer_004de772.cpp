struct CPersistContext { void Xfer_00431061(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_00431061(reinterpret_cast<char*>(this) + 0x28); }