// CTCTavernGameCoinGolf::GetRequiredScore() -- the "required score" text from the tavern-game def followed by the target round count as a number
// Leaf: GetRequiredScore
// Retail: Fable.exe @ 0x007D2C20, 156 bytes (`ret 4`, CWideString by hidden pointer); CTCTavernGameCoinGolf vtable
//         0x0126F49C slot 45. Byte-identical to CTCTavernGameShoveHaPenny's slot-45 body 0x007CF740.
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; GFIntToCharString(long) 0x0099F830 (__fastcall,
//          CCharString by hidden pointer in ecx); CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(
//          unsigned long) 0x005E9E70; operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall);
//          CWideString::~CWideString 0x0099B510 (x2); CCharString::~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout (retail): CTCTavernGame::TargetRound +0x9c (PDB +0xb8); CTavernGameDef::RequiredScore +0xbc (PDB offset holds).
// Shape: one expression `GetText(def->RequiredScore) + GFIntToCharString(TargetRound).ToWideString()`: the right operand
//        is evaluated first (its return slot pushed before the integer conversion); the temporaries die in reverse order;
//        the null-tested CDefPointer dtor calls the def's inline ReleaseTemporary().
struct CWideString {
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
struct CCharString {
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
struct CTavernGameDef : CDefVirtual, CRefCounted {
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

struct CTCTavernGame {
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x04[0x98];
    long          TargetRound;                  // +0x9c
};

struct CTCTavernGameCoinGolf : CTCTavernGame {
    CWideString GetRequiredScore();
};

CWideString CTCTavernGameCoinGolf::GetRequiredScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    return GetText(def->RequiredScore) + GFIntToCharString(TargetRound).ToWideString();
}