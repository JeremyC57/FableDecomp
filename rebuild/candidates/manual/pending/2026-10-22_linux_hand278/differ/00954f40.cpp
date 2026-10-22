// Grade: DIFFER(604v604) -- code identical; frame slot order only: retail puts the teleport position at +0x24 below found (+0x30), filter (+0x34), name (+0x38); ours found +0x24, name +0x28, filter +0x2c, position +0x30 (declaration order and scoping have no effect)
// CAIStateGroup_MazeTeleport::Update() -- find the "MazeTeleportPos<phase>" marker within 200 units of Maze and remember its position on the boss component, otherwise pick a random valid navigation position within 20 units (terminating when there is none), then teleport out to that position and terminate
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954F40, length 604 bytes (+ int3 padding), plain `ret`, /GS cookie (sprintf buffer).
// Callees: LowerBound @ 0x0040F020, CTCBoss::GetBossPhase @ 0x007AED10, sprintf @ 0x00BFEB10, CCharString @ 0x0099EBF0 /
// 0x0099EC30 / ~ @ 0x0099EAE0, CThing::GetPosition @ 0x004C73D0, _ftol @ 0x00BFEA70, PeekThingSearchTools @ 0x008FD000,
// GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex,CThing> @ 0x007FD110, GetPWorld @ 0x008FCFC0, CWorld slot 12,
// GetValidPositions @ 0x0074A5D0 (`__fastcall`, 5 stack args), CWorld seed getter @ 0x0049D810, GFROR13 @ 0x00497890,
// free @ 0x00BFEA14, CCreatureAction_TeleportOut ctor @ 0x008BCF20 / dtor @ 0x0062E900 (labelled ~CTCLightningOrb),
// AIAction_PerformActionAndWait @ 0x0097DB70, Terminate @ 0x008FCF40. String "MazeTeleportPos%d" @ 0x012949F0.
#include <stdio.h>
#include <stdlib.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};
struct CCharString {
    void* PData;
    CCharString(const char* text, long length);  // 0x0099EBF0
    CCharString(const CCharString& other);       // 0x0099EC30
    ~CCharString();                              // 0x0099EAE0
};
struct CThingFilter_HasDefGlobalIndex {
    CCharString Name;
    CThingFilter_HasDefGlobalIndex(const CCharString& name) : Name(name) { }
};
struct CRectI {
    long Left, Top, Right, Bottom;
    CRectI(const C3DVector& centre, long radius)
    {
        long x = (long)centre.X;
        long y = (long)centre.Y;
        Left = x - radius;
        Top = y - radius;
        Right = x + radius;
        Bottom = y + radius;
    }
};
struct CPositionVector {
    C3DVector* Start;
    C3DVector* Finish;
    C3DVector* EndOfStorage;
    CPositionVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~CPositionVector() { if (Start) free(Start); }
    long size() const { return Finish - Start; }
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing { };
#pragma pack(push, 1)
struct CTCBoss {                                // interface 0xad
    unsigned char _pad0[0x22];
    C3DVector TeleportPos;                      // +0x22
    long GetBossPhase();                        // 0x007AED10
};
#pragma pack(pop)
struct CNavigationMap;
struct CWorldMap;
void __fastcall GetValidPositions(CPositionVector& out, CNavigationMap* nav, CWorldMap* map, const C3DVector& pos,
                                  float radius, long a, long b);   // 0x0074A5D0
struct CWorld {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CNavigationMap* GetNavigationMap();  // slot 12 (+0x30)
    unsigned char _pad04[0x1c];
    CWorldMap* PWorldMap;                       // +0x20
    unsigned long& GetRandomSeed();             // 0x0049D810
};
struct CThingSearchTools {
    CThing* GetPFirstThingInRoughArea(const CRectI& area, const CThingFilter_HasDefGlobalIndex& filter, CThing** found);   // 0x007FD110
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x120];
};
struct CCreatureAction_TeleportOut : CCreatureActionBase {
    CCreatureAction_TeleportOut(CThingCreatureBase* creature, const C3DVector& pos, bool flag);   // 0x008BCF20
    ~CCreatureAction_TeleportOut();             // 0x0062E900
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_MazeTeleport : CAIStateGroupBase {
    void Update();
};

void CAIStateGroup_MazeTeleport::Update()
{
    CThingCreatureBase* creature = GetCreature();
    CTCBoss* boss;
    if (creature->m_presentBits[0xad >> 5] & (1UL << (0xad & 31))) {
        *(long*)&boss = 0xad;
        CInterfaceMap* map = &creature->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound((long*)&boss);
        boss = (CTCBoss*)((it == map->last || 0xad < it->type) ? map->last : it)->component;
    }
    char text[32];
    sprintf(text, "MazeTeleportPos%d", boss->GetBossPhase());
    CThing* found = 0;
    {
        CCharString name(text, -1);
        PeekThingSearchTools()->GetPFirstThingInRoughArea(CRectI(GetCreature()->GetPosition(), 200),
            CThingFilter_HasDefGlobalIndex(name), &found);
    }
    C3DVector pos;
    if (found) {
        pos = found->GetPosition();
        boss->TeleportPos = pos;
    } else {
        CPositionVector positions;
        CWorld* world = GetPWorld();
        CThingCreatureBase* me = GetCreature();
        CWorldMap* map = GetPWorld()->PWorldMap;
        GetValidPositions(positions, world->GetNavigationMap(), map, me->GetPosition(), 20.0f, 0, 0);
        long count = positions.size();
        if (count == 0)
            Terminate();
        else
            pos = positions.Start[WorldRandomRange(GetPWorld()->GetRandomSeed(), positions.size())];
    }
    AIAction_PerformActionAndWait(&CCreatureAction_TeleportOut(GetCreature(), pos, false), false);
    Terminate();
}
