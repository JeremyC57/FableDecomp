// Grade: DIFFER(770v786) -- GetValidPositions argument evaluation order (retail fetches the search tools' world before pushing), the predicate's radius spilled to a temp before GetPosition, the Random range test kept (ours drops it: count known non-zero), register choice (retail keeps a zero in edi, def in ebx, nav in ebp); 4 grades spent
// CAIStateGroup_MazeTutorialBattleTeleport::Update() -- pick a random navigable valid position within 15 of the creature that is clear of the target (2x the maze def's enflame range) and of the creature (7), teleport there (or back to the last teleport position when none is reachable), stamp the frame and terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956060, length 800 bytes (+ int3 padding), plain `ret`, frame 0x158.
// Callees: CThing::GetPosition @ 0x004C73D0, GetSubDef<CMazeBattleDef> @ 0x008C3510, GetValidPositions @ 0x0074A5D0
// (__fastcall), CAIStateGroupBase::GetPWorld @ 0x008FCFC0 / Terminate @ 0x008FCF40, remove_if<C3DVector*, within-radius>
// @ 0x008AC060 and __copy<C3DVector*> @ 0x0074CA20 (both __fastcall, /Gr TU), CWorld::GetRandomSeed @ 0x0049D810,
// GFROR13 @ 0x00497890, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos @ 0x006CB620, CCreatureAction_TeleportOut
// ctor @ 0x008BCF20 / ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70, GFGetCurrentFrame
// @ 0x0049D870, free @ 0x00BFEA14.
// Layout: local model (PBrain +4; PTarget +0x14; FrameOfLastTeleport +0x20 / LastTeleportPos +0x24 from the engine header,
// which flattens the base; CMazeBattleDef retail +0x4c = donor EnflameRange +0x50; creature definition +0x70).
// Shape: STLport erase(first,last) / erase(pos) written out; the def pointer releases without a null test.
extern "C" void __cdecl free(void* block);
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
C3DVector* __fastcall CopyPositions(C3DVector* first, C3DVector* last, C3DVector* result, const random_access_iterator_tag& tag, long* distance);   // 0x0074CA20
struct CPositionWithinRadius {
    float Radius;
    C3DVector Centre;
    CPositionWithinRadius(C3DVector centre, float radius) : Radius(radius), Centre(centre) { }
};
C3DVector* __fastcall RemoveWithinRadius(C3DVector* first, C3DVector* last, CPositionWithinRadius pred);   // 0x008AC060
struct CPositionVector {                        // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPositionVector() { if (_M_start) free(_M_start); }
    C3DVector* begin() { return _M_start; }
    C3DVector* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    C3DVector* erase(C3DVector* first, C3DVector* last)
    {
        C3DVector* i = CopyPositions(last, _M_finish, first, random_access_iterator_tag(), (long*)0);
        _M_finish = i;
        return first;
    }
    C3DVector* erase(C3DVector* pos)
    {
        if (pos + 1 != _M_finish)
            CopyPositions(pos + 1, _M_finish, pos, random_access_iterator_tag(), (long*)0);
        --_M_finish;
        return pos;
    }
};
struct CWorldMap;
struct CThingSearchTools;
struct CTCShapeManager;
void __fastcall GetValidPositions(CPositionVector& positions, const CWorldMap& map, const CThingSearchTools& search,
                                  const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes);   // 0x0074A5D0
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
long __fastcall GFGetCurrentFrame();                     // 0x0049D870
struct CWorld {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CWorldMap* GetWorldMap();           // slot 12 (+0x30)
    unsigned char _pad04[0x1c];
    CThingSearchTools* ThingSearchTools;        // +0x20
    unsigned long* GetRandomSeed();             // 0x0049D810
    static long GetFrameCount() { return GFGetCurrentFrame(); }
    static __forceinline unsigned long Random(unsigned long range, unsigned long* seed)
    {
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                     // slot 1
};
struct CRefCounted { long m_count; };
struct CDefBase : CDefVirtual, CRefCounted {
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CMazeBattleDef : CDefBase {
    unsigned char _pad08[0x44];
    float EnflameRange;                         // +0x4c (donor +0x50)
};
struct CDefPointer {
    CMazeBattleDef* m_p;
    CDefPointer() { }
    ~CDefPointer() { m_p->Release(); }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer& out);           // 0x008C3510
};
struct CTCCreatureNavigation {
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620
};
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
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* Definition;                  // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x120];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_TeleportOut : CCombatActionBase {
    CCreatureAction_TeleportOut(CThingCreatureBase* creature, const C3DVector* pos, bool flag);   // 0x008BCF20
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_MazeTutorialBattleTeleport : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    unsigned char _pad1c[4];
    long FrameOfLastTeleport;                   // +0x20
    C3DVector LastTeleportPos;                  // +0x24
    void Update();
};

void CAIStateGroup_MazeTutorialBattleTeleport::Update()
{
    C3DVector teleportPos = GetCreature()->GetPosition();
    CDefPointer def;
    def.m_p = 0;
    GetCreature()->Definition->GetSubDef(def);
    CPositionVector positions;
    GetValidPositions(positions, *GetPWorld()->GetWorldMap(), *GetPWorld()->ThingSearchTools, GetCreature()->GetPosition(),
                      15.0f, false, 0);
    positions.erase(RemoveWithinRadius(positions.begin(), positions.end(),
                        CPositionWithinRadius(PTarget.GetPItem()->GetPosition(), def.m_p->EnflameRange * 2.0f)),
                    positions.end());
    positions.erase(RemoveWithinRadius(positions.begin(), positions.end(),
                        CPositionWithinRadius(GetCreature()->GetPosition(), 7.0f)),
                    positions.end());
    unsigned long count = positions.size();
    if (count == 0) {
        Terminate();
    } else {
        CTCCreatureNavigation* nav;
        GetCreature()->GetInterface(&nav, 7);
        do {
            unsigned long index = CWorld::Random(count, GetPWorld()->GetRandomSeed());
            if (nav->IsAbleToNavigateFromPosToPos(GetCreature()->GetPosition(), positions._M_start[index], 0.0f)) {
                teleportPos = positions._M_start[index];
                goto teleport;
            }
            positions.erase(positions._M_start + index);
            count = positions.size();
        } while (count != 0);
        teleportPos = LastTeleportPos;
    }
teleport:
    AIAction_PerformActionAndWait(&CCreatureAction_TeleportOut(GetCreature(), &teleportPos, false), false);
    FrameOfLastTeleport = GetPWorld()->GetFrameCount();
    LastTeleportPos = teleportPos;
    Terminate();
}
