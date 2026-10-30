// CAIStateGroup_JackDragon::UpdateStrafe() -- with both strafe markers alive and physical, aim along the marker line (heading as a fraction of a turn, wrapped into [0,1)), build the strafe route from the creature through the chosen side's points and hand the dragon a strafe-burninate mode; then terminate when the enemy is dead or the state stops, otherwise flip sides, remember the strafe start and pick the into-ground state
// Grade: DIFFER(892v886) nd=68 (was 896v886 nd=75) -- levers: per-arm GetPItem ternaries for from/to (75->69), offset vector constructed before the heading (69->68); tried: if/else marker select (124); remaining: retail pushes ebx/ebp in the prologue, frame 0x34 v 0x38, the heading fraction kept with fst (ours fstp+fld), marker register pair edi/ebx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00950CF0, length 886 bytes (+ int3 padding), plain `ret`.
#include <stdlib.h>
#include <math.h>
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
    void push_back(const C3DVector& x);         // 0x0066E630
};
struct CTCCreatureModeManager;
struct CModeBase { virtual void v00(); };
struct CModeDragonStrafeBurninate : CModeBase {
    unsigned char _body[0xc4];
    CModeDragonStrafeBurninate(CThing* creature, CVector3DList* route, C3DVector* previous, float a, float b,
                               C3DVector* first, C3DVector* second);   // 0x008E1580
};
struct CModePointer {
    CModeBase* m_pObject;
    void* m_pCount;
    CModePointer(CModeBase* object);            // 0x00718A00
};
struct CTCCreatureModeManager {
    void AddMode(CModePointer mode);            // 0x0073ACC0
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
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
};
#pragma pack(push, 1)
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    CBaseIntelligentPointer PLandedMarker;      // +0x1c
    CBaseIntelligentPointer PHoverMarker1;      // +0x24
    CBaseIntelligentPointer PHoverMarker2;      // +0x2c
    CBaseIntelligentPointer PStrafeMarker1;     // +0x34
    CBaseIntelligentPointer PStrafeMarker2;     // +0x3c
    unsigned char _pad44[0x5c - 0x44];
    long CurrentState;                          // +0x5c
    long GoLeft;                                // +0x60
    bool BattleTriggered;                       // +0x64
    C3DVector PreviousFlightPos;                // +0x65
    unsigned char _pad71[0xf6 - 0x71];
    C3DVector StrafeStartLeft;                  // +0xf6
    C3DVector StrafeStartRight;                 // +0x102
    C3DVector StrafeMidLeft;                    // +0x10e
    C3DVector StrafeMidRight;                   // +0x11a
    C3DVector StrafeEndLeft;                    // +0x126
    C3DVector StrafeEndRight;                   // +0x132
    long ChooseIntoGroundState();               // 0x00950A30
    void UpdateStrafe();
};
#pragma pack(pop)

void CAIStateGroup_JackDragon::UpdateStrafe()
{
    if (IS_THING_ALIVE(PStrafeMarker1) && IS_THING_ALIVE(PStrafeMarker2)
        && PStrafeMarker1.GetPItem()->PhysicsBody != 0 && PStrafeMarker2.GetPItem()->PhysicsBody != 0) {
        CThing* from = GoLeft ? PStrafeMarker2.GetPItem() : PStrafeMarker1.GetPItem();
        CThing* to = GoLeft ? PStrafeMarker1.GetPItem() : PStrafeMarker2.GetPItem();
        const C3DVector& a = from->GetPosition();
        const C3DVector& b = to->GetPosition();
        C3DVector offset(0.0f, -60.0f, 60.0f);
        float turn = (float)atan2(b.X - a.X, b.Y - a.Y) * 0.15915494f;
        if (turn < 0.0f || turn >= 1.0f) {
            turn = (float)fmod((double)turn, 1.0);
            if (turn < 0.0f)
                turn += 1.0f;
        }
        offset.RotateAboutZ(turn);
        from->GetPosition();
        CVector3DList route;
        route.push_back(GetCreature()->GetPosition());
        C3DVector first;
        C3DVector second;
        if (GoLeft) {
            route.push_back(StrafeStartLeft);
            route.push_back(StrafeStartRight);
            route.push_back(StrafeMidRight);
            first = StrafeEndRight;
            second = StrafeEndLeft;
        } else {
            route.push_back(StrafeStartRight);
            route.push_back(StrafeStartLeft);
            route.push_back(StrafeMidLeft);
            first = StrafeEndLeft;
            second = StrafeEndRight;
        }
        CThingCreatureBase* creature = GetCreature();
        if (creature->m_presentBits[0x31 >> 5] & (1UL << (0x31 & 31))) {
            CTCCreatureModeManager* modes;
            creature->GetInterface(&modes, 0x31);
            modes->AddMode(CModePointer(new CModeDragonStrafeBurninate(GetCreature(), &route, &PreviousFlightPos,
                                                                       0.15f, 0.6f, &first, &second)));
            AIAction_DoNothing();
        }
    }
    if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
        Terminate();
        return;
    }
    if (GoLeft) {
        PreviousFlightPos = StrafeStartRight;
        GoLeft = 0;
        CurrentState = ChooseIntoGroundState();
        return;
    }
    PreviousFlightPos = StrafeStartLeft;
    GoLeft = 1;
    CurrentState = ChooseIntoGroundState();
}
