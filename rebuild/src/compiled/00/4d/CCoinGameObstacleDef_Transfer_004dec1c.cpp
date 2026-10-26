class CPersistContext { public: template <class T> void Transfer(T& value); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Transfer(*(float*)(reinterpret_cast<char*>(this) + 0x28)); }