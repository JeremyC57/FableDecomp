// CCreatureAction_FireMissileTest::CCreatureAction_FireMissileTest(CThingCreatureBase*, const CThing*, long, CAnimationEntry*, float, long, long) -- creature action with the given animation and speed (silent sound pair) aimed at a target, remembering the missile's global index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D5200, length 116 bytes, `ret 1Ch` (seven arguments). Vtable 0x01285544.
// Callees: CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*, const CSoundPair&, long, long)
//          @ 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem
//          @ 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C).
// Shape: eax zeroes the CSoundPair temporary first, then the base arguments are pushed right to left straight from
//   the argument slots (-1.0f immediate); after the vtable store PTarget (addressed once in edi) runs its base
//   constructor, vtable store and SetPItem(target); GlobalIndex is the last initialiser (third argument slot).
// Layout: Ego_r.pdb puts PTarget/GlobalIndex at +0xb4/+0xbc (PDB CCreatureActionBase 0xb4 bytes, retail 0xa8):
//   +0xa8/+0xb0 here; no engine header exists for this class.
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CBaseObject;

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>: vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation,
                        const CSoundPair& sound, long c, long d);                 // 0x00693B30
};

struct CCreatureAction_FireMissileTest : CCreatureActionBase {   // vtable 0x01285544
    CIntelligentPointer<const CThing> PTarget;   // +0xa8 (PDB +0xb4)
    long GlobalIndex;                            // +0xb0 (PDB +0xbc)
    virtual void v00();
    CCreatureAction_FireMissileTest(CThingCreatureBase* owner, const CThing* target, long globalIndex,
                                    CAnimationEntry* animation, float speed, long c, long d);
};

CCreatureAction_FireMissileTest::CCreatureAction_FireMissileTest(CThingCreatureBase* owner, const CThing* target, long globalIndex,
                                                                 CAnimationEntry* animation, float speed, long c, long d)
    : CCreatureActionBase(owner, -1.0f, speed, animation, CSoundPair(), c, d)
    , PTarget(target)
    , GlobalIndex(globalIndex)
{
}
