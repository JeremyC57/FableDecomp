// CCreatureAction_FinishFireballSpell::FinishAction() -- base FinishAction; reset the creature's 0xA7 component (flag and
// counter), set the 0x9D component's flag, drop combination animations 1 and 0x23, remove creature mode 0x19 unless the
// player auto-faces, and re-equip the item taken out (fast unsheathe to the combat pose) when it still lives
// Leaf: ?FinishAction@CCreatureAction_FinishFireballSpell
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);             // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer { public:
    unsigned char _pad[0x20a];
    unsigned char m_autoFaceTarget;                   // +0x20a
};
class CThingCreatureBase;
class CCreatureActionBase;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0 (CThingCreatureBase)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};
struct CTCA7 {                                        // interface 0xA7
    unsigned char _pad[0xc];
    bool          m_active;                           // +0x0c
    long          m_count;                            // +0x10
};
struct CTC9D {                                        // interface 0x9D
    unsigned char _pad[0x7c];
    bool          m_flag;                             // +0x7c
};
class CAnimateThing { public:
    void RemoveCombinationAnimsOfType(long type);     // 0x0070D8C0
};
struct CTCAnimation {                                 // interface 0x5A
    CAnimateThing* GetAnimation();                    // 0x0070B460
};
class CTCCreatureModeManager { public:                       // interface 0x31
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
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
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer<CThing> PItemRemoved;             // +0x2c
    unsigned char _pad34[0xa8 - 0x34];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:
    unsigned char _body[0x10];
    CCreatureAction_UnsheatheItemFromInventory(CThing* owner, const CCharString& animation, CThing* item,
                                               long priority, long timeout, CCreatureActionBase* next);   // 0x0069F0F0
    ~CCreatureAction_UnsheatheItemFromInventory();    // 0x005CC091
};
class CCombatActionBase : public CCreatureActionBase { public:      // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    void FinishAction();                              // 0x00859A20
};
class CCreatureAction_FinishFireballSpell : public CCombatActionBase { public:
    void FinishAction();
};

void CCreatureAction_FinishFireballSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCA7* a7;
    if (PCreature.GetPItem()->GetInterface(&a7, 0xa7)) {
        a7->m_active = false;
        a7->m_count = 0;
    }
    CTC9D* c9d;
    if (PCreature.GetPItem()->GetInterface(&c9d, 0x9d))
        c9d->m_flag = true;
    CTCAnimation* animation;
    if (PCreature.GetPItem()->GetInterface(&animation, 0x5a)) {
        CAnimateThing* anim = animation->GetAnimation();
        anim->RemoveCombinationAnimsOfType(1);
        anim->RemoveCombinationAnimsOfType(0x23);
    }
    if (!PCreature.GetPItem()->PeekPlayer().m_autoFaceTarget) {
        CTCCreatureModeManager* modes;
        if (PCreature.GetPItem()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x19);
    }
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString name("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        GetCreature()->SetCurrentAction(&CCreatureAction_UnsheatheItemFromInventory(GetCreature(), name, PItemRemoved.GetPItem(), 100, 250, 0));
    }
}