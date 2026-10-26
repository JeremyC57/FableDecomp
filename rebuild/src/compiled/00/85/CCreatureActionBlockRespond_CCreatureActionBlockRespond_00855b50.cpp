// CCreatureAction_BlockRespond::CCreatureAction_BlockRespond(CThingCreatureBase* creature, CThingCreatureBase* target, long param) -- block-base constructor with the action id 0x96, two flags (+0x124, +0x131) cleared, then the vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855B50, length 64 bytes.
// Callees: CCreatureAction_BlockBase::CCreatureAction_BlockBase(CThingCreatureBase*, CThingCreatureBase*, long, long) @ 0x00855AF0.
// Vtable 0x01276084 (CCreatureAction_BlockRespond, rtti-vtables.tsv). Manifest label "CGameJoystickManager::CGameJoystickManager" is propagated.
// Shape: the third argument is pushed first (base's 4th), then the constant 0x96 (3rd), target, creature; the two byte stores from
// `xor al,al` sit before the vtable store = initialiser-list members. The retail object is 0x138 bytes (PDB 0x14c) so a
// local layout is used.
// Leaf: ??0CCreatureAction_BlockRespond
class CThingCreatureBase;

class CCreatureAction_BlockBase { public:
    virtual ~CCreatureAction_BlockBase();
    unsigned char _body[0x120];                     // +0x04 .. +0x124 (block-base state)
    CCreatureAction_BlockBase(CThingCreatureBase* creature, CThingCreatureBase* target, long actionId, long param);   // 0x00855AF0
};

class CCreatureAction_BlockRespond : public CCreatureAction_BlockBase { public:
    bool m_useHitPos;                               // +0x124
    unsigned char m_hitPos[0xc];                    // +0x125 C3DVector
    bool m_faceTargetAtStart;                       // +0x131
    CCreatureAction_BlockRespond(CThingCreatureBase* creature, CThingCreatureBase* target, long param);
};

CCreatureAction_BlockRespond::CCreatureAction_BlockRespond(CThingCreatureBase* creature, CThingCreatureBase* target, long param)
    : CCreatureAction_BlockBase(creature, target, 0x96, param), m_useHitPos(false), m_faceTargetAtStart(false)
{
}