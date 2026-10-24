struct Sub { void M_00a316b0(int* pv); };
// Leaf: AddLearnedExpression
struct S { char pad[56]; void Add(int v); };
void S::Add(int v){ ((Sub*)((char*)this+56))->M_00a316b0(&v); }