// MSVC5 claim: memory locals are packed in a list sorted by size (small first) then reference count (high first);
// a local joins the newest earlier slot it does not interfere with. Check VC7.1 slot order for address-taken ints.
void take(int*); void take2(int*, int*);
// all four live for the whole function (interfere), different ref counts: a 1, b 3, c 2, d 4 extra reads
int fr1() { int a, b, c, d; take(&a); take(&b); take(&c); take(&d); int s = a + b + c + d;
  s += b + b + c + d + d + d; take2(&a, &d); return s; }
// same but declared in the other order
int fr2() { int d, c, b, a; take(&a); take(&b); take(&c); take(&d); int s = a + b + c + d;
  s += b + b + c + d + d + d; take2(&a, &d); return s; }
// disjoint lifetimes in nested blocks: do they share?
int fr3() { int s = 0; { int a; take(&a); s += a; } { int b; take(&b); s += b * 2; } { int c; take(&c); s += c * 3; } return s; }
// function-scope locals with disjoint live ranges (no blocks): share?
int fr4() { int s = 0; int a; take(&a); s += a; int b; take(&b); s += b * 2; int c; take(&c); s += c * 3; return s; }
// sizes: a char[8] (2 refs), an int (5 refs), a double (1 ref)
int fr5() { char buf[8]; int i; double d; take((int*)buf); take(&i); take((int*)&d); return buf[1] + i + i + i + i + (int)d; }
