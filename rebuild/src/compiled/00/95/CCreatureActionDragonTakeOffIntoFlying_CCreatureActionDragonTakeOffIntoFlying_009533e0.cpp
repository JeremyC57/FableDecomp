// CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(CThingCreatureBase*, const vector<C3DVector>&) -- combat action with the SWORD_SWING sound pair (action code 250), copies the movement points, UsePreviousPoint = true, empty end-action pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009533E0, length 119 bytes, `ret 8`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString @ 0x0099EAE0, the out-of-line copy constructor of the 12-byte retail
//          vector<C3DVector> @ 0x006F0810 (landed CBasicShape model). Own vtable 0x01293A7C.
// Shape: base call as in the landed SnapDragon family (implicit literal temporary, hidden-pointer CSoundPair); the
//   vtable store is scheduled between the vector-copy argument push and the copy call; then the initialisers in
//   declaration order: UsePreviousPoint byte 1, and the CCountedPointer's inline constructor zeroing Data/Info from
//   one zero register.
// Layout: engine/CCreatureAction_DragonTakeOffIntoFlying.h is the PDB build (base 0x12c, 16-byte vectors): the retail
//   base is 0x114 and the vector 12 bytes, so MovementPoints sits at +0x138, UsePreviousPoint at +0x144 and the
//   PEndAction counted pointer at +0x148 here (the Dest/StartPos/PreviousPoint vectors at +0x114..+0x137 are untouched).
struct CThingCreatureBase;
struct CCreatureActionBase;
struct CCPPointerInfo;

struct CCharString {                        // one pointer; the defaulted length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                         // user constructor: the 8-byte result comes back through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct C3DVector { float X, Y, Z; };

template <class T>
struct CRetailVector {                      // VC6-era Dinkumware vector: first / last / end (12 bytes)
    T* first;
    T* last;
    T* end;
    CRetailVector(const CRetailVector& other);          // 0x006F0810 (out of line)
};

template <class T>
struct CCountedPointer {                    // Data + Info, both cleared by the inline default constructor
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCreatureAction_DragonTakeOffIntoFlying : CCombatActionBase {   // vtable 0x01293A7C
    C3DVector Dest;                                       // +0x114
    C3DVector StartPos;                                   // +0x120
    C3DVector PreviousPoint;                              // +0x12c
    CRetailVector<C3DVector> MovementPoints;              // +0x138
    unsigned char UsePreviousPoint;                       // +0x144
    CCountedPointer<CCreatureActionBase> PEndAction;      // +0x148
    virtual void v00();
    CCreatureAction_DragonTakeOffIntoFlying(CThingCreatureBase* owner, const CRetailVector<C3DVector>& points);
};

CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(CThingCreatureBase* owner, const CRetailVector<C3DVector>& points)
    : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING"))
    , MovementPoints(points)
    , UsePreviousPoint(1)
    , PEndAction()
{
}