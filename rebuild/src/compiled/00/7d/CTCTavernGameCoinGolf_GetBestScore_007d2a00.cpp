// CTCTavernGameCoinGolf::GetBestScore() -- the def's "best score" text followed by the hero's best coin-golf score for this table (Oakvale or Snowspire), when one is set
// Leaf: GetBestScore
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D2A00, 358 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCoinGolf vtable 0x0126F49C slot 43 (the catalog's "vfunc_43"; slot 43 is GetBestScore in every tavern game:
// ShoveHaPenny 0x007CF5B0, CardPairs 0x007D6560, SpotTheAddition 0x007D7C80, all landed).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          CTCTavernGame::GetHeroStats(CTCHeroStats**) 0x005EB4B0; CThing::GetDefName() const 0x004C7CC0;
//          CDefString::operator==(const char*) const 0x005B3440; CCharString::FormatDouble(long, double) 0x009A08B0
//          (static `__fastcall`); CCharString::ToWideString() const 0x0099E480; CWideString::operator+= 0x0099B8D0;
//          ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; CWideString(const CWideString&) 0x0099B720; def slot 1.
// Literals: "OBJECT_TAVERN_TABLE_COIN_GOLF" 0x0126F3F4, "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE" 0x0126F3CC.
// Layout (retail): ParentThing +0x04; CTavernGameDef::BestScore +0xb4; CTCHeroStats BestScoreCoinGolfOakVale +0x184,
//          BestScoreCoinGolfSnowSpire +0x188 (Ego_r.pdb - 0x18).
// Shape: as the landed CardPairs GetBestScore: text fetched first, the stats pointer zero-initialised, `best != 0.0f` is
//        `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp`; the table test and the score test share one `&&` per arm
//        so a set Oakvale name with no score falls through to the Snowspire test; the Snowspire arm reads the parent thing
//        into a named local (retail loads `mov ecx,[esi+4]` before the literal push there).
class CWideString { public:
    void* m_data;
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
class CCharString { public:
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
    CWideString ToWideString() const;           // 0x0099E480
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
class CDefString { public:                             // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_tablePos;
    CDefString();
    bool operator==(const char* text) const;    // 0x005B3440
};
class CThing { public:
    CDefString GetDefName() const;              // 0x004C7CC0
};

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
    unsigned char _pad_0x08[0xac];
    unsigned long BestScore;                    // +0xb4
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

class CTCHeroStats { public:                           // retail layout (PDB - 0x18 here)
    unsigned char _pad[0x184];
    float BestScoreCoinGolfOakVale;             // +0x184
    float BestScoreCoinGolfSnowSpire;           // +0x188
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    void GetHeroStats(CTCHeroStats** stats);    // 0x005EB4B0
    CWideString GetText(unsigned long id);      // 0x005E9E70
};

class CTCTavernGameCoinGolf : public CTCTavernGame { public:
    CWideString GetBestScore();
};

CWideString CTCTavernGameCoinGolf::GetBestScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CWideString text = GetText(def->BestScore);
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    if (ParentThing->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF" && stats->BestScoreCoinGolfOakVale != 0.0f)
        text += CCharString::FormatDouble(0, stats->BestScoreCoinGolfOakVale).ToWideString();
    else {
        CThing* parent = ParentThing;
        if (parent->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE" && stats->BestScoreCoinGolfSnowSpire != 0.0f)
            text += CCharString::FormatDouble(0, stats->BestScoreCoinGolfSnowSpire).ToWideString();
    }
    return text;
}