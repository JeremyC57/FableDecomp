// CTCSpecialAbilities::SetupGhostSwordSpell() -- give the creature a "CTCGhostSword" component (activated, no params) unless it already uses one, then make the ghost swords appear
// Leaf: SetupGhostSwordSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072AA50, 72 bytes, called from CTCSpecialAbilities::UseSpecialAbility's switch (0x0072CF40).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 with "CTCGhostSword" @ 0x0123D1A8, CThing::AddTC(const CCharString&, bool,
//   CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0, CTCGhostSword::MakeSwordsAppear @ 0x007C5DB0.
// Layout: Ego_r.pdb CTCSpecialAbilities with the retail one-pointer list: UndeadFollowers +0x14, Creature +0x18 (PDB +0x20); CThing::UsingComponents
//   (bitset<274>) at +0x20: interface 0x9a (CTCGhostSword) = word +0x30 bit 0x4000000.
// Shape (the retry-10 "AddTC this-load before the pushes" gap): HasTC is the real STLport bitset<274>::test (range check that folds for a constant
//   position, `!= 0`); with that body the compiler loads the thing into ecx right after the CCharString ctor, before `push 0 ; push 1 ; lea eax`.
//   A hand-folded `Words[type >> 5] & mask` keeps the load after the pushes (`lea ecx ; push ecx ; mov ecx,[esi+N]`). The class name is the
//   implicit const-reference temporary of AddTC.
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC
namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.6 bitset<274>: 9 words, test() range-checked (no exceptions)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
}
class CTCParamsBase;
class CTCBase;
class CCharString {
public:
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    void* m_pStringData;
};
enum ETCInterfaceType { TC_NONE = 0 };
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> UsingComponents;                       // +0x20 (Ego_r.pdb `bitset<274> UsingComponents`)
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
};
class CThingCreatureBase : public CThing { public: };
class CTCSpecialAbilities {
public:
    enum EWEAPON_STATE { WEAPON_STATE_NONE = 0 };
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    bool          AbilityMutexLock;                         // +0x0c
    bool          UsingBed;                                 // +0x0d
    long          CurrentAbility;                           // +0x10 EHeroAbility
    void*         UndeadFollowers;                          // +0x14 retail std::list (one sentinel pointer; PDB +0x14..+0x1f)
    CThingCreatureBase* Creature;                           // +0x18 (PDB +0x20)
    bool SetupGhostSwordSpell();
};
class CTCGhostSword {
public:
    void MakeSwordsAppear();                                // 0x007C5DB0
};

bool CTCSpecialAbilities::SetupGhostSwordSpell()
{
    if (!Creature->HasTC((ETCInterfaceType)0x9a)) {
        CTCGhostSword* sword = (CTCGhostSword*)Creature->AddTC("CTCGhostSword", true, 0);
        sword->MakeSwordsAppear();
    }
    return true;
}