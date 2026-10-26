class CPersistContext { public: void Xfer_00433df0(void* sub); };
struct S { char pad[0x28]; void AddLearnedExpression(CPersistContext* ctx); };
void S::AddLearnedExpression(CPersistContext* ctx){ ctx->Xfer_00433df0(reinterpret_cast<char*>(this) + 0x28); }