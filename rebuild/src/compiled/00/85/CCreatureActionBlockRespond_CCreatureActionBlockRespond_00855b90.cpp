// CCreatureAction_BlockRespond::CCreatureAction_BlockRespond(CThingCreatureBase&, const C3DVector&, long) -- block response at a known hit position
// Leaf: ??0CCreatureAction_BlockRespond@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@J@Z
// Retail: Fable.exe @ 0x00855B90, 80 bytes, thiscall `ret 0Ch`, this in eax.
// Identity (proven): the body stores vtable 0x01276084 = CCreatureAction_BlockRespond (RTTI, rtti-vtables.tsv) after calling
// the CCreatureAction_BlockBase constructor 0x00855AF0; Ego_r.pdb has the two BlockRespond constructors
// ??0CCreatureAction_BlockRespond@@QAE@AAVCThingCreatureBase@@0J@Z and ...@AAVCThingCreatureBase@@ABVC3DVector@@J@Z, and
// this one copies a 12-byte C3DVector argument into HitPos (+0x125) and sets UseHitPos (+0x124) -- the C3DVector overload.
// The neighbouring row 0x00855AF0 is the BlockBase constructor, 0x00855CF0 the class's IsOverrideableByActionOfSamePriority.
// Label correction: catalog "_gapscan::sub_00855b90".
// Callees: 0x00855AF0 ??0CCreatureAction_BlockBase@@QAE@AAVCThingCreatureBase@@0JJ@Z (Ego_r.pdb spelling: references;
// reloc_bindings carries the pointer stand-in ??0CCreatureAction_BlockBase@@QAE@PAVCThingCreatureBase@@0JJ@Z from landed
// 0x00855AF0).
// Layout: engine/CCreatureAction_BlockRespond.h agrees (UseHitPos +0x124, HitPos +0x125 packed, FaceTargetAtStart +0x131),
// but it flattens the bases and models the vptr as data, so a constructor calling the base constructor cannot use it:
// local model of the base (retail CCreatureAction_BlockBase is 0x124 bytes). Header debt (NOTES.md).
// Shape: the creature is passed twice and the action code 150 (0x96) between them and the blow strength; then the vtable
// store, UseHitPos = true, the member-wise C3DVector copy (user copy constructor, pack(1) in retail), FaceTargetAtStart = false.
class CThingCreatureBase;

#pragma pack(push, 1)
class C3DVector {
public:
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    float X, Y, Z;
};
#pragma pack(pop)

class CCreatureAction_BlockBase {
public:
    CCreatureAction_BlockBase(CThingCreatureBase& owner, CThingCreatureBase& target, long actionCode, long blowStrength);   // 0x00855AF0
    virtual ~CCreatureAction_BlockBase();
    unsigned char _body[0x124 - 4];                         // CCombatActionBase (0x114) + BlowStrength + MovementDirection
};

#pragma pack(push, 1)
class CCreatureAction_BlockRespond : public CCreatureAction_BlockBase {
public:
    CCreatureAction_BlockRespond(CThingCreatureBase& creature, const C3DVector& hitPos, long blowStrength);
    virtual ~CCreatureAction_BlockRespond();

    bool      UseHitPos;                                    // +0x124
    C3DVector HitPos;                                       // +0x125
    bool      FaceTargetAtStart;                            // +0x131
};
#pragma pack(pop)

CCreatureAction_BlockRespond::CCreatureAction_BlockRespond(CThingCreatureBase& creature, const C3DVector& hitPos, long blowStrength)
    : CCreatureAction_BlockBase(creature, creature, 150, blowStrength)
    , UseHitPos(true)
    , HitPos(hitPos)
    , FaceTargetAtStart(false)
{
}