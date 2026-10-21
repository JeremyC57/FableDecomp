// CAIStateGroup_MinionGuardReactToCriminal::IsRunnable() -- runnable when the guard's village holds the hero as a criminal, the guard perceives (and can see or smell) the hero, and either the village has no warning string for the current crimes or nobody is handling the criminal yet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00932990, length 435 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookup 0x23 = CTCVillager) and the const LowerBound @ 0x004365B0
// (0x34 = CTCPerceiveThing), GFGetPVillageInterface @ 0x007EE540 (manifest "IS_THING_ALIVE<..>" propagated),
// CTCVillage::IsHeroCriminal @ 0x0065BFCE, CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer
// @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 /
// SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CTCPerceiveThing::GetPerceivedThing @ 0x007446A0 (twice), the this-TU
// operator==(const CThing*, const CIntelligentPointer&) @ 0x00933250 (register arguments; manifest label propagated),
// CTCPerceiveThing::CanSeeOrSmellPerceivedThing @ 0x00661600, CTCVillage::GetWarningStringForCurrentCrimes @ 0x0065A075,
// CBasicString<char>::operator== @ 0x004115A0, ~CCharString @ 0x0099EAE0, IS_THING_ALIVE<CThing*> @ 0x004A88F0.
// Globals: the empty literal @ 0x0122D70E; CIntelligentPointer<CThingPlayerCreature> vtable 0x01231C4C.
// Shape: the villager lookup is the out-pointer GetInterface whose miss path passes the never-written word; nested ifs
// share one false exit; `== ""` is the inline CCharString compare (strcmp intrinsic on the null block); the pointer
// operator= names the item before SetPItem, which keeps the &PHero lea after the temporary's GetPItem call.
// Layout: Ego_r.pdb CAIStateGroup_MinionGuardReactToCriminal (PHero +0x14); the engine header flattens the base, so the
// model is local. CTCVillage::PCriminalHandler +0xdc as in the landed 0x009331E0.
#include <string.h>

struct CTCBase;
struct CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
template<class T> bool __fastcall IS_THING_ALIVE(T* const& thing);     // 0x004A88F0 <CThing*>

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
bool __fastcall operator==(const CThing* thing, const CIntelligentPointer& pointer);   // 0x00933250

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)

struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};

struct CTCVillage {
    unsigned char _pad0[0xdc];
    CBaseIntelligentPointer PCriminalHandler;   // +0xdc the guard assigned to the current criminal
    bool IsHeroCriminal();                      // 0x0065BFCE
    CCharString GetWarningStringForCurrentCrimes(long crime, bool& isFine, long& amount) const;   // 0x0065A075
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CTCPerceiveThing {                       // interface 0x34
    CThing* GetPerceivedThing();                // 0x007446A0
    bool CanSeeOrSmellPerceivedThing() const;   // 0x00661600
};

struct CAIBrain {
    virtual void v00();
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
};

struct CAIStateGroup_MinionGuardReactToCriminal : CAIStateGroupBase {
    CIntelligentPointer PHero;                  // +0x14
    bool IsRunnable();
};

bool CAIStateGroup_MinionGuardReactToCriminal::IsRunnable()
{
    CTCVillager* villager;
    PBrain->PCreature->GetInterface(&villager, 0x23);
    CTCVillage* village = GFGetPVillageInterface(villager);
    if (village != 0 && village->IsHeroCriminal()) {
        const CThing* creature = PBrain->PCreature;
        if (creature->HasTC(0x34)) {
            CTCPerceiveThing* perceive = creature->GetTC<CTCPerceiveThing>(0x34);
            PHero = GetPPlayerManager()->GetMainPlayer()->GetHeroThing();
            if (PHero.GetPItem() != 0 && perceive->GetPerceivedThing() != 0
                && perceive->GetPerceivedThing() == PHero && perceive->CanSeeOrSmellPerceivedThing()) {
                bool isFine;
                long amount;
                bool noWarning = village->GetWarningStringForCurrentCrimes(0, isFine, amount) == "";
                if (noWarning)
                    return true;
                return !IS_THING_ALIVE(village->PCriminalHandler.GetPItem());
            }
        }
    }
    return false;
}
