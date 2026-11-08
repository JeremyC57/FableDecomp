// CCombatAction_GenericResponseToDecapitation::Initialise() -- decapitation reaction: set the combat animation, decapitate the creature, take its full MaxHealth away, base Initialise, then add a knocked-down mode filtered on the anim's NextFilter group
// Leaf: ?Initialise@CCombatAction_GenericResponseToDecapitation@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00867010, 481 bytes, plain `ret` (virtual; Ego_r.pdb UAEXXZ).
// Callees (Ego_r / FableWin spellings): CCharString(const char*, long) 0x0099EBF0 / CCharString() 0x0099E4B0 /
//   CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0 / operator= 0x0099EFB0,
//   CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0, CBaseIntelligentPointer::GetPItem() const 0x00A01B50
//   (returns CBaseObject*), CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound 0x0040F020 (inside
//   CThing::GetTC<CTCDecapitation> 0xD1 / GetTC<CTCCreatureModeManager> 0x31), CTCDecapitation::Decapitate 0x007EF710,
//   CThingPhysical::ModifyHealth(float, bool) virtual slot 59 (+0xEC), CCombatActionBase::Initialise 0x00857520,
//   CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnimFilter> 0x00867F60 (landed), CDefStringTable::GetString(long)
//   const 0x009D49B0 on the global table 0x013CA828, operator new 0x00BFEA1A, CTCCreatureModeManager::CModeBase(CThing&)
//   0x0073EAC0, CCRC::Calc 0x004014A0, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0,
//   CCountedPointer<CModeBase>::DeleteData 0x007180B0.
// Data: "RESPONSE_GENERIC_REACT_TO_DECAPITATION" 0x012771A8; NCreatureMode::CModeKnockedDown vtable 0x01267E78, CAnimFilterOfGroup vtable 0x01267568.
// Layout: engine/CThingPhysical.h and engine/CCreatureActionBase.h are generated with the vptr as a data member and padding
//   only (no engine/CCombatActionBase.h, CThing.h, CTCCreatureModeManager.h), so the virtual chain (CThing slots 0..58 as filler, CThingPhysical::ModifyHealth at 59)
//   and the members used are modelled locally: CThing UsingComponents +0x20, Components +0x44; CThingPhysical MaxHealth
//   +0xb0 (engine/CThingPhysical.h retail offset); CCreatureActionBase PCreature +0x08, PAnimEntry +0x74; CModeKnockedDown
//   0x28 bytes = CModeBase 0x1c + CAnimFilterOfGroup (Group, GroupCRC) at +0x1c (landed model 0x00740820).
// Shape: with the PDB names (GetTC<T>, GetMaxHealth, CDefString::GetString, the CTCCreatureModeManager-nested CModeBase
//   taking CThing& as the base of NCreatureMode::CModeKnockedDown(CThing&, const CCharString&) (RTTI + Ego_r ctor), the CAnimFilterOfGroup member) VC7.1 keeps `this` in ebx and the lookups in esi as retail (the old
//   stand-in model swapped them); the anim name is a named local in its own block, the NextFilter CDefString is copied out
//   of a block holding the component pointer (frame 8: the pointer shares the slot of the decap pointer and the string
//   temporary), CDefString::GetString returns a named local (build, copy, destroy: no NRVO).
#include "fable_compiler.h"
#define UNPROVEN_SLOT(n) virtual void UnprovenSlot##n();
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
class CCharStringData { public:
    const char*   m_pData;
    unsigned long m_length;
};
class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CDefStringTable { public:
    CCharString GetString(long id) const;           // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)            // 0x013CA828
class CDefString { public:
    long TablePos;
    CCharString GetString() const
    {
        CCharString name = GDefStringTable.GetString(TablePos);
        return name;
    }
};
class CTCBase;
enum ETCInterfaceType { TCI_CREATURE_MODE_MANAGER = 0x31, TCI_DECAPITATION = 0xD1 };   // descriptive enumerator names
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > {
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    UNPROVEN_SLOT(0) UNPROVEN_SLOT(1) UNPROVEN_SLOT(2) UNPROVEN_SLOT(3) UNPROVEN_SLOT(4) UNPROVEN_SLOT(5) UNPROVEN_SLOT(6)
    UNPROVEN_SLOT(7) UNPROVEN_SLOT(8) UNPROVEN_SLOT(9) UNPROVEN_SLOT(10) UNPROVEN_SLOT(11) UNPROVEN_SLOT(12) UNPROVEN_SLOT(13)
    UNPROVEN_SLOT(14) UNPROVEN_SLOT(15) UNPROVEN_SLOT(16) UNPROVEN_SLOT(17) UNPROVEN_SLOT(18) UNPROVEN_SLOT(19) UNPROVEN_SLOT(20)
    UNPROVEN_SLOT(21) UNPROVEN_SLOT(22) UNPROVEN_SLOT(23) UNPROVEN_SLOT(24) UNPROVEN_SLOT(25) UNPROVEN_SLOT(26) UNPROVEN_SLOT(27)
    UNPROVEN_SLOT(28) UNPROVEN_SLOT(29) UNPROVEN_SLOT(30) UNPROVEN_SLOT(31) UNPROVEN_SLOT(32) UNPROVEN_SLOT(33) UNPROVEN_SLOT(34)
    UNPROVEN_SLOT(35) UNPROVEN_SLOT(36) UNPROVEN_SLOT(37) UNPROVEN_SLOT(38) UNPROVEN_SLOT(39) UNPROVEN_SLOT(40) UNPROVEN_SLOT(41)
    UNPROVEN_SLOT(42) UNPROVEN_SLOT(43) UNPROVEN_SLOT(44) UNPROVEN_SLOT(45) UNPROVEN_SLOT(46) UNPROVEN_SLOT(47) UNPROVEN_SLOT(48)
    UNPROVEN_SLOT(49) UNPROVEN_SLOT(50) UNPROVEN_SLOT(51) UNPROVEN_SLOT(52) UNPROVEN_SLOT(53) UNPROVEN_SLOT(54) UNPROVEN_SLOT(55)
    UNPROVEN_SLOT(56) UNPROVEN_SLOT(57) UNPROVEN_SLOT(58)
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];                 // +0x20
    CInterfaceMap Components;                         // +0x44
    template <class T> FABLE_FORCEINLINE bool GetTC(T** out)
    {
        long type = T::StatGetInterfaceType();
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing {
public:
    virtual void ModifyHealth(float amount, bool flag);   // slot 59 (+0xEC)
    unsigned char _pad4c[0xb0 - 0x4c];
    float MaxHealth;                                  // +0xb0
    float Health;                                     // +0xb4
    float GetMaxHealth() const { return MaxHealth; }
};
class CThingCreatureBase : public CThingPhysical { };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* GetPItem() const;                    // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CTCDecapitation {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_DECAPITATION; }
    void Decapitate();                                // 0x007EF710
};
#include "engine/CCountedPointer.h"
class CAnimFilterOfGroup { public:
    virtual void v00();
    CCharString Group;                                // +4
    unsigned long GroupCRC;                           // +8
    FABLE_FORCEINLINE CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        const char* data;
        unsigned long length;
        if (group.m_pData == 0) {
            length = 0;
            data = "";
        }
        else {
            length = group.m_pData->m_length;
            data = group.m_pData->m_pData;
        }
        GroupCRC = CCRC::Calc(0, data, length);
    }
};
class CTCCreatureModeManager {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATURE_MODE_MANAGER; }
    class CModeBase {
    public:
        virtual void v00();
        unsigned char _body[0x18];
        CModeBase(CThing& thing);                     // 0x0073EAC0
    };
    bool AddMode(CCountedPointer<CModeBase> mode);    // 0x0073ACC0
};
namespace NCreatureMode {
class CModeKnockedDown : public CTCCreatureModeManager::CModeBase {
public:
    CAnimFilterOfGroup Filter;                        // +0x1c
    virtual void v00();
    FABLE_FORCEINLINE CModeKnockedDown(CThing& thing, const CCharString& group) : CTCCreatureModeManager::CModeBase(thing), Filter(group) {}
};
}
class CAnimComponentFollowingOnAnimFilter { public:
    void* __vftable;
    long IVRefCount;
    CDefString NextFilter;                            // +8
};
class CAnimationEntry { public:
    template <class T> void GetComponent(T** out);   // <CAnimComponentFollowingOnAnimFilter> 0x00867F60
};
class CCombatActionBase {
public:
    virtual void Initialise();                        // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x74 - 0x10];
    CCountedPointer<CAnimationEntry> PAnimEntry;      // +0x74
    unsigned char _pad7c[0x114 - 0x7c];
    void SetCombatAnimation(const CCharString& name); // 0x008574D0
};
class CCombatAction_GenericResponseToDecapitation : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCombatAction_GenericResponseToDecapitation::Initialise()
{
    {
        CCharString name("RESPONSE_GENERIC_REACT_TO_DECAPITATION");
        SetCombatAnimation(name);
    }
    CTCDecapitation* decapitation;
    if (PCreature.GetPItem()->GetTC(&decapitation))
        decapitation->Decapitate();
    PCreature.GetPItem()->ModifyHealth(-PCreature.GetPItem()->GetMaxHealth(), true);
    CCombatActionBase::Initialise();
    CDefString nextFilter;
    {
        CAnimComponentFollowingOnAnimFilter* filter;
        PAnimEntry->GetComponent(&filter);
        nextFilter = filter->NextFilter;
    }
    CCharString group = nextFilter.GetString();
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetTC(&modes);
    modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeKnockedDown(*PCreature.GetPItem(), group)));
}