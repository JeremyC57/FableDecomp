struct CCountBlock { long References; void (__fastcall* Del)(void*); void* D; };
void operator delete(void*);
struct CBaseClass { virtual ~CBaseClass(); CBaseClass() {} };
struct CThing : CBaseClass {
    void* Data; CCountBlock* Info;
    CThing(const CThing& o) : CBaseClass(o), Data(o.Data), Info(o.Info) { if (Info) ++Info->References; }
    virtual ~CThing() { if (Info) { if (--Info->References == 0) { Info->Del(Info->D); delete Info; } } Data = 0; Info = 0; }
};
struct CPred { VD ~CPred() {} virtual bool T() = 0; };
struct P : CPred { CThing m; P(const CThing& t) : m(t) { } DT virtual bool T(); };
void Set(const CPred&);
struct E { int g; CThing Me; void Main(); };
void E::Main() { Set(P(Me)); }
