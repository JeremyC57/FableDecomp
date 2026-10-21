// CAIStateGroup_MinionGuardGreetPlayer::IsRunnable() -- runnable when the loaded region's def allows it, the village's next guard-greeting frame has passed, and the living hero is within 5 units and perceived (and noticed) by the guard
// Grade: DIFFER(448v452) -- register assignment only: retail keeps the region def pointer in ebp and the next-greeting frame / hero in edi, ours swaps them (4 bytes of [ebp] disp8); every instruction otherwise lines up
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929650, length 452 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::PeekWorldMap @ 0x008FCFF0 (twice), CWorldMap::GetLoadedRegion @ 0x004FB150 (manifest label
// "CWorld::DrawGetEnvironment" propagated; it returns LoadedRegion +0x9c), CWorldMap::GetRegion @ 0x004FC180 (manifest
// "GetCategoryWithIndex" propagated; Regions +0x2c, 0x58-byte CRegion), the region def's virtual slot 1 (count release),
// CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x23 = CTCVillager, 0x34 = CTCPerceiveThing), GFGetPVillageInterface
// @ 0x007EE540 (twice; manifest "IS_THING_ALIVE<..>" propagated), __ftol2 @ 0x00BFEA70, CWorld::GetWorldFrame @ 0x0049D870,
// CWorld::GetCurrentPlayer @ 0x0049D9D0, CPlayer::GetHeroThing @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0 (twice),
// CTCPerceiveThing::GetPerceivedThing @ 0x007446A0, CTCPerceiveThing::IsAwareOfThing @ 0x00643B3B.
// Globals: g_pMainGameComponent @ 0x013B86A0 (+0x24 = the world); 5.0f @ 0x012919A0.
// Layout: engine/CAIStateGroup_MinionGuardGreetPlayer.h flattens the base; local model. CRegion from Ego_r.pdb with 12-byte
// retail vectors (PDef +0x24); the frame word read at retail +0xac is taken to be CTCVillage::FrameForNextGuardGreeting (+0x138 in the PDB view); inferred from this function's loads only, not cross-checked against a second reader.
#include <math.h>

struct CTCBase;
struct C3DVector { float X, Y, Z; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

struct CRegionDef {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1: called when the count reaches zero
    long RefCount;                              // +0x04
    unsigned char _pad08[0x95];
    bool Flag9d;                                // +0x9d
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerRegionDef {                   // CDefPointer<const CRegionDef>
    CRegionDef* m_p;
    __forceinline CDefPointerRegionDef(const CDefPointerRegionDef& other) : m_p(other.m_p) { if (m_p) ++m_p->RefCount; }
    __forceinline CDefPointerRegionDef(CRegionDef* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    __forceinline ~CDefPointerRegionDef() { if (m_p) m_p->Release(); }
    CRegionDef* operator->() const { return m_p; }
};
struct CRegion {
    unsigned char _pad0[0x24];                  // Maps, VisMaps (12-byte vectors), Name, DisplayName, RegionDefName
    CDefPointerRegionDef PDef;                  // +0x24
};
struct CWorldMap {
    long GetLoadedRegion() const;               // 0x004FB150
    const CRegion& GetRegion(long index) const; // 0x004FC180
};

struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CWorld {
    static long __fastcall GetWorldFrame();     // 0x0049D870
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
struct CMainGameComponent {
    unsigned char _pad0[0x24];
    CWorld* World;                              // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CTCVillage {
    unsigned char _pad0[0xac];
    long FrameForNextGuardGreeting;             // +0xac (inferred; see Layout note)
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CTCPerceiveThing {                       // interface 0x34
    CThing* GetPerceivedThing();                // 0x007446A0
    bool IsAwareOfThing() const;                // 0x00643B3B
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
    const CWorldMap& PeekWorldMap() const;      // 0x008FCFF0
};

struct CAIStateGroup_MinionGuardGreetPlayer : CAIStateGroupBase {
    bool IsRunnable();
};

bool CAIStateGroup_MinionGuardGreetPlayer::IsRunnable()
{
    CDefPointerRegionDef def = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).PDef.m_p;
    if (def.m_p != 0 && !def->Flag9d)
        return false;
    long nextGreeting = 0x7fffffff;
    CThing* creature = PBrain->PCreature;
    if (creature->HasTC(0x23)) {
        CTCVillager* villager = creature->GetTC<CTCVillager>(0x23);
        if (GFGetPVillageInterface(villager) != 0)
            nextGreeting = (long)(float)GFGetPVillageInterface(villager)->FrameForNextGuardGreeting;
    }
    if (CWorld::GetWorldFrame() > nextGreeting) {
        CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
        if (hero != 0 && !(hero->m_flags91 & 1)) {
            const C3DVector& me = PBrain->PCreature->GetPosition();
            const C3DVector& him = hero->GetPosition();
            float dx = him.X - me.X;
            float dy = him.Y - me.Y;
            if ((float)sqrt(dx * dx + dy * dy) > 5.0f)
                return false;
            CThing* self = PBrain->PCreature;
            if (self->HasTC(0x34)) {
                CTCPerceiveThing* perceive = self->GetTC<CTCPerceiveThing>(0x34);
                CThing* perceived = perceive->GetPerceivedThing();
                if (perceived != 0 && !(perceived->m_flags91 & 1) && perceive->IsAwareOfThing())
                    return true;
            }
        }
    }
    return false;
}
