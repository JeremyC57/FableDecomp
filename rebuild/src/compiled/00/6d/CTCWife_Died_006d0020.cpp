// CTCWife::Died() -- a married wife's death unmarries her and sets the living hero's marriage state to 6 (widowed)
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0020, 120 bytes (+8 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::Unmarry(bool) 0x006CFF10; CPlayerManager::GetMainPlayer()
//          0x00449970; CPlayer::GetPControlledCreature() 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020;
//          CTCHeroStats::SetMarriageState(CThingAICreature*, EMarriageState) 0x0057BC6E. Global g_pMainGameComponent 0x013B86A0.
// Shape: same guard as OnGreet; the player manager is read from the main game component's +0x1C (not through CTCBase);
//        interface 4 is CTCHeroStats (bitset byte +0x20 bit 4), called through the out-parameter lookup form.
// Layout: CTCWife ParentThing +0x04, FrameGotMarriedToThePlayer +0x20; CMainGameComponent PlayerManager +0x1C.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020 (the other TU copy is 0x004365B0)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6C;                      // +0x6c (bit 4 tested, descriptive)
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    unsigned char _pad92[0x1c9 - 0x92];
    unsigned char Flags1C9;                     // +0x1c9 (CThingCreatureBase flag byte; bit 3 tested, descriptive)
    const C3DVector& GetPos() const;       // 0x004C73D0
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
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;       // 0x00449960 (Ego_r.pdb)
    CPlayer& GetMainPlayer();                   // 0x00449970 (Ego_r.pdb)
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
class CMainGameComponent { public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;               // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
class CTCHeroStats { public:
    void SetMarriageState(CThing* spouse, long state);   // 0x0057BC6E
};
class CTCWife { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[0x20 - 0x08];
    long FrameGotMarriedToThePlayer;             // +0x20
    void Unmarry(bool notify);                   // 0x006CFF10
    void Died();
};
void CTCWife::Died()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetFrame())
        return;
    Unmarry(true);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || (hero->Flags91 & 1))
        return;
    CTCHeroStats* stats;
    if (hero->GetInterface(&stats, 4))
        stats->SetMarriageState(ParentThing, 6);
}