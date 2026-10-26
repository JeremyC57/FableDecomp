class CPersistContext { public: void Xfer_00469c3e(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_00469c3e(reinterpret_cast<char*>(this) + 0x28); }