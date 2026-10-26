// CTCTavernGameShoveHaPenny::GetCurrentScore() -- the "current score" text from the tavern-game def followed by the score formatted with no decimals
// Leaf: GetCurrentScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CF690, 159 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameShoveHaPenny vtable 0x0126F2F4 slot 44. The catalog's "CTCTavernGameCardBase::GetCurrentScore" is a
// propagated label: the body is byte-identical to CTCTavernGameCoinGolf's slot-44 body 0x007D2B70.
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CCharString::FormatDouble(long digits, double)
//          0x009A08B0 (static `__fastcall`, hidden result in ecx, digits in edx; Ego_r.pdb names this body
//          `GFFloatToCharString(double, long)`, the landed callers' spelling is kept); CCharString::ToWideString() const
//          0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70; operator+(const CWideString&, const CWideString&)
//          0x0099BE70 (__fastcall); CWideString::~CWideString 0x0099B510 (x2); CCharString::~CCharString 0x0099EAE0;
//          the def's slot 1 on release.
// Layout (retail): CTCTavernGame::Score +0x2c (PDB +0x34); CTavernGameDef::CurrentScore +0xb8 (PDB offset holds).
// Shape: one expression `GetText(def->CurrentScore) + FormatDouble(0, Score).ToWideString()` as the landed
//        GetRequiredScore twins (0x007D2C20 / 0x007CF740): the right operand is evaluated first (the float widened to
//        a double on the stack), the temporaries die in reverse order, the null-tested CDefPointer dtor calls the def's
//        inline ReleaseTemporary().
class CWideString { public:
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
class CCharString { public:
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
    CWideString ToWideString() const;           // 0x0099E480
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CTavernGameDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad_0x08[0xb0];
    unsigned long CurrentScore;                 // +0xb8
    unsigned long RequiredScore;                // +0xbc
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

class CTCTavernGame { public:
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x04[0x28];
    float         Score;                        // +0x2c
};

struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    CWideString GetCurrentScore();
};

CWideString CTCTavernGameShoveHaPenny::GetCurrentScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    return GetText(def->CurrentScore) + CCharString::FormatDouble(0, Score).ToWideString();
}