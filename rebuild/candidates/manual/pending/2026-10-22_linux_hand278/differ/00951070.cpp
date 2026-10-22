// Grade: DIFFER(944v953) -- route building and counted action match in shape; retail frame is 12 bytes larger (unused rotated vector kept), places the success tail after the terminate return with its own route free and def release, and reads the physics body straight into ecx for the heading calls (4 tries)
// CAIStateGroup_JackDragon::IntoGround() -- build the dive-into-ground route (creature position, the chosen side's out-of-ground point, the final point, then the landed marker's position offset along its heading and raised by the def's ground height), fly it with a new ground-dive action; terminate when the enemy is dead or the state stops, otherwise create the invisible bodge creature, reset the boss and timing fields and switch to the in-ground state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00951070, length 953 bytes (+ int3 padding), plain `ret`.
#include <stdlib.h>
inline void* operator new(unsigned int, void* where) { return where; }
void* operator new(unsigned int size);
void operator delete(void* p);
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void RotateAboutZ(float angle);             // 0x00A14040
};
#pragma pack(pop)
struct __true_type { };
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
};
struct CDefClassBase;
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
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBodyV* PhysicsBody;                 // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
    void _M_insert_overflow(C3DVector* position, const C3DVector& x, const __true_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x0066E370
    void push_back(const C3DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DVector(x);
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
    }
};
struct CJackDragonDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x50 - 0x08];
    float GroundHeight;                         // +0x50
};
struct CDefClassBase {
    bool GetSubDef(CJackDragonDef** out);       // 0x008E3300
};
struct CTCBoss {
    unsigned char _pad[0x10];
    long Phase;                                 // +0x10
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCreatureAction_DragonIntoGround : CCreatureActionBase {
    CCreatureAction_DragonIntoGround(CThingCreatureBase* creature, CThing* enemy);   // 0x00953370
};
void __fastcall CountedDestroy(void* object);   // 0x00668610
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(CountedDestroy), m_pObject(object) { }
};
template<class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(T* object)
    {
        m_pObject = object;
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
};
struct CWorld {
    static long __fastcall GetWorldFrame();     // 0x0049D870
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
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
};
#pragma pack(push, 1)
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    CBaseIntelligentPointer PLandedMarker;      // +0x1c
    CBaseIntelligentPointer PHoverMarker1;      // +0x24
    CBaseIntelligentPointer PHoverMarker2;      // +0x2c
    unsigned char _pad34[0x4c - 0x34];
    long LastAttackFrame;                       // +0x4c
    long LastMoveFrame;                         // +0x50
    long StateStartFrame;                       // +0x54
    long AttackCount;                           // +0x58
    long CurrentState;                          // +0x5c
    long GoLeft;                                // +0x60
    bool BattleTriggered;                       // +0x64
    C3DVector PreviousFlightPos;                // +0x65
    unsigned char _pad71[0xd2 - 0x71];
    C3DVector IntoGroundPosLeft;                // +0xd2
    C3DVector IntoGroundPosRight;               // +0xde
    C3DVector IntoGroundPosFinal;               // +0xea
    void StartFlying(CThingCreatureBase* creature, const CVector3DList& route, const C3DVector& previous,
                     CCountedPointer<CCreatureActionBase> action);   // 0x00950C30
    void CreateIBC();                           // 0x0094FC00
    void IntoGround();
};
#pragma pack(pop)

void CAIStateGroup_JackDragon::IntoGround()
{
    CJackDragonDef* def = 0;
    GetCreature()->m_pDef->GetSubDef(&def);
    C3DVector offset(0.0f, -37.930191f, 30.148378f);
    C3DVector unused(0.0f, -80.0f, 30.0f);
    if (IS_THING_ALIVE(PLandedMarker) && PLandedMarker.GetPItem()->PhysicsBody != 0) {
        offset.RotateAboutZ(PLandedMarker.GetPItem()->PhysicsBody->GetHeading());
        unused.RotateAboutZ(PLandedMarker.GetPItem()->PhysicsBody->GetHeading());
    }
    CVector3DList route;
    route.push_back(GetCreature()->GetPosition());
    route.push_back(*(GoLeft != 0 ? &IntoGroundPosLeft : &IntoGroundPosRight));
    route.push_back(IntoGroundPosFinal);
    float height = def->GroundHeight;
    const C3DVector& marker = PLandedMarker.GetPItem()->GetPosition();
    C3DVector sum(offset.X + marker.X, offset.Y + marker.Y, offset.Z + marker.Z);
    C3DVector last(sum.X, sum.Y, sum.Z + height);
    route.push_back(last);
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
    {
        CCountedPointer<CCreatureActionBase> action(new CCreatureAction_DragonIntoGround(GetCreature(), PEnemy.GetPItem()));
        StartFlying(GetCreature(), route, PreviousFlightPos, action);
    }
    bool stop = !IS_THING_ALIVE(PEnemy) || IsStopping();
    if (stop) {
        Terminate();
        if (--def->RefCount == 0)
            def->Destroy();
        return;
    }
    CreateIBC();
    boss->Phase = 0;
    AttackCount = 0;
    LastAttackFrame = LastMoveFrame = GetPWorld()->GetWorldFrame();
    StateStartFrame = GetPWorld()->GetWorldFrame();
    CurrentState = 1;
    if (--def->RefCount == 0)
        def->Destroy();
}
