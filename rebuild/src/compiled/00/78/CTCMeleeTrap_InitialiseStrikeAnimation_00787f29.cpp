// CTCMeleeTrap::InitialiseStrikeAnimation(const CAnimationEntry& entry) -- clear StrikeAnimation, collect the event names of the entry's animation and set StrikeAnimation when one starts with "STRIKE_START_" or is "COMBT_STRIKE_EVENT" / "COMBT_STRIKE_EVENT_2" (the "STRIKE_END_" / "COMBT_STOP" / "COMBT_STOP_2" test is evaluated and discarded)
// Leaf: InitialiseStrikeAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787F29, 370 bytes, `ret 4`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`leave`), as the landed trap siblings (0x00788555).
//   Ego_r.pdb: `?InitialiseStrikeAnimation@CTCMeleeTrap@@AAEXABVCAnimationEntry@@@Z`.
// Callees: the animation manager getter @ 0x006F90B0 (label "GFGetPreMainMemoryUsed" is propagated: `mov eax,[13BABB4h]`),
//   the manager's event-name query (long, vector<CCharString>&) @ 0x006F9B20 (label "CEnginePrimitiveManager2D
//   AnimatedMeshes::RenderSubPrimitive" is propagated), CTCBase::GetWorld @ 0x00686A70, the world +0x3C object's
//   (long) query @ 0x00A242C0 (result unused; label "CTCInventoryAbilities::GetAbilityLevel" is propagated), strncmp
//   @ 0x00BFEAF8, CBasicString<char>::operator== @ 0x004A93C0 (calls Compare @ 0x00411570), vector<CCharString>::~vector @ 0x00414EA0 (label
//   "CCombatWheel::ResetRings" is propagated).
// Data: CCharString globals 0x013BAC6C "STRIKE_START_", 0x013BAC70 "STRIKE_END_", 0x013BAC78 "COMBT_STRIKE_EVENT",
//   0x013BAC7C "COMBT_STOP", 0x013BAC80 "COMBT_STRIKE_EVENT_2", 0x013BAC84 "COMBT_STOP_2"; "" @ 0x0129AAF4.
// Shape: the animation manager and the world are named locals (retail calls each getter before pushing the arguments);
//   CBasicString<char>::operator== and the Compare it calls (landed 0x00411570) are defined here under
//   `#pragma auto_inline(off)` as same-TU stand-ins (as landed 0x0075D2B0), so VC7.1 knows the equality writes no memory
//   and keeps the name's string block in esi across both `==` calls. The end-marker test is evaluated and discarded,
//   which leaves the last equality stored to a dead byte (`mov [ebp+0Bh],al`).
// Layout: retail CTCMeleeTrap StrikeAnimation +0x61 (PDB +0x6D; the trap base is 12 bytes smaller in retail, as the
//   landed 0x00788555 PlayingContinuous +0x60); CAnimationEntry +0x00 the animation id.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);   // 0x00BFEAF8

#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

template<class T> class CBasicString {
public:
    T*            PData;                                    // +0x00
    unsigned long StringLength;                             // +0x04
    bool operator==(const CBasicString& other) const;       // 0x004A93C0
};
#pragma auto_inline(off)
template<> bool CBasicString<char>::operator==(const CBasicString<char>& other) const   // 0x004A93C0
{
    if (StringLength != other.StringLength)
        return false;
    return Compare_00411570(PData, other.PData) == 0;
}
#pragma auto_inline(on)
class CCharStringData : public CBasicString<char> { public: };
class CCharString { public:
    CCharStringData* PStringData;
    __forceinline const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }   // "" = 0x0129AAF4
    __forceinline unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    __forceinline bool StartsWith(const CCharString& prefix) const
    {
        return strncmp(GetPString(), prefix.GetPString(), prefix.Length()) == 0;
    }
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return *PStringData == *other.PStringData;
        return false;
    }
};
extern CCharString g_StrikeStart;                           // 0x013BAC6C
extern CCharString g_StrikeEnd;                             // 0x013BAC70
extern CCharString g_CombatStrikeEvent;                     // 0x013BAC78
extern CCharString g_CombatStop;                            // 0x013BAC7C
extern CCharString g_CombatStrikeEvent2;                    // 0x013BAC80
extern CCharString g_CombatStop2;                           // 0x013BAC84

struct CStringVector {                                      // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CStringVector();                                       // 0x00414EA0
};
struct CAnimationManager {
    void GetEventNames(long animation, CStringVector& names);   // 0x006F9B20
};
CAnimationManager* GetAnimationManager();                   // 0x006F90B0
struct CAnimationBank {
    long Query(long animation) const;                       // 0x00A242C0
};
class CWorld { public:
    unsigned char   _pad00[0x3c];
    CAnimationBank* PAnimationBank;                         // +0x3C
};
class CAnimationEntry { public:
    long Animation;                                         // +0x00
};

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCTrapBase : public CTCBase
{
public:
};

struct CTCMeleeTrap : public CTCTrapBase {
    unsigned char _pad00[0x61];
    bool          StrikeAnimation;                          // +0x61
    void InitialiseStrikeAnimation(const CAnimationEntry& entry);
};

void CTCMeleeTrap::InitialiseStrikeAnimation(const CAnimationEntry& entry)
{
    long animation = entry.Animation;
    CAnimationManager* manager = GetAnimationManager();
    CStringVector names;
    manager->GetEventNames(animation, names);
    StrikeAnimation = false;
    CWorld* world = (&GetWorld());
    world->PAnimationBank->Query(entry.Animation);
    for (CCharString* it = names._M_start; it != names._M_finish; ++it) {
        if (it->StartsWith(g_StrikeStart) || *it == g_CombatStrikeEvent || *it == g_CombatStrikeEvent2)
            StrikeAnimation = true;
        else if (it->StartsWith(g_StrikeEnd) || *it == g_CombatStop || *it == g_CombatStop2) {
        }
    }
}