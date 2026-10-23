//   (reads the Bind3, all three bound filters and the range filter), and the Bind2&&C operator builds the Bind3 straight from
//   the bound operands (*ab.PF1, *ab.PF2, c); together they hoist the &type store above the IsOriginatedByThing ctor as in retail
// CAIStateGroup_MinionKidGroupTag::IsRunnable() -- after seven seconds out of the game: join the nearest kid tag group whose nearest member is within 8, else answer a nearby tag invitation (0x2e) by creating a marker-light group and joining it as the initiator, else post the creature's own invitation unless one is queued
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00934920, length 853 bytes (+ int3 padding), plain `ret`, frame 0x30.
// Callees: GFGetCurrentFrame @ 0x0049D870, CThing::GetPosition @ 0x004C73D0, CThingCreatureBase::GetPosition @ 0x006A4D00,
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000 / GetPWorld @ 0x008FCFC0, CThingSearchTools::GetPBestThing<HasDefGlobalIndex,
// Nearest> @ 0x00935040, CVectorMap::LowerBound @ 0x0040F020 (interface 0xA4 = CTCCreatureGroupTag), CTCCreatureGroup::GetNearestMember
// @ 0x008128F0, the group's virtual slot 28 (+0x70, add member), CMessageEventManager::FindMostRecentMessage @ 0x00934E50 /
// @ 0x0090E090, CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF2B0, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0,
// CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, Create @ 0x00870BB0 (__fastcall), CThing::AddTC @ 0x004C9D60,
// RemoveMessage @ 0x00493B5B, AddMessage @ 0x00687540, __ftol2 @ 0x00BFEA70.
// Layout: CAIStateGroup_MinionKidGroupTag members as the landed Cleanup 0x009347B0 (InitiatedTag +0x14, PTCGroupTag +0x18,
// InitialTagPos +0x1c, FrameLastPlayedIn +0x2c); world definition manager +0x10, message manager +0x60.
// Shape: the distance is sqrt(dz*dz + dy*dy + dx*dx) of component locals; InitialTagPos takes the implicit operator=.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();             // 0x0049D870
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
private:
    void* PStringData;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
inline float GetDistance(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return (float)sqrt(dz * dz + dy * dy + dx * dx);
}
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCParamsBase;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool flag, CTCParamsBase* params);   // 0x004C9D60
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
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingAICreature : CThingCreatureBase { };
CThing* __fastcall Create(long defGlobalIndex, const C3DVector& pos);   // 0x00870BB0
struct CTCCreatureGroupTag {                    // interface 0xA4
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool AddMember(CThing* creature);   // slot 28 (+0x70)
    CThing* GetNearestMember(const C3DVector& pos);   // 0x008128F0
};
struct CThingFilter_HasDefGlobalIndex {
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) { }
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
bool SearchFilterPasses1(const CThingFilter_HasDefGlobalIndex& filter, const CThing* thing);
float SearchDistanceTo(const CThingCompare_Nearest& compare, const CThing* thing);
struct CThingSearchTools {
    CThing** m_things;                          // candidate list (stand-in model)
    CThing** m_thingsEnd;
    bool GetPBestThing_00935040(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** best);   // 0x00935040
};
struct CMessageEvent;
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct CMessageEventFilter_IsOriginatedByThing {
    CThing* Thing;
    long ID;
    CMessageEventFilter_IsOriginatedByThing(CThing* thing);    // 0x006AF2B0
};
struct CThingFilter_IsNotThing {
    CThing* Thing;
    CThingFilter_IsNotThing(CThing* thing) : Thing(thing) { }
};
struct CThingFilter_In2DRange {
    C3DVector Pos;
    float Range;
    __forceinline CThingFilter_In2DRange(const CThing* thing, float range) : Pos(thing->GetPosition()), Range(range) { }
};
template<class F> struct CMessageEventFilter_ThingSearchFilter {
    const F* Filter;
    CMessageEventFilter_ThingSearchFilter(const F& filter) : Filter(&filter) { }
};
template<class F1, class F2> struct CFilter_Bind2 {
    const F1* PF1; const F2* PF2;
    CFilter_Bind2(const F1& f1, const F2& f2) : PF1(&f1), PF2(&f2) { }
};
template<class F1, class F2, class F3> struct CFilter_Bind3 {
    const F1* PF1; const F2* PF2; const F3* PF3;
    CFilter_Bind3(const CFilter_Bind2<F1, F2>& f12, const F3& f3) : PF1(f12.PF1), PF2(f12.PF2), PF3(&f3) { }
    CFilter_Bind3(const F1& f1, const F2& f2, const F3& f3) : PF1(&f1), PF2(&f2), PF3(&f3) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(*ab.PF1, *ab.PF2, c); }
typedef CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNotThing> TSFNotMe;
typedef CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> TSFRange;
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, TSFNotMe, TSFRange> CInvitationFilter;
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, TSFRange> COwnMessageFilter;
struct CMessageEventManager {
    const CMessageEvent* FindMostRecentMessage(const CInvitationFilter& filter) const;      // 0x00934E50
    const CMessageEvent* FindMostRecentMessage(const COwnMessageFilter& filter) const;      // 0x0090E090
    void RemoveMessage(const CMessageEvent* message);                                      // 0x00493B5B
    void AddMessage(long type, unsigned long lifetime, const C3DVector* pos, CThing* from, CThing* to,
                    float range, float radius, long data);                                 // 0x00687540
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld {
    unsigned char _pad[0x10];
    CDefinitionManager* PDefinitionManager;     // +0x10
    unsigned char _pad14[0x4c];
    CMessageEventManager* PMessageEventManager; // +0x60
    CDefinitionManager* GetPDefinitionManager() { return PDefinitionManager; }
    CMessageEventManager* GetPMessageManager() { return PMessageEventManager; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_MinionKidGroupTag : CAIStateGroupBase {
    bool InitiatedTag;                          // +0x14
    CTCCreatureGroupTag* PTCGroupTag;           // +0x18
    C3DVector InitialTagPos;                    // +0x1c
    bool StandModeAddedToCreature;              // +0x28
    long FrameLastPlayedIn;                     // +0x2c
    bool IsRunnable();
};

// GetPBestThing stand-in: the nearest candidate passing the filters.
#pragma auto_inline(off)
bool CThingSearchTools::GetPBestThing_00935040(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** arg2)
{
    CThing* best = 0;
    float bestDistance = 0.0f;
    for (CThing* const* it = m_things; it != m_thingsEnd; ++it) {
        CThing* thing = *it;
        if (!SearchFilterPasses1(filter, thing))
            continue;
        float distance = SearchDistanceTo(compare, thing);
        if (!best || distance < bestDistance) {
            best = thing;
            bestDistance = distance;
        }
    }
    *arg2 = (CThing*)best;
    return best != 0;
}
#pragma auto_inline(on)

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
const CMessageEvent* CMessageEventManager::FindMostRecentMessage(const COwnMessageFilter& filter) const
{
    int hc = 0;
    { const unsigned char* b = (const unsigned char*)&filter; for (unsigned i = 0; i < sizeof(COwnMessageFilter); ++i) hc += b[i]; }
    { const unsigned char* b = (const unsigned char*)filter.PF1; for (unsigned i = 0; i < sizeof(CMessageEventFilter_IsOfType); ++i) hc += b[i]; }
    { const unsigned char* b = (const unsigned char*)filter.PF2; for (unsigned i = 0; i < sizeof(CMessageEventFilter_IsOriginatedByThing); ++i) hc += b[i]; }
    { const unsigned char* b = (const unsigned char*)filter.PF3; for (unsigned i = 0; i < sizeof(TSFRange); ++i) hc += b[i]; }
    { const unsigned char* b = (const unsigned char*)filter.PF3->Filter; for (unsigned i = 0; i < sizeof(CThingFilter_In2DRange); ++i) hc += b[i]; }
    return (const CMessageEvent*)StandInSink(hc, 11);
}
#pragma auto_inline(on)
bool CAIStateGroup_MinionKidGroupTag::IsRunnable()
{
    long framesPerSecond = g_FramesPerSecond;
    if (GFGetCurrentFrame() - FrameLastPlayedIn < framesPerSecond * 7)
        return false;
    InitiatedTag = false;
    {
    CThing* group;
    if (PeekThingSearchTools()->GetPBestThing_00935040(CThingFilter_HasDefGlobalIndex(0xa4),
                                              CThingCompare_Nearest(GetCreature()->CThing::GetPosition()), &group)) {
        group->GetInterface(&PTCGroupTag, 0xa4);
        CTCCreatureGroupTag* tag = PTCGroupTag;
        CThing* nearest = tag->GetNearestMember(GetCreature()->CThing::GetPosition());
        if (nearest && GetDistance(nearest->GetPosition(), GetCreature()->CThing::GetPosition()) < 8.0f
                && PTCGroupTag->AddMember(GetCreature())) {
            InitialTagPos = GetCreature()->GetPosition();
            return true;
        }
    }
    }
    {
        const CMessageEvent* invitation = GetPWorld()->GetPMessageManager()->FindMostRecentMessage(
            CMessageEventFilter_IsOfType(0x2e) && TSFNotMe(CThingFilter_IsNotThing(GetCreature()))
            && TSFRange(CThingFilter_In2DRange(GetCreature(), 8.0f)));
        if (invitation) {
            long markerIndex;
            {
                CCharString markerName("INTERNAL_MARKER_LIGHT");
                markerIndex = GetPWorld()->GetPDefinitionManager()->GetDefGlobalIndexFromName(markerName);
            }
            CThing* marker = Create(markerIndex, GetCreature()->CThing::GetPosition());
            {
                CCharString componentName("CTCCreatureGroupTag");
                PTCGroupTag = (CTCCreatureGroupTag*)marker->AddTC(componentName, false, 0);
            }
            if (PTCGroupTag && PTCGroupTag->AddMember(GetCreature())) {
                GetPWorld()->GetPMessageManager()->RemoveMessage(invitation);
                InitialTagPos = GetCreature()->GetPosition();
                InitiatedTag = true;
                return true;
            }
        }
    }
    if (!GetPWorld()->GetPMessageManager()->FindMostRecentMessage(
            CMessageEventFilter_IsOfType(0x2e) && CMessageEventFilter_IsOriginatedByThing(GetCreature())
            && TSFRange(CThingFilter_In2DRange(GetCreature(), 8.0f)))) {
        CThingAICreature* creature = GetCreature();
        long fps = g_FramesPerSecond;
        CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
        manager->AddMessage(0x2e, (long)(fps * 4.0f), &creature->GetPosition(), creature, 0, 8.0f, 8.0f, 0);
    }
    return false;
}