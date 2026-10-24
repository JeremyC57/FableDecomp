struct Sub { void M_00a317e0(int* pv); };
// Leaf: AddLearnedExpression
struct S { char pad[72]; void Add(int v); };
void S::Add(int v){ ((Sub*)((char*)this+72))->M_00a317e0(&v); }