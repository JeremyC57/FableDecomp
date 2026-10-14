// CCreatureAction_SpiritHitResponse::CCreatureAction_SpiritHitResponse(CThingCreatureBase*, const C3DVector*) -- base CCreatureActionBase(owner, zeroed CSoundPair, 0, 0), own vtable (0x01275B34), the hit direction at +0xa8 copied from the pointer argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00854610, length 80 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long) @ 0x00693C70.
// Shape: the sound pair is zeroed by its inline constructor (register-zeroed stack stores; the two zero arguments come from the
// same register); the pointer argument is loaded after the base call, the three floats copied one register each after the vtable
// store (init-list member after the base; the memberwise copy constructor gives one register per dword, a POD would go through a re-based pointer). Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4): `this` is modelled locally. `ret 8`.
struct CThingCreatureBase;
struct C3DVector {                          // memberwise user copy constructor: retail copies one register per dword
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long interruptionGroup, long interruptionPriority);   // 0x00693C70
};

struct CCreatureAction_SpiritHitResponse : CCreatureActionBase {   // vtable 0x01275B34
    C3DVector m_hitDirection;               // +0xa8
    virtual void v00();
    CCreatureAction_SpiritHitResponse(CThingCreatureBase* owner, const C3DVector* direction);
};

CCreatureAction_SpiritHitResponse::CCreatureAction_SpiritHitResponse(CThingCreatureBase* owner, const C3DVector* direction)
    : CCreatureActionBase(owner, CSoundPair(), 0, 0)
    , m_hitDirection(*direction)
{
}