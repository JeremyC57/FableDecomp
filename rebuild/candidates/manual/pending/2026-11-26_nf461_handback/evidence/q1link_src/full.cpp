// full model: every virtual slot (4)
inline void* operator new(unsigned int, void* p) { return p; }
struct Foo { virtual void a(); virtual int b(); virtual int c(); virtual int d(); Foo(int); int x; };
void Foo::a() {}
int Foo::b() { return 2; }
int Foo::c() { return 3; }
int Foo::d() { return 4; }
Foo::Foo(int v) { x = v; }
Foo* makeFull(void* mem) { return new (mem) Foo(5); }
int callD(Foo* p) { return p->d(); }
