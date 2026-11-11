// CTCHeroStats::OnInitialActivate() -- re-applies morality, age, fatness, sun tan and money, loads the drunkenness def and resets the training status
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057D1A4, 151 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), no EH frame.
// Callees (all CTCHeroStats unless noted): SetMorality(long) @ 0x0057C2E6, SetAge(float) @ 0x0057B05F,
//          SetFatness(float) @ 0x0057B169, SetSunTan(float) @ 0x0057B0F7 (catalog "SetFatness"), SetMoney(long, bool)
//          @ 0x0057A774, CTCBase::GetDefinitionManager() const @ 0x006869D0 (result unused), CCharString(const char*, long)
//          @ 0x0099EBF0, CGameDefinitionManager::Get() @ 0x0044C6B0 (__fastcall, twice), the manager's name lookup
//          (const CCharString&, long) @ 0x009AD390, ~CCharString @ 0x0099EAE0,
//          CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(long, CDefPointer<const ..>*) const @ 0x0057FF30,
//          ResetTrainingStatus() @ 0x0057ADB8.
// Shape: the float setters take the member by value (`fld ; push ecx ; fstp [esp]`); the bare GetDefinitionManager call is
// an unused `GetHeroStatsDef()` (PeekHeroStatsDef) read (the load is dropped, the call kept); the def name is a named CCharString in its own
// `{}` block (built before the pushes, destroyed right after the lookup; a temporary is built after `push 1`).
// Layout: retail Morality +0x28, Age +0x30, SunTan +0x34, Fatness +0x38, Money +0x3C, drunkenness CDefPointer PDef
// +0x1BC (PDB +0x30/+0x38/+0x3C/+0x40/+0x44/+0x1D4).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};

class CSpecialAbilitiesDrunkennessDef;

template <class T> class CDefPointer { public:       // CDefPointer<const CSpecialAbilitiesDrunkennessDef>
    T* Object;
};

class CDefinitionManager { public:
    long GetGlobalIndexFromClassIndex(CCharString const&a0, long a1) const;   // 0x009AD390 (Ego_r.pdb ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z)
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0057FF30 GetDef<CSpecialAbilitiesDrunkennessDef> (Ego_r / FableWin template, const)
};
class CHeroStatsDef;
class CGameDefinitionManager : public CDefinitionManager { public:   // RTTI CGameDefinitionManager : CDefinitionManager
    static CGameDefinitionManager& __fastcall Get(); // 0x0044C6B0
    const CHeroStatsDef& PeekHeroStatsDef() const { return *PHeroStatsDef; }   // FableWin.pdb ?PeekHeroStatsDef@CGameDefinitionManager@@QBEABVCHeroStatsDef@@XZ (inline)
    unsigned char  _pad_0x00[0xDC];
    const CHeroStatsDef* PHeroStatsDef;              // +0xDC (descriptive name, shared engine/CGameDefinitionManager.h)
};

class CTCBase { public:                              // the component base (members this row reads)
    virtual ~CTCBase();
    void*         ParentThing;                       // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};

class CTCHeroStats : public CTCBase { public:
    unsigned char _pad_0x08[0x20];
    long          Morality;                          // +0x28 (PDB +0x30)
    bool          MoralityChangingEnabled;           // +0x2C
    unsigned char _pad_0x2d[0x3];
    float         Age;                               // +0x30
    float         SunTan;                            // +0x34
    float         Fatness;                           // +0x38
    long          Money;                             // +0x3C
    unsigned char _pad_0x40[0x17C];
    CDefPointer<const CSpecialAbilitiesDrunkennessDef> PDef;   // +0x1BC (PDB +0x1D4)

    const CHeroStatsDef& GetHeroStatsDef() const { return GetDefinitionManager().PeekHeroStatsDef(); }
    void SetMorality(long morality);                 // 0x0057C2E6
    void SetAge(float age);                          // 0x0057B05F
    void SetFatness(float fatness);                  // 0x0057B169
    void SetSunTan(float tan);                       // 0x0057B0F7
    void SetMoney(long money, bool silent);          // 0x0057A774
    void ResetTrainingStatus();                      // 0x0057ADB8
    void OnInitialActivate();
};

void CTCHeroStats::OnInitialActivate()
{
    SetMorality(Morality);
    SetAge(Age);
    SetFatness(Fatness);
    SetSunTan(SunTan);
    SetMoney(Money, true);
    const CHeroStatsDef& def = GetHeroStatsDef();
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_DRUNKENNESS_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    ResetTrainingStatus();
}