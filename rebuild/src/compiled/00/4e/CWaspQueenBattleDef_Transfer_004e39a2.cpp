class CPersistContext { public: template <class T> void Transfer(T& value); };
struct S { char pad[0x3c]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Transfer(*(long*)(reinterpret_cast<char*>(this) + 0x3c)); }