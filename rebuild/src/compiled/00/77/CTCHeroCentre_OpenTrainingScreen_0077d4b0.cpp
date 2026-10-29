// CTCHeroCentre::OpenTrainingScreen() -- if the hero still has the training tutorial to see, show it and wait for it to finish; otherwise, for a hero that can train, queue game event 0x1a (open the training screen, payload 0xb) for the main player
// Leaf: OpenTrainingScreen
// Flags: /GS
//
// Retail 0x0077D4B0, 256 bytes incl. padding (plain `ret`, /GS cookie 0x0139C8A8). Ego_r.pdb `?OpenTrainingScreen@CTCHeroCentre@@...`.
// Callees: CTCBase::PeekPlayerManager const @ 0x00686D50 (twice), CPlayerManager::GetMainPlayer const @ 0x00449960,
// CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for
// CTCHero, type 0x29 = bit 9 of the bitset word +0x24, the component null-tested in ecx), CTCHero::GiveTutorialForCategory
// (ETutorialCategory, bool) @ 0x006C9FA0 (category 0xf), CMemoryBuffer::Copy @ 0x009F1760, CPlayerManager::GetMainPlayerNumber
// @ 0x00449700, CMainGameComponent::AddGameEvent @ 0x0041649C on g_pMainGameComponent 0x013B86A0, __security_check_cookie
// @ 0x00BFE9F9.
// Layout: Ego_r.pdb CTCHeroCentre with retail 12-byte vectors: WaitingForTutorialToFinish +0x30 (PDB +0x38; the engine header
// carries the PDB offsets); retail CThing bitset +0x20 (type 0x16 = bit 22 of the first word), interface map +0x44, dead bit +0x91.
// Shape: the landed CTCShop::OpenShoppingInterface 0x00773B20 event model (CGameEvent over an empty CMemoryBuffer with an inline
// by-value AddULONG); the tutorial branch returns early with its own cookie check; the out-parameter GetInterface stores the
// end/hit choice in one ternary so the component lands in ecx.
class CThing;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CTCHero { public:
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0 (ETutorialCategory)
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                             // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                     // 0x00449960
    long GetMainPlayerNumber() const;                         // 0x00449700
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};
class CTCHeroCentre : public CTCBase { public:
    void*         PDef;                                 // +0x0c CDefPointer<CHeroCentreDef const>
    long          CurrentGUIMode;                       // +0x10
    unsigned long FrameQuestCardListFrom;               // +0x14
    unsigned char QuestCards[0x0c];                     // +0x18 vector<CIntelligentPointer<CThing>> (retail 12 bytes)
    unsigned char QuestCardPositions[0x0c];             // +0x24 vector<C3DVector> (retail 12 bytes)
    bool          WaitingForTutorialToFinish;           // +0x30
    void OpenTrainingScreen();
};

void CTCHeroCentre::OpenTrainingScreen()
{
    CThing* hero = PeekPlayerManager().GetMainPlayer()->GetPControlledCreature();
    if (hero && !(hero->m_flags91 & 1)) {
        CTCHero* heroTC;
        if (hero->GetInterface(&heroTC, 0x29) && heroTC && heroTC->GiveTutorialForCategory(0xf, false)) {
            WaitingForTutorialToFinish = true;
            return;
        }
        if (hero->HasTC(0x16)) {
            CGameEvent event(0x1a);
            event.AddULONG(0xb);
            event.Player = (signed char)PeekPlayerManager().GetMainPlayerNumber();
            g_pMainGameComponent->AddGameEvent(event);
        }
    }
}