// CTCTavernGameShoveHaPenny::GetRequiredScore() -- the "required score" text from the tavern-game def followed by the target round count as a number
// Leaf: GetRequiredScore
// Retail: Fable.exe @ 0x007CF740, 156 bytes (`ret 4`, CWideString by hidden pointer); CTCTavernGameShoveHaPenny vtable
//         0x0126F2F4 slot 45 (the catalog label CTCTavernGameCoinGolf is a byte-twin propagation from 0x007D2C20).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; GFIntToCharString(long) 0x0099F830 (__fastcall,
//          CCharString by hidden pointer in ecx); CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(
//          unsigned long) 0x005E9E70; operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall);
//          CWideString::~CWideString 0x0099B510 (x2); CCharString::~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout (retail): CTCTavernGame::TargetRound +0x9c (PDB +0xb8); CTavernGameDef::RequiredScore +0xbc (PDB offset holds).
// Shape: one expression `GetText(def->RequiredScore) + GFIntToCharString(TargetRound).ToWideString()`: the right operand
//        is evaluated first (its return slot pushed before the integer conversion); the temporaries die in reverse order;
//        the null-tested CDefPointer dtor calls the def's inline ReleaseTemporary().
class CWideString { public:
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
class CCharString { public:
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
    CWideString ToWideString() const;           // 0x0099E480
};
CCharString __fastcall GFIntToCharString(long value);                         // 0x0099F830
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
    unsigned char _pad_0x08[0xb4];
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
    unsigned char _pad_0x04[0x98];
    long          TargetRound;                  // +0x9c
};

struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    CWideString GetRequiredScore();
};

CWideString CTCTavernGameShoveHaPenny::GetRequiredScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    return GetText(def->RequiredScore) + GFIntToCharString(TargetRound).ToWideString();
}