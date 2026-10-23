struct Sub { void M_00a31790(int* pv); };
struct S { char pad[72]; void Add(int v); };
void S::Add(int v){ ((Sub*)((char*)this+72))->M_00a31790(&v); }