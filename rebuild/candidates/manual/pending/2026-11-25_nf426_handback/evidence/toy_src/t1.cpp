struct Base { virtual ~Base(){} virtual void Delete() const; mutable unsigned long rc; long Release() const { return --rc; } void AddRef() const { ++rc; } };
#ifdef FI
#define INL __forceinline
#else
#define INL inline
#endif
template<class T> class CDefPointer {
public:
    CDefPointer() : Object(0) {}
    INL ~CDefPointer() { Release(); }
    INL void Release() { const Base* o = Object; if (o) { if (o->Release() == 0) o->Delete(); Object = 0; } }
    const Base* Object;
};
class A1; class B1; class C1;
void Get(CDefPointer<const A1>&);
void Get(CDefPointer<const B1>&);
void Get(CDefPointer<const C1>&);
int f1() { CDefPointer<const A1> a; Get(a); return 1; }
int f2() { CDefPointer<const B1> b; Get(b); return 2; }
int f3() { CDefPointer<const C1> c; Get(c); c.Release(); return 3; }
