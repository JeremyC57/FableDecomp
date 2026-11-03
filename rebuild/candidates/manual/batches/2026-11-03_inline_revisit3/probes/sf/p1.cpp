// probe: by-value class arg copied at the call site; member copy inside an inlined template ctor
struct CCountBlock { long References; };
struct CBaseClass { virtual ~CBaseClass(); CBaseClass(); };
struct CThing : CBaseClass {
    void* Data; CCountBlock* Info;
    CThing(const CThing& o);
    virtual ~CThing();
};
#ifndef CCDEF
#define CCDEF inline CThing::CThing(const CThing& o) : CBaseClass(o), Data(o.Data), Info(o.Info) { if (Info) ++Info->References; }
#endif
CCDEF
struct CName { void* p; CName(const char*); ~CName(); };
struct CBase { virtual ~CBase(); CBase(const CName&, unsigned long); unsigned char pad[0x30]; virtual void Run() = 0; };
template <class T, class P1> struct CFunc1 : CBase {
    CTORKW CFunc1(const CName& n, unsigned long id, T* pc, void (T::*f)(P1), P1 p) : CBase(n, id), Func(f), PClass(pc), Par1(p) { }
    virtual void Run() { (PClass->*Func)(Par1); }
    void (T::*Func)(P1); T* PClass; P1 Par1;
};
void* operator new(unsigned int);
struct CScript { void Watch(CThing); void Add(CBase*); };
struct CEnt { int g; CThing Me; CScript* Parent; void Init(); };
void CEnt::Init()
{
    Parent->Add(new CFunc1<CScript, CThing>(CName("x"), 0, Parent, &CScript::Watch, Me));
}
