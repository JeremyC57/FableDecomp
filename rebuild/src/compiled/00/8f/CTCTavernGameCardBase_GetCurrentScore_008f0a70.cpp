// CTCTavernGameCardBase::GetCurrentScore() -- the tavern-game def's "current score" text followed by Counter formatted to one decimal
// Leaf: ?GetCurrentScore@CTCTavernGameCardBase@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F0A70, 167 bytes (`ret 4`, CWideString by hidden pointer); CTCTavernGameCardBase vtable
//   0x01288294 slot 44 (slot 44 = GetCurrentScore, 45 = GetRequiredScore, as the landed CoinGolf / CardPairs rows).
// Identity (proven): the vtable slot; Ego_r.pdb ?GetCurrentScore@CTCTavernGameCardBase@@UAE?AVCWideString@@XZ (donor 0x00916120); the float is
//   Ego_r.pdb CTCTavernGameCardBase::Counter at retail +0x154.
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CCharString::FormatDouble(long, double) 0x009A08B0;
//   CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70; operator+(const
//   CWideString&, const CWideString&) 0x0099BE70; ~CWideString 0x0099B510 (x2); ~CCharString 0x0099EAE0; the def's slot 1.
// Shape (family K026, representative 0x007D53A0): the landed CTCTavernGameCoinGolf::GetCurrentScore 0x007D2B70 expression
//   with one decimal (`mov edx,1`): `GetText(def->CurrentScore) + CCharString::FormatDouble(1, Counter).ToWideString()`.
class CWideString { public:
    void* m_data;
    ~CWideString();                                      // 0x0099B510
};
class CCharString { public:
    void* m_data;
    ~CCharString();                                      // 0x0099EAE0
    CWideString ToWideString() const;                    // 0x0099E480
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
    virtual void Destroy();                              // slot 1
};
class CTavernGameDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad_0x08[0xb0];
    unsigned long CurrentScore;                          // +0xb8
    unsigned long RequiredScore;                         // +0xbc
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
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb): the vptr lives here, 4-aligned
public:
    virtual void v00();
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CTCTavernGame : public CTCBase { public:           // Ego_r.pdb 264 bytes; retail 0xEC of data, 0xF0 with its 8-byte alignment
    void GetTavernGameDef(CDefPointer* out);             // 0x005EA000
    CWideString GetText(unsigned long id);               // 0x005E9E70
    unsigned char _pad_0x0c[0x18 - 0x0c];
    double        LastRenderTime;                        // +0x18 (PDB +32): makes the class 8-aligned (sizeof 0xF0)
    unsigned char _pad_0x20[0x2c - 0x20];
    float         Score;                                 // +0x2c
    unsigned char _pad_0x30[0x98 - 0x30];
    float         TargetTime;                            // +0x98 (PDB +180)
    unsigned char _pad_0x9c[0xec - 0x9c];
};
class CTCTavernGameCardBase : public CTCTavernGame {     // Ego_r.pdb 824 bytes; retail 32 lower at Counter
public:
    virtual CWideString GetCurrentScore();
    unsigned char _pad_0xf0[0x154 - 0xf0];               // GamePosition .. ParticleEffects (not touched here)
    float Counter;                                       // +0x154 (PDB +376)
};

CWideString CTCTavernGameCardBase::GetCurrentScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    return GetText(def->CurrentScore) + CCharString::FormatDouble(1, Counter).ToWideString();
}