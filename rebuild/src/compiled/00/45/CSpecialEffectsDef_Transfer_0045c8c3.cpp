class CPersistContext { public: void Xfer_0045c8d3(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_0045c8d3(reinterpret_cast<char*>(this) + 0x28); }