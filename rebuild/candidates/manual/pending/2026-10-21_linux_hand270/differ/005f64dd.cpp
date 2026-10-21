// Grade: DIFFER(510v508) -- register allocation: retail keeps this=ebx, the search iterator in edi and the page-start iterator memory-resident at [ebp-4]; ours spills this to [ebp-4] and keeps first/it in edi/ebx; the search temporary's inc/dec merge into load/dec/store in ours (retail two RMW ops). Tried: && temp (560, flag-guarded dtor), nested if (510), count before original (508 size but early count), decl order of first, split Release.
// CTCInventoryAbilities::CycleAssignedAbilities(bool forward) -- clear the assignable ability slots, then page one row
// forward or back through the prioritised ability defs (wrapping, row-aligned, giving up after a full turn without a
// def), re-assign the slots from the new first index and play the menu click
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F64DD, 506 bytes, `ret 4` (frame pointer, `leave`).
// Callees: GetPrioritisedAbilityDefs 0x005F4B30, GetNumAbilitiesPerRow 0x005EDCF0, SetAssignedAbility(EHeroAbility, long,
// bool) 0x005F6423 (`ret 0Ch`), ~vector<CDefPointer<const CHeroAbilityDef> > 0x005F9F14, CSoundInit() 0x0041C930,
// CThing::GetSampleBank 0x004C7A80, CASoundBank::GetSoundIndexFromSymbol 0x00A01920, CCharString 0x0099EBF0/0x0099EAE0.
// Layout: CTCInventoryAbilities FirstAssignedAbility +0x144 (retail); CHeroAbilityDef Ability +0x3C (retail, as landed
// 0x00469AB3); CSoundInit 0x20 bytes, Flags +0x14 (retail frame).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CRefCountedBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };
struct CHeroAbilityDef : CRefCountedBase {
    unsigned char _pad08[0x3c - 0x08];
    EHeroAbility  Ability;                                  // +0x3C
};
template<class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
typedef CDefPointer<CHeroAbilityDef> CAbilityDefPointer;
struct CAbilityDefVector {                                  // vector<CDefPointer<const CHeroAbilityDef> >
    CAbilityDefPointer* _M_start;
    CAbilityDefPointer* _M_finish;
    CAbilityDefPointer* _M_end_of_storage;
    CAbilityDefVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CAbilityDefVector();                                   // 0x005F9F14
    CAbilityDefPointer* begin() { return _M_start; }
    CAbilityDefPointer* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    long size() const { return _M_finish - _M_start; }
};
struct CSoundInit {
    unsigned char _pad00[0x14];
    long          Flags;                                    // +0x14
    unsigned char _pad18[0x20 - 0x18];
    CSoundInit();                                           // 0x0041C930
};
class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920
};
class CThing {
public:
    CASoundBank& GetSampleBank() const;                     // 0x004C7A80
};
struct CSoundPlayer2D {
    virtual void p00(); virtual void p01(); virtual void p02();
    virtual void Play(CASoundBank& bank, long sound, bool flag, CSoundInit* init, long a);   // slot 3 (+0x0C)
};
struct CGameState {
    virtual void g00(); virtual void g01(); virtual void g02(); virtual void g03(); virtual void g04(); virtual void g05(); virtual void g06(); virtual void g07(); virtual void g08(); 
    virtual CSoundPlayer2D* GetSoundPlayer();               // slot 9 (+0x24)
};
extern CGameState* g_pGameState;                            // 0x013B8394

class CTCInventoryAbilities {
public:
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x144 - 0x08];
    long          FirstAssignedAbility;                     // +0x144
    void GetPrioritisedAbilityDefs(CAbilityDefVector* defs);   // 0x005F4B30
    long GetNumAbilitiesPerRow();                           // 0x005EDCF0
    void SetAssignedAbility(EHeroAbility ability, long slot, bool a);   // 0x005F6423
    void CycleAssignedAbilities(bool forward);
};

void CTCInventoryAbilities::CycleAssignedAbilities(bool forward)
{
    CAbilityDefVector defs;
    GetPrioritisedAbilityDefs(&defs);
    for (long slot = 0; slot < GetNumAbilitiesPerRow(); ++slot)
        SetAssignedAbility(ABILITY_NULL, slot, false);
    if (defs.empty())
        return;
    long original = Max(FirstAssignedAbility, -GetNumAbilitiesPerRow());
    long count = defs.size();
    CAbilityDefPointer* first;
    for (;;) {
        if (forward)
            FirstAssignedAbility += GetNumAbilitiesPerRow();
        else
            FirstAssignedAbility -= GetNumAbilitiesPerRow();
        if (FirstAssignedAbility >= count)
            FirstAssignedAbility = Min(original, 0L);
        else if (FirstAssignedAbility < 0) {
            FirstAssignedAbility = count - GetNumAbilitiesPerRow();
            if (FirstAssignedAbility < 0)
                FirstAssignedAbility = 0;
        }
        FirstAssignedAbility += FirstAssignedAbility % GetNumAbilitiesPerRow();
        first = defs.begin();
        if (FirstAssignedAbility > 0)
            first += FirstAssignedAbility;
        CAbilityDefPointer* it = first;
        for (long checked = 0; checked < GetNumAbilitiesPerRow(); ++checked, ++it)
            if (it != defs.end())
                if (CAbilityDefPointer(*it).Object)
                    goto found;
        if (FirstAssignedAbility == original)
            return;
    }
found:
    for (long slot = 0; first != defs.end(); ++first) {
        CAbilityDefPointer def = *first;
        SetAssignedAbility(def.Object ? def->Ability : ABILITY_NULL, slot, false);
        if (++slot == GetNumAbilitiesPerRow())
            break;
    }
    if (g_pGameState) {
        CSoundInit init;
        init.Flags = 0x20;
        CASoundBank& bank = ParentThing->GetSampleBank();
        long sound = bank.GetSoundIndexFromSymbol(CCharString("SND_MENU_04", -1));
        g_pGameState->GetSoundPlayer()->Play(bank, sound, true, &init, 0);
    }
}
