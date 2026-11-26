// short model: the local class model a ctor row uses (one virtual slot)
inline void* operator new(unsigned int, void* p) { return p; }
struct Foo { virtual void a(); Foo(); int x; };
Foo::Foo() { x = 1; }
Foo* makeShort(void* mem) { return new (mem) Foo; }
