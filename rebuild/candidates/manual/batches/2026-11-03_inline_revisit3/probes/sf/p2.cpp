struct CCountBlock { long References; void (__fastcall* Del)(void*); void* D; };
void operator delete(void*);
struct CBaseClass { virtual ~CBaseClass(); CBaseClass() {} };
struct CThing : CBaseClass {
    void* Data; CCountBlock* Info;
    CThing(const CThing& o) : CBaseClass(o), Data(o.Data), Info(o.Info) { if (Info) ++Info->References; }
    virtual ~CThing() { if (Info) { if (--Info->References == 0) { Info->Del(Info->D); delete Info; } } Data = 0; Info = 0; }
};
struct CName { void* p; CName(const char*); ~CName(); };
struct CBase { virtual ~CBase(); CBase(const CName&, unsigned long); unsigned char pad[0x30]; virtual void Run() = 0; };
#if V==1
// non-template holder, ctor defined in class before the caller
struct CFunc1 : CBase {
    CFunc1(const CName& n, CThing p) : CBase(n, 0), Par1(p) { }
    virtual void Run();
    CThing Par1;
};
#elif V==2
// ctor declared in class, defined after the caller
struct CFunc1 : CBase {
    CFunc1(const CName& n, CThing p);
    virtual void Run();
    CThing Par1;
};
#elif V==3
template <class P1> struct CFunc1T : CBase {
    CFunc1T(const CName& n, P1 p) : CBase(n, 0), Par1(p) { }
    virtual void Run() { }
    P1 Par1;
};
typedef CFunc1T<CThing> CFunc1;
#elif V==4
template <class P1> struct CFunc1T : CBase {
    __forceinline CFunc1T(const CName& n, P1 p) : CBase(n, 0), Par1(p) { }
    virtual void Run() { }
    P1 Par1;
};
typedef CFunc1T<CThing> CFunc1;
#endif
void* operator new(unsigned int);
void Add(CBase*);
struct CEnt { int g; CThing Me; void Init(); };
void CEnt::Init()
{
    Add(new CFunc1(CName("x"), Me));
}
#if V==2
inline CFunc1::CFunc1(const CName& n, CThing p) : CBase(n, 0), Par1(p) { }
#endif
