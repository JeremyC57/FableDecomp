// CCreatureAction_DuckAndCover::CCreatureAction_DuckAndCover(CThingCreatureBase*, const C3DVector&) -- combat action with the SWORD_SWING sound pair (action code 150) that remembers the position to duck from
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009717A0, length 112 bytes, `ret 8`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString @ 0x0099EAE0. Own vtable 0x012974A4.
// Shape: base call as in the landed SnapDragon family (implicit literal temporary, hidden-pointer CSoundPair, string
//   destroyed after the base); then the vtable store and the three dwords of the position argument copied straight
//   into +0x114..+0x11c (ecx/edx/eax): three scalar members
//   (a struct member is addressed once with `lea` and copied through it).
// No engine header exists for this class; the retail CCombatActionBase is 0x114 bytes.
struct CThingCreatureBase;

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

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCreatureAction_DuckAndCover : CCombatActionBase {   // vtable 0x012974A4
    float PosX;                             // +0x114 position to duck away from (three scalars: a C3DVector
    float PosY;                             // +0x118   member would copy through `lea ecx,[esi+114h]`)
    float PosZ;                             // +0x11c
    virtual void v00();
    CCreatureAction_DuckAndCover(CThingCreatureBase* owner, const C3DVector& pos);
};

CCreatureAction_DuckAndCover::CCreatureAction_DuckAndCover(CThingCreatureBase* owner, const C3DVector& pos)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING"))
    , PosX(pos.X)
    , PosY(pos.Y)
    , PosZ(pos.Z)
{
}