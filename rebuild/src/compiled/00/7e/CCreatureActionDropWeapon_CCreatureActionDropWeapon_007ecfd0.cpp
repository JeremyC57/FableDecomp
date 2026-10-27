// CCreatureAction_DropWeapon::CCreatureAction_DropWeapon(CThingCreatureBase& owner) -- the base action for the owner: zero timings, the owner's random loaded "ST_IDLE" animation, an empty sound pair, flags 0 / 0
// Leaf: ??0CCreatureAction_DropWeapon@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ECFD0, 101 bytes, thiscall, `ret 4`. Vtable 0x01271274.
// Identity (proven): it stores vtable 0x01271274 whose RTTI is .?AVCCreatureAction_DropWeapon@@; Ego_r.pdb ??0CCreatureAction_DropWeapon@@QAE@AAVCThingCreatureBase@@@Z;
//   the literal 0x0127138C is "ST_IDLE"; its caller is the carried-action use/drop handler 0x007ED0A0. The manifest's "CCreatureAction_AlertToPlayer" is propagated.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCreatureActionBase::GetRandomAnimFromName(const
//   CThingCreatureBase&, const CCharString&) 0x006924B0 (`__fastcall`), CCreatureActionBase::CCreatureActionBase(
//   CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30,
//   CCharString::~CCharString() 0x0099EAE0.
// Shape (family M171, representative 0x0062D740): the landed constructors 0x008C7F60 / 0x00842AF0 (family K028) with both
//   timings zero -- the CSoundPair() zeros are then stored after its address is pushed.
class CAnimationEntry;
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};
class CThingCreatureBase {
public:
    virtual ~CThingCreatureBase();
};
class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];                           // retail sizeof(CCreatureActionBase) 0xA8
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                const CCharString& name);   // 0x006924B0
};
class CCreatureAction_DropWeapon : public CCreatureActionBase {
public:
    CCreatureAction_DropWeapon(CThingCreatureBase& owner);
};
CCreatureAction_DropWeapon::CCreatureAction_DropWeapon(CThingCreatureBase& owner)
    : CCreatureActionBase(owner, 0.0f, 0.0f, GetRandomAnimFromName(owner, "ST_IDLE"), CSoundPair(), 0, 0)
{
}