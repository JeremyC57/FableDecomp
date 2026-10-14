// CCreatureAction_BlockRespond::CCreatureAction_BlockRespond(const CCreatureAction_BlockRespond&) -- copy constructor: base CCreatureAction_BlockBase(other), own vtable (0x01276084), UseHitPos, HitPos and FaceTargetAtStart copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855E80, length 96 bytes.
// Callees: CCreatureAction_BlockBase::CCreatureAction_BlockBase(const CCreatureAction_BlockBase&) @ 0x00855EE0 (stores vtable
// 0x01275F3C = CCreatureAction_BlockBase; the manifest label "CQuickAccessItemButtonGuiDef" is propagated)
// Shape: the implicit memberwise copy after the base copy and the vtable store: the UseHitPos byte (`mov al`), the
// three dwords of HitPos (C3DVector's memberwise copy constructor: one register per dword), the FaceTargetAtStart
// byte. Names from engine/CCreatureAction_BlockRespond.h (PDB +0x13c/+0x13d/+0x149; retail CCreatureAction_BlockBase
// is 0x124 bytes, so +0x124/+0x125/+0x131 here). The manifest label "CCreatureAction_CastBattleCharge" is rejected:
// the vtable stored is CCreatureAction_BlockRespond's (rtti-vtables.tsv).
// The retail layout is byte-packed (bool members followed by unaligned dwords/vectors: +0x115, +0x125...), as the
// PDB-derived engine headers also declare (#pragma pack(push, 1)); the pragma here reproduces that real layout.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

// engine/CCreatureAction_BlockBase.h exists but is not used: its PDB layout is 0x13c bytes (retail 0x124: the base's
// containers are 4 bytes wider there) and carries no copy constructor; the base is modelled locally.
struct CCreatureAction_BlockBase {         // retail sizeof 0x124 (CCombatActionBase 0x114 + BlowStrength + MovementDirection)
    virtual void v00();
    unsigned char _body[0x124 - 4];
    CCreatureAction_BlockBase(const CCreatureAction_BlockBase& other);   // 0x00855EE0
};

#pragma pack(push, 1)
struct CCreatureAction_BlockRespond : CCreatureAction_BlockBase {   // vtable 0x01276084; retail sizeof 0x132
    unsigned char UseHitPos;                // +0x124
    C3DVector     HitPos;                   // +0x125
    unsigned char FaceTargetAtStart;        // +0x131
    virtual void v00();
    CCreatureAction_BlockRespond(const CCreatureAction_BlockRespond& other);
};
#pragma pack(pop)

CCreatureAction_BlockRespond::CCreatureAction_BlockRespond(const CCreatureAction_BlockRespond& other)
    : CCreatureAction_BlockBase(other)
    , UseHitPos(other.UseHitPos)
    , HitPos(other.HitPos)
    , FaceTargetAtStart(other.FaceTargetAtStart)
{
}
