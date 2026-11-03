struct CCountBlock { long References; void (__fastcall* Del)(void*); void* D; };
void operator delete(void*);
struct CBaseClass { virtual ~CBaseClass(); CBaseClass() {} };
struct CThing : CBaseClass {
    void* Data; CCountBlock* Info;
    CThing(const CThing& o);
    virtual ~CThing();
};
#if P==0
inline CThing::CThing(const CThing& o) : CBaseClass(o), Data(o.Data), Info(o.Info) { if (Info) ++Info->References; }
inline CThing::~CThing() { if (Info) { if (--Info->References == 0) { Info->Del(Info->D); delete Info; } } Data = 0; Info = 0; }
#endif
struct CName { void* p; CName(const char*); ~CName(); };
struct CBase { virtual ~CBase(); CBase(const CName&, unsigned long); unsigned char pad[0x30]; virtual void Run() = 0; };
template <class P1> struct CFunc1T : CBase {
    KW CFunc1T(const CName& n, P1 p) : CBase(n, 0), Par1(p) { }
    virtual void Run() { }
    P1 Par1;
};
#if P==1
inline CThing::CThing(const CThing& o) : CBaseClass(o), Data(o.Data), Info(o.Info) { if (Info) ++Info->References; }
inline CThing::~CThing() { if (Info) { if (--Info->References == 0) { Info->Del(Info->D); delete Info; } } Data = 0; Info = 0; }
#endif
void* operator new(unsigned int);
void Add(CBase*);
struct CEnt { int g; CThing Me; void Init(); };
void CEnt::Init()
{
    Add(new CFunc1T<CThing>(CName("x"), Me));
}
#if P==2
inline CThing::CThing(const CThing& o) : CBaseClass(o), Data(o.Data), Info(o.Info) { if (Info) ++Info->References; }
inline CThing::~CThing() { if (Info) { if (--Info->References == 0) { Info->Del(Info->D); delete Info; } } Data = 0; Info = 0; }
#endif
