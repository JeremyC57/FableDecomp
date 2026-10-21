// CThingAICreature::AutoSetupSexuality(float maleChance) -- decide whether the creature can be courted and married: a female with interfaces 0x4F and 0xD7 can; a male can when he has a talk component voiced "AM3" and a random roll comes in under the chance; a courtable creature without a wife component gets a CTCWife
// Leaf: AutoSetupSexuality
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008330D0, length 340 bytes (`ret 4`, no frame pointer; Ego_r.pdb
//   `?AutoSetupSexuality@CThingAICreature@@QAEXM@Z`).
// Callees: CThingCreatureBase::GetSex() const 0x00661FF0 (as landed; manifest "CPixelShader::CountReferences" propagated);
//   CVectorMap LowerBound const 0x004365B0 (inlined const GetTC, interface 0xF = CTCTalk); CCharString(const CCharString&)
//   0x0099EC30 and ~CCharString 0x0099EAE0; Compare(const char*, const char*) 0x00411570 (__fastcall, the inlined CCharString
//   equality, as landed); CThing::GetRandomSeed() 0x004C7AE0 (as landed); GFFloatRandom(float, unsigned long*) 0x004978A0
//   (__fastcall); CCharString(const char*, long) 0x0099EBF0; CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
//   0x004C9D60 (as landed).
// Data: g_CCharString_013bab2c = "AM3" (0x013BAB2C, landed static initialiser); "CTCWife" 0x0123A700.
// Layout: retail CThing interface bitset +0x20 (0xF = bit 15 of +0x20, 0x4F = bit 15 of +0x28, 0xD7 = bit 23 of +0x38),
//   interface map +0x44; CThingAICreature bitfield run +0x1C8 (as the landed constructor 0x00831F80: CanBeCourted is bit 3
//   and CanBeMarried bit 4 of +0x1C9); CTCTalk DialogueVoice +0x38 (Ego_r.pdb = retail).
// Shape: GetSex() is called once per test (`== female`, else `== male`); the male verdict is one bool built from
//   `GetInterface(&talk, 0xF) && talk->GetDialogueVoice() == "AM3" && roll < chance` (the out-parameter lookup leaves the
//   component in eax; the voice is a by-value copy, a conditionally constructed temporary with its frame flag, which
//   shares its word with the lookup key thanks to a same-TU const LowerBound stand-in) and then written to both bits
//   through an if/else of constant stores (VC7.1 merges them into `or al,18h` / `and al,0E7h`; `a = b = v` inserts each
//   bit); CCharString's inline operator== is `__forceinline`; the "CTCWife" name is a block-scoped named local (built in
//   the dead argument slot before the AddTC pushes).
struct CTCBase;
struct CCharStringData {
    const char*   m_pData;                              // +0x00
    unsigned long m_length;                             // +0x04
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};
extern CCharString g_CCharString_013bab2c;              // 0x013BAB2C ("AM3")

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound const 0x004365B0 (STLport lower_bound over the 8-byte entries; the landed
// 0x00848730 idiom): VC7.1 then knows the key's address does not escape and shares its word with the temporary's flag.
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CTCTalk {                                        // interface 0xF
    unsigned char _pad00[0x38];
    CCharString   DialogueVoice;                        // +0x38
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};
struct CTCParamsBase;
enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };

class CThingAICreature {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x1c8 - 0x4c];
    bool PerformingVillageTaskFlag : 1;                 // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;
    bool EnableCreatureAutoPlacing : 1;
    bool AllowedToFollowHero : 1;                       // +0x1c9
    bool RegionFollowingOverriddenFromScript : 1;
    bool RespondingToFollowAndWait : 1;
    bool CanBeCourted : 1;                              // +0x1c9 bit 3
    bool CanBeMarried : 1;                              // +0x1c9 bit 4
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
    ESex GetSex() const;                                // 0x00661FF0 (CThingCreatureBase)
    unsigned long* GetRandomSeed();                     // 0x004C7AE0 (CThing)
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60 (CThing)
    void AutoSetupSexuality(float maleChance);
};

void CThingAICreature::AutoSetupSexuality(float maleChance)
{
    if (GetSex() == SEX_FEMALE) {
        if (HasTC(0x4f) && HasTC(0xd7)) {
            CanBeCourted = true;
            CanBeMarried = true;
        }
    } else if (GetSex() == SEX_MALE) {
        const CTCTalk* talk;
        bool marriageable = GetInterface(&talk, 0xf) && talk->GetDialogueVoice() == g_CCharString_013bab2c &&
                            GFFloatRandom(1.0f, GetRandomSeed()) < maleChance;
        if (marriageable) {
            CanBeCourted = true;
            CanBeMarried = true;
        } else {
            CanBeCourted = false;
            CanBeMarried = false;
        }
    }
    if (CanBeCourted && !HasTC(0xd7)) {
        CCharString wifeName("CTCWife");
        AddTC(wifeName, false, 0);
    }
}
