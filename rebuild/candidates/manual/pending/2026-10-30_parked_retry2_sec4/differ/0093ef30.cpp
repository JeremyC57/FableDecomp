// CAIStateGroup_VillageTaskPatrol::InitCreaturePosition() -- while the hero is wanted, try to drop the guard on a navigable spot just in front of the hero (sweeping the angle); otherwise take the village's first unassigned patrol task and auto-place near its start node
// Grade: DIFFER(1175v1161) nd=184 (was 1176v1161 nd=188) -- lever: no user C3DVector copy constructor (the hero-position copies into the filters load all three words before storing, like retail); tried: Nearest compare with a body assignment (188); remaining: frame 0x74 v 0x6c (the PTaskThing reset temporary and the filters 8 bytes higher), the patrol start-node point
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    void RotateAboutZ(float angle);             // 0x00A14040
};
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
};
struct CMatrixFrame {
    CRightHandedSet Set;                        // +0x00
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CPhysicsBody {                           // the thing's +0x60 component
    virtual void v00();
    C3DVector Position;                         // +0x04? (unused)
    float PosX;                                 // +0x0c..
};
struct CTCPhysical {
    unsigned char _vt[4];
    unsigned char _pad04[8];
    float X, Y, Z;                              // +0x0c position
};
struct CThingPhysics {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s0a(); virtual void s0b();
    virtual void s0c(); virtual void s0d(); virtual void s0e(); virtual void s0f(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s1a(); virtual void s1b(); virtual void s1c(); virtual void s1d();
    virtual void s1e();
    virtual void SetPosition(const C3DVector& pos);            // +0x7c
    virtual void SetRotation(const CRightHandedSet& set);      // +0x80
    virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26();
    virtual void s27(); virtual void s28(); virtual void s29(); virtual void s2a(); virtual void s2b(); virtual void s2c();
    virtual void s2d(); virtual void s2e(); virtual void s2f(); virtual void s30(); virtual void s31(); virtual void s32();
    virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38();
    virtual void s39(); virtual void s3a(); virtual void s3b(); virtual void s3c(); virtual void s3d(); virtual void s3e();
    virtual void s3f(); virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44();
    virtual void s45(); virtual void s46(); virtual void s47();
    virtual const CRightHandedSet* GetMatrix(); // +0x120 (forward at +0x0c)
    unsigned char _pad04[8];
    C3DVector Pos;                              // +0x0c position
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPhysics* PPhysics;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    bool AutoPlaceNear(const C3DVector& position, float radius);    // 0x00665DF0
};
struct CTCCreatureNavigation {                  // interface 7
    void Reset();                               // 0x006CB6B0
    float FindPosZAt(const C3DVector& pos);     // 0x006CB6C0
    bool IsPosNavigable(const C3DVector& pos);  // 0x006CBA50
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x0128B090
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CPatrolTask {
    unsigned char _pad[8];
    CBaseIntelligentPointer PStartNode;         // +0x08
};
struct CTCVillage {
    long GetFineAmountForCurrentCrimes();       // 0x0065A41C
    CThing* GetFirstUnAssignedPatrolTask();     // 0x00658A02
    void SetPatrolTaskAsAcceptedByThing(CThing* task, CThing* creature);   // 0x0065852D
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
struct CThingFilter_HasTC {
    long Type;
    CThingFilter_HasTC(long type) : Type(type) { }
};
struct CThingFilter_InRange {
    C3DVector Pos;
    float Radius;
    CThingFilter_InRange(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) { }
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
long Sink_93ef30(const void* a, const void* b, const void* c);
struct CThingSearchTools {
    bool GetPBestThing(const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InRange>& filter, const CThingCompare_Nearest& compare, CThing** out);   // 0x0093F8A0
    bool GetPBestThing(const CThingFilter_InRange& filter, const CThingCompare_Nearest& compare, CThing** out);   // 0x0093F930
};
#pragma auto_inline(off)
bool CThingSearchTools::GetPBestThing(const CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InRange>& filter, const CThingCompare_Nearest& compare, CThing** out)
{
    *out = (CThing*)Sink_93ef30(this, &filter, &compare);
    return *out != 0;
}
bool CThingSearchTools::GetPBestThing(const CThingFilter_InRange& filter, const CThingCompare_Nearest& compare, CThing** out)
{
    *out = (CThing*)Sink_93ef30(this, &filter, &compare);
    return *out != 0;
}
#pragma auto_inline(on)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
struct CPlayer {
    CThing* GetCharacter();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_VillageTaskPatrol : CAIStateGroupBase {
    CTCVillager* PVillager;                     // +0x14
    CIntelligentPointerThing PTaskThing;        // +0x18
    CPatrolTask* Task() const { return (CPatrolTask*)PTaskThing.GetPItem(); }
    virtual bool InitCreaturePosition();
};

bool CAIStateGroup_VillageTaskPatrol::InitCreaturePosition()
{
    PTaskThing = 0;
    CTCVillage* village = GFGetPVillageInterface(PVillager);
    if (village) {
    CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetCharacter();
    if (village->GetFineAmountForCurrentCrimes() > 0 && hero && !(hero->m_flags91 & 1)) {
        CThing* found;
        if (PeekThingSearchTools()->GetPBestThing(CThingFilter_HasTC(0x63) && CThingFilter_InRange(hero->GetPosition(), 0.4f),
                CThingCompare_Nearest(hero->GetPosition()), &found)
            || PeekThingSearchTools()->GetPBestThing(CThingFilter_HasTC(0xdd) && CThingFilter_InRange(hero->GetPosition(), 0.4f),
                CThingCompare_Nearest(hero->GetPosition()), &found)) {
            CThingPhysics* body = hero->PPhysics;
            float angle = 0.0f;
            while (fabs(angle) < 0.25) {
                C3DVector dir = body->GetMatrix()->Up;
                dir.RotateAboutZ(angle);
                C3DVector pos = dir * 8.0f + body->Pos;
                pos.Z = (float)(pos.Z + 1.5);
                CRightHandedSet set = *body->GetMatrix();
                set.RotateAboutAxis(set.Forward, 0.5f);
                CTCCreatureNavigation* navigation;
                if (GetCreature()->GetInterface(&navigation, 7) && navigation->IsPosNavigable(pos)
                    && !PeekThingSearchTools()->GetPBestThing(CThingFilter_InRange(pos, 0.4f), CThingCompare_Nearest(pos), &found)) {
                    CTCCreatureNavigation* nav2;
                    GetCreature()->GetInterface(&nav2, 7);
                    nav2->Reset();
                    pos.Z = navigation->FindPosZAt(pos);
                    GetCreature()->PPhysics->SetPosition(pos);
                    GetCreature()->PPhysics->SetRotation(set);
                    nav2->Reset();
                    return true;
                }
                if (angle > 0.0f)
                    angle = -angle;
                else
                    angle = 0.02f - angle;
            }
        }
    }
    PTaskThing = village->GetFirstUnAssignedPatrolTask();
    if (!PTaskThing.GetPItem() || !Task()->PStartNode.GetPItem())
        return false;
    village->SetPatrolTaskAsAcceptedByThing(PTaskThing.GetPItem(), GetCreature());
    GetCreature()->AutoPlaceNear(Task()->PStartNode.GetPItem()->GetPosition(), 3.0f);
    return true;
    }
    return false;
}
