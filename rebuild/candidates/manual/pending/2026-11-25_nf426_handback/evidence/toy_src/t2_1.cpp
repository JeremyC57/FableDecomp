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
class Eps;
void Get(CDefPointer<const Eps>&);
class Delta;
void Get(CDefPointer<const Delta>&);
class Gamma;
void Get(CDefPointer<const Gamma>&);
class Beta;
void Get(CDefPointer<const Beta>&);
class Alpha;
void Get(CDefPointer<const Alpha>&);
int g0() { CDefPointer<const Eps> a; Get(a); return 0; }
int g1() { CDefPointer<const Delta> a; Get(a); return 1; }
int g2() { CDefPointer<const Gamma> a; Get(a); return 2; }
int g3() { CDefPointer<const Beta> a; Get(a); return 3; }
int g4() { CDefPointer<const Alpha> a; Get(a); return 4; }
