// CTCWife::OnGreet() -- once married, a greeting from the living hero posts opinion deed 0x50 at the wife's position
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CFC00, 140 bytes (+4 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCBase::GetPlayerManager() 0x00686D40; CPlayerManager::GetMainPlayer()
//          0x00449970; CPlayer::GetPControlledCreature() 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020;
//          CThing::GetPos() 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType, const C3DVector*, CThing*,
//          CThingAICreature*, EOpinionPostDeedType, bool, bool, CFaction*, bool, long) 0x006C6990.
// Shape: FrameGotMarriedToThePlayer (+0x20) must not be in the future; the hero's interface 0x51 (CTCHeroOpinionDeedLog,
//        bitset word 2 bit 17: `test dword ptr [eax+28h],20000h`) is fetched into a named local (esi) and, when the wife's
//        parent has bit 4 of its +0x6C byte set, PostDeed(0x50, &pos, parent, parent, 1, false, false, 0, false, 0).
// Layout: CTCWife ParentThing +0x04, FrameGotMarriedToThePlayer +0x20; retail CThing bitset +0x20, interface map +0x44,
//         Dead +0x91.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (the other TU copy is 0x004365B0)
};
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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)
    CPlayer* GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
class CFaction;
class CThingAICreature;
class CTCHeroOpinionDeedLog { public:
    long PostDeed(long deedType, const C3DVector* position, CThing* thing, CThing* creature, long postType,
                  bool b1, bool b2, CFaction* faction, bool b3, long value);   // 0x006C6990
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCWife : public CTCBase { public:
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[0x20 - 0x08];
    long FrameGotMarriedToThePlayer;             // +0x20
    void OnGreet();
};
void CTCWife::OnGreet()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetWorldFrame())
        return;
    CThing* hero = GetPlayerManager().GetMainPlayer()->GetPControlledCreature();
    if (!hero || (hero->Flags91 & 1))
        return;
    CTCHeroOpinionDeedLog* deedLog;
    if (hero->GetInterface(&deedLog, 0x51)) {
        if (ParentThing->Flags6C & 0x10)
            deedLog->PostDeed(0x50, &ParentThing->GetPos(), ParentThing, ParentThing, 1, false, false, 0, false, 0);
    }
}