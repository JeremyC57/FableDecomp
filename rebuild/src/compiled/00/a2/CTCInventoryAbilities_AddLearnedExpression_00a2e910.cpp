struct Sub { void M_00a31700(int* pv); };
struct S { char pad[56]; void Add(int v); };
void S::Add(int v){ ((Sub*)((char*)this+56))->M_00a31700(&v); }