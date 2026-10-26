// CTCTavernGameCardBlackjack::vfunc_42() -- CTCTavernGameCardBlackjack vtable slot 42: fetches the game's def (unused) and returns an empty wide string
// Leaf: vfunc_42
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA3A0, 92 bytes, `ret 4` (hidden CWideString result). CTCTavernGameCardBlackjack
// vtable 0x0126F8F4 slot 42. Callees: the tavern-game def getter 0x005EA000 (`mov ecx,[ecx+4] ; mov ecx,[ecx+70h] ;
// jmp GetSubDef 0x005ED4A0`: the owner thing's def's sub-def into a CDefPointer out-parameter),
// CCharString(const char*, long) 0x0099EBF0 with the empty literal 0x0122D70E, CCharString::ToWideString() const
// 0x0099E480, ~CCharString 0x0099EAE0; the def's virtual slot 1 (+4) on the last release.
// Shape: a null CDefPointer local filled by the getter and never read (a base-class text getter whose game
// has no extra text), then `return CCharString("", -1).ToWideString();` -- the hidden result is pushed
// first and the temporary string is homed in the dead hidden-result argument slot; the def pointer's
// destructor releases the long way (DecRef, then GetRefCount() == 0) with the null store elided.
class CCharString;
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    CWideString ToWideString() const;                // 0x0099E480
};

struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

struct CTCTavernGameCardBlackjack {
    bool GetGameDef(CDefPointer& out);               // 0x005EA000
    CWideString vfunc_42();
};

CWideString CTCTavernGameCardBlackjack::vfunc_42()
{
    CDefPointer def;
    GetGameDef(def);
    return CCharString("", -1).ToWideString();
}