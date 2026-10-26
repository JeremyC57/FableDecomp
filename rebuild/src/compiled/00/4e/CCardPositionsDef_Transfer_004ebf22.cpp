class CPersistContext { public: void Xfer_00475d99(void* sub); };
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer_00475d99(reinterpret_cast<char*>(this) + 0x3c); }