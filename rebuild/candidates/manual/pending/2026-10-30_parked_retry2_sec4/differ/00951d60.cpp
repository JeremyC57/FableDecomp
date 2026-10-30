// CAIStateGroup_JackDragon::IntoHover() -- with both hover markers alive and physical, build the hover-entry route from the creature to the chosen side's marker (offset rotated to the marker's heading and lowered by the def's hover height), start flying along it with a new hover-entry action, terminating when no longer runnable; then switch to the hover state and release the dragon def
// Grade: DIFFER(613v588) nd=111 (was 601v588 nd=124) -- lever: C3DVector::operator+ __forceinline in the return-constructor form (the route point is summed straight into the push_back temporary like retail); tried: def fetched by an explicit GetSubDef out-parameter (149), forceinline with the named-result body (113); engine/CCountedPointer.h not applied: retail calls CCountedPointer<CCreatureActionBase>(T*) 0x00695880 and ~CCountedPointer 0x0048C5C0 out of line; remaining: ebx not kept as the zero register, the def pointer's second frame home (+0x10), frame 0x30 v 0x40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00951D60, length 588 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef @ 0x008E3300, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, C3DVector::RotateAboutZ
// @ 0x00A14040, CThing::GetPosition @ 0x004C73D0, vector<C3DVector>::push_back @ 0x0066E630, operator new @ 0x00BFEA1A,
// hover-entry action ctor @ 0x009534D0, CCountedPointer<CCreatureActionBase> ctor @ 0x00695880 / dtor @ 0x0048C5C0,
// StartFlying @ 0x00950C30, IsStillRunnable @ 0x0094FDF0, Terminate @ 0x008FCF40, free @ 0x00BFEA14.
// Layout: Ego_r.pdb CAIStateGroup_JackDragon (packed): PHoverMarker1 +0x24, PHoverMarker2 +0x2c, CurrentState +0x5c,
// GoLeft +0x60, PreviousFlightPos +0x65.
#include <stdlib.h>
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    void RotateAboutZ(float angle);             // 0x00A14040
};
#pragma pack(pop)
struct CPhysicsBodyV {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41();
    virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53();
    virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61(); virtual void s62();
    virtual float GetHeading();                 // slot 63 (+0xfc)
    virtual void s64(); virtual void s65(); virtual void s66();
    virtual void SetGravity(bool on);           // slot 67 (+0x10c)
};
struct CDefClassBase;
struct CJackDragonDef;
struct CThing {
    unsigned char _pad0[0x60];
    CPhysicsBodyV* PhysicsBody;                 // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    CDefClassBase* GetDef() const { return m_pDef; }
    CJackDragonDef* GetDragonDef() const;
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool b);                          // 0x004C9B80
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CVector3DList {                          // STLport vector<C3DVector>, retail 12 bytes
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CVector3DList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVector3DList() { if (_M_start) free(_M_start); }
    void push_back(const C3DVector& v);         // 0x0066E630
};
struct CJackDragonDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x54 - 0x08];
    float HoverHeight;                          // +0x54
};
struct CDefClassBase {
    bool GetSubDef(CJackDragonDef** out);       // 0x008E3300
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCreatureAction_DragonIntoHover : CCreatureActionBase {
    CCreatureAction_DragonIntoHover(CThingCreatureBase* creature);   // 0x009534D0
};
template<class T> struct CCountedPointer {
    T* Object;
    long* RefCount;
    CCountedPointer(T* p);                      // 0x00695880
    __forceinline CCountedPointer(const CCountedPointer& o) : Object(o.Object), RefCount(o.RefCount) { if (RefCount) ++*RefCount; }
    ~CCountedPointer();                         // 0x0048C5C0
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
    void Terminate();                           // 0x008FCF40
};
#pragma pack(push, 1)
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    CBaseIntelligentPointer PLandedMarker;      // +0x1c
    CBaseIntelligentPointer PHoverMarker1;      // +0x24
    CBaseIntelligentPointer PHoverMarker2;      // +0x2c
    unsigned char _pad34[0x5c - 0x34];
    long CurrentState;                          // +0x5c
    long GoLeft;                                // +0x60
    bool BattleTriggered;                       // +0x64
    C3DVector PreviousFlightPos;                // +0x65
    void StartFlying(CThingCreatureBase* creature, const CVector3DList& route, const C3DVector& previous,
                     CCountedPointer<CCreatureActionBase> action);   // 0x00950C30
    bool IsStillRunnable();                     // 0x0094FDF0
    void IntoHover();
};
#pragma pack(pop)

inline CJackDragonDef* CThing::GetDragonDef() const
{
    CJackDragonDef* def = 0;
    GetDef()->GetSubDef(&def);
    return def;
}
static inline void ReleaseDef(CJackDragonDef* def)
{
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}

void CAIStateGroup_JackDragon::IntoHover()
{
    CJackDragonDef* def = GetCreature()->GetDragonDef();
    if (IS_THING_ALIVE(PHoverMarker1) && IS_THING_ALIVE(PHoverMarker2)
        && PHoverMarker1.GetPItem()->PhysicsBody != 0 && PHoverMarker2.GetPItem()->PhysicsBody != 0) {
        CThing* marker = (GoLeft ? PHoverMarker1 : PHoverMarker2).GetPItem();
        C3DVector offset(0.0f, -10.07981f - def->HoverHeight, 30.148378f);
        C3DVector unused(0.0f, -80.0f, 30.0f);
        offset.RotateAboutZ(marker->PhysicsBody->GetHeading());
        unused.RotateAboutZ(marker->PhysicsBody->GetHeading());
        GetCreature()->PhysicsBody->SetGravity(false);
        CVector3DList route;
        route.push_back(GetCreature()->GetPosition());
        route.push_back(offset + marker->GetPosition());
        CCountedPointer<CCreatureActionBase> action(new CCreatureAction_DragonIntoHover(GetCreature()));
        StartFlying(GetCreature(), route, PreviousFlightPos, action);
        if (!IsStillRunnable()) {
            Terminate();
            ReleaseDef(def);
            return;
        }
    }
    CurrentState = 3;
    ReleaseDef(def);
}
