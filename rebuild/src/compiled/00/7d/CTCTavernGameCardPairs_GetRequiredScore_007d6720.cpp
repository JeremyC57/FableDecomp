// CTCTavernGameCardPairs::GetRequiredScore() -- in the timed game type (1), the def's "required score" text followed by the target time to one decimal; otherwise an empty string
// Leaf: GetRequiredScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D6720, 261 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardPairs vtable 0x0126F6FC slot 45 (the GetRequiredScore slot: ShoveHaPenny 0x007CF740 and CoinGolf
// 0x007D2C20 sit in slot 45 too). The manifest row says "GetBestScore" (wrong slot; byte-identical to the
// CardSorting slot-45 body 0x007D5450).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CCharString(const char*, long) 0x0099EBF0;
//          CCharString::ToWideString() const 0x0099E480; ~CCharString 0x0099EAE0; CCharString::FormatDouble(long
//          digits, double) 0x009A08B0 (static `__fastcall`, hidden result in ecx); CTCTavernGame::GetText(unsigned
//          long) 0x005E9E70; operator+(const CWideString&, const CWideString&) 0x0099BE70 (`__fastcall`);
//          ~CWideString 0x0099B510; the def's virtual slot 1 when its count reaches zero.
// Layout (retail, below Ego_r.pdb): CTCTavernGame::TargetTime +0x98 (PDB +0xB4), CTCTavernGameCardBase::GameType +0x300
//          (PDB +0x324); CTavernGameDef::RequiredScore +0xbc.
// Shape: both returns build straight into the hidden result (no named result, so no copy-out); the CDefPointer dtor
//        releases through the def's DecRef/GetRefCount pair and clears the field (`mov edx,edi ; test edx,edx`, the
//        dead null store elided), expanded at both returns.
class CWideString { public:
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length); // 0x0099EBF0
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
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CTavernGameDef* operator->() const { return Object; }
};

class CTCTavernGame { public:
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x04[0x94];
    float         TargetTime;                   // +0x98
    unsigned char _pad_0x9c[0x264];
    long          GameType;                     // +0x300
};

struct CTCTavernGameCardPairs : CTCTavernGame {
    CWideString GetRequiredScore();
};

CWideString CTCTavernGameCardPairs::GetRequiredScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    switch (GameType) {
    case 1:
        return GetText(def->RequiredScore) + CCharString::FormatDouble(1, TargetTime).ToWideString();
    }
    return CCharString("", -1).ToWideString();
}