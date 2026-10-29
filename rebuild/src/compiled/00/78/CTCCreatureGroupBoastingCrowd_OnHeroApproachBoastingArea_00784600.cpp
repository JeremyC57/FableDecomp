// CTCCreatureGroupBoastingCrowd::OnHeroApproachBoastingArea() -- while the quest manager has boasts and the hero has not boasted yet, give the living hero the boasting tutorial (category 5) and enter crowd state 1
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model (landed 0x00584157 idiom) on a GetPItem() thing local, hero zero-initialised: the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00784600, length 135 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (inlined GetInterface for type 0x29 = bit 9 of the presence word at +0x24: `mov ecx,[eax+24h] ; test ch,2`),
// CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0, EnterCrowdState(EBoastingCrowdState) @ 0x00784260.
// Globals: the quest manager @ 0x013B89FC; its retail +0x68 member is a single-pointer STLport list (empty when the
// sentinel links to itself), most likely the PDB's Boasts list.
// Layout: retail = engine/CTCCreatureGroupBoastingCrowd.h - 4 (the retail member map is 12 bytes): PHero +0x24,
// HeroBoasted +0x34. Local model (the header flattens the vptr and has the PDB offsets).
// Shape: one guarded block; the hero tutorial is an inline IsThingAlive test then GetInterface + null test.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CTCHero { public:                                // interface 0x29
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
struct SListNode { SListNode* next; SListNode* prev; };
class CQuestManager { public:
    unsigned char _pad00[0x68];
    SListNode* m_boasts;                        // +0x68 list head (retail 4-byte STLport list)
    bool BoastsEmpty() const { return m_boasts->next == m_boasts; }
};
extern CQuestManager* g_pQuestManager;          // 0x013B89FC

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
class CTCCreatureGroupBoastingCrowd { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x24 - 0x08];          // CTCBase tail + CTCCreatureGroup members
    CIntelligentPointer<CThing> PHero;              // +0x24 CIntelligentPointer<CThing>
    long CrowdState;                            // +0x2c
    long CurrentEncouragementState;             // +0x30
    bool HeroBoasted;                           // +0x34
    bool HeroMadeGoodBoast;                     // +0x35
    void EnterCrowdState(long state);           // 0x00784260
    void OnHeroApproachBoastingArea();
};

void CTCCreatureGroupBoastingCrowd::OnHeroApproachBoastingArea()
{
    if (!g_pQuestManager->BoastsEmpty() && !HeroBoasted) {
        if (PHero.GetPItem() && !PHero.GetPItem()->IsDead()) {
            CTCHero* hero = 0;
            CThing* heroThing = PHero.GetPItem();
            if (heroThing->HasTC(0x29))
                hero = heroThing->GetTC<CTCHero>(0x29);
            if (hero)
                hero->GiveTutorialForCategory(5, false);
        }
        EnterCrowdState(1);
    }
}