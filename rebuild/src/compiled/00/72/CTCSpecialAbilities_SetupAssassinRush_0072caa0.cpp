// CTCSpecialAbilities::SetupAssassinRush() -- start the assassin-rush cast action on the hero (not while interfaces 0xA8/0xA9 are present): aimed at a live creature target when the targeting component has one (with the player's Z-targeting flag for a player-controlled hero), otherwise untargeted; returns whether the action was set
// Leaf: SetupAssassinRush
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072CAA0, 252 bytes (plain `ret`).
// Callees: the const interface LowerBound @ 0x004365B0 (interface 8), the targeting component's slot 33 (+0x84, target thing),
//   CThing::GetPlayer() @ 0x004C7A10, CCreatureAction_CastAssassinRush(CThingCreatureBase&, CThing&, bool, long,
//   CCountedPointer<CCreatureActionBase>) @ 0x008CE040 and (CThingCreatureBase&, long, CCountedPointer<CCreatureActionBase>)
//   @ 0x008CE100, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, ~CCreatureAction_CastAssassinRush
//   @ 0x0072D990 (manifest "_Dest_val<...>" is propagated).
// Layout: CTCSpecialAbilities Creature +0x18 (retail 4-byte list); CThing::UsingComponents bitset<274> +0x20 (8 = word +0x20 bit 8,
//   0xA8/0xA9 = word +0x34 bits 8/9), Components map +0x44, ThingTypeFlags +0x6C (8 creature, 0x20 player-controlled), Dead bitfield +0x91;
//   CPlayer::ZTargeting +0x20A.
// Shape: HasTC is the real STLport bitset::test (range check + `!= 0`), which keeps the three presence tests as separate word
//   loads (a hand-folded mask form lets VC merge 0xA9/0xA8 into one `test ch,3`); both actions are TEMPORARIES bound to
//   SetCurrentAction's const reference (the ctor's eax is pushed and the two set+destroy tails cross-jump); the empty counted
//   pointer is built in its argument slot (user copy constructor).
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
class CTCBase;
enum ETCInterfaceType { TC_NONE = 0 };
class CInterfaceMap {                                   // CVectorMap<ETCInterfaceType, CTCBase*, ...>
public:
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType* type) const;   // 0x004365B0
};
class CPlayer {
public:
    unsigned char _pad00[0x20a];
    bool          ZTargeting;                           // +0x20a (Ego_r.pdb CPlayer::ZTargeting)
};
class CThing;
class CTCTargeter {                                     // interface 8 (named descriptively)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTarget();                       // slot 33 (+0x84)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> UsingComponents;                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                             // +0x91 bit 0
    CPlayer& GetPlayer();                               // 0x004C7A10
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template <class T> __forceinline T* GetTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CCreatureActionBase;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other);
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CCreatureAction_CastAssassinRush {                // vtable 0x01284254
public:
    unsigned char _body[0x138];
    CCreatureAction_CastAssassinRush(CThingCreatureBase& creature, CThing& target, bool flag, long priority,
                                     CCountedPointer<CCreatureActionBase> next);   // 0x008CE040
    CCreatureAction_CastAssassinRush(CThingCreatureBase& creature, long priority, CCountedPointer<CCreatureActionBase> next);   // 0x008CE100
    ~CCreatureAction_CastAssassinRush();                // 0x0072D990
    operator const CCreatureActionBase&() const { return *(const CCreatureActionBase*)this; }
};
class CTCSpecialAbilities {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                              // +0x0c
    bool UsingBed;                                      // +0x0d
    long CurrentAbility;                                // +0x10
    void* UndeadFollowers;                              // +0x14 list (retail 4 bytes)
    CThingCreatureBase* Creature;                       // +0x18
    bool SetupAssassinRush();
};

bool CTCSpecialAbilities::SetupAssassinRush()
{
    if (!Creature->HasTC((ETCInterfaceType)0xa9) && !Creature->HasTC((ETCInterfaceType)0xa8)) {
        if (Creature->HasTC((ETCInterfaceType)8)) {
            CThing* target = Creature->GetTC<CTCTargeter>((ETCInterfaceType)8)->GetPTarget();
            if (target && !target->Dead && (target->ThingTypeFlags & 8)) {
                bool flag = false;
                if (Creature->ThingTypeFlags & 0x20)
                    flag = Creature->GetPlayer().ZTargeting;
                return Creature->SetCurrentAction(CCreatureAction_CastAssassinRush(*Creature, *target, flag, 0, CCountedPointer<CCreatureActionBase>()));
            }
        }
        return Creature->SetCurrentAction(CCreatureAction_CastAssassinRush(*Creature, 0, CCountedPointer<CCreatureActionBase>()));
    }
    return false;
}