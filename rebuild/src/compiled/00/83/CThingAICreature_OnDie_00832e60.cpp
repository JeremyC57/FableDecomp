// CThingAICreature::OnDie() -- a creature killed by a thing that answers the killer query posts the hero-postcard message event; a killer with an opinion deed log posts the murder deeds against the victim's faction and tells the victim's wife it was murdered (otherwise she is told it died); then the base OnDie
// Leaf: OnDie
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDef<T>
// @ 0x007923C0, CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CThing::PeekWorld @ 0x004C7AA0,
// CThingCreatureBase::PeekPos @ 0x006A4D00, CMessageEventManager::AddMessage @ 0x00687730, LowerBound @ 0x0040F020 (non-const) and
// @ 0x004365B0 (const), CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990, CTCWife::Murdered @ 0x006D00A0, CTCWife::Died @ 0x006D0020,
// CThingCreatureBase::OnDie @ 0x00663F50.
// Shape: GetDef gets a same-TU auto_inline(off) stand-in that only writes the def slot (the slot then does not escape, so the
// release at the end is `dec dword ptr [ecx+4] ; jne` off the flags); the def is released by hand through a const local after
// the base OnDie; the faction is read through an inline GetFaction() accessor (second read lands in the dead component's edi);
// the faction component (TC 0x49) goes through the const LowerBound copy.
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44; CThingAICreature's last-attacker intelligent pointer +0xF4
// (descriptive); the world's message event manager +0x60; the faction component (TC 0x49) keeps its faction at +0x18.
class C3DVector;
class CFaction;
class CTCBase;
class CMessageEvent;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CMessageEventDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_count;                                       // +4
};
struct CDefPointer {
    CMessageEventDef* m_p;
    CDefPointer() : m_p(0) { }
};
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x007923C0
};
void* FindDef_2e60(const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer& def)
{
    void* found = FindDef_2e60(name);
    if (!found)
        return false;
    *(void**)&def = found;
    return true;
}
#pragma auto_inline(on)

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority);   // 0x00687730
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};

#define VF(n) virtual void v##n();
class CThing { public:
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49)
    VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    VF(70) VF(71) VF(72) VF(73) VF(74) VF(75) VF(76)
    virtual bool IsKiller();                            // slot 77 (+0x134)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    CDefinitionManager* GetDefinitionManager();         // 0x004C7990
    CWorld* PeekWorld();                                // 0x004C7AA0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
    template <class T> __forceinline const T* GetConstTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (const T*)it->second;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingAICreature;
class CTCHeroOpinionDeedLog { public:
    long PostDeed(long deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  long postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};
struct CTCFaction {
    unsigned char _pad00[0x18];
    CFaction* Faction;                                  // +0x18
    CFaction* GetFaction() const { return Faction; }
};
class CTCWife { public:
    void Murdered();                                    // 0x006D00A0
    void Died();                                        // 0x006D0020
};

class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;               // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
    void OnDie();                                       // 0x00663F50
};

class CThingAICreature : public CThingCreatureBase {
public:
    unsigned char _pad[0xf4 - 0x4c];
    CIntelligentPointer<CThing> LastAttacker;               // +0xf4
    void OnDie();
};

void CThingAICreature::OnDie()
{
    CDefPointer def;
    CThing* killer = LastAttacker.GetPItem();
    if (killer && killer->IsKiller()) {
        bool found;
        {
            CCharString name("STANDARD_MESSAGE_EVENT_MURDER", -1);
            found = GetDefinitionManager()->GetDef(name, def);
        }
        if (found) {
            CMessageEventManager* manager = PeekWorld()->MessageEventManager;
            manager->AddMessage(def.m_p, &PeekPos(), killer, this, 0);
        }
    }
    CTCWife* wife = 0;
    if (HasTC(0xd7))
        wife = GetTC<CTCWife>(0xd7);
    if (killer && killer->HasTC(0x51)) {
        CTCHeroOpinionDeedLog* log = killer->GetTC<CTCHeroOpinionDeedLog>(0x51);
        if (HasTC(0x49)) {
            const CTCFaction* faction = GetConstTC<CTCFaction>(0x49);
            if (faction->GetFaction()) {
                log->PostDeed(9, PeekPos(), this, 0, 2, true, false, faction->GetFaction(), false, 0);
                log->PostDeed(0x3a, PeekPos(), this, 0, 7, false, false, faction->GetFaction(), true, 0);
            }
        }
        if (wife)
            wife->Murdered();
    }
    else if (wife)
        wife->Died();
    CThingCreatureBase::OnDie();
    const CMessageEventDef* object = def.m_p;
    if (object && --const_cast<CMessageEventDef*>(object)->m_count == 0)
        const_cast<CMessageEventDef*>(object)->Destroy();
}