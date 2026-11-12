// CTCTargetingAI::FrameUpdate() -- prune the things-not-to-attack list (dead or vanished things, and stealthed things that have come within 5 units), then refresh the best enemy creature for attack and the nearest interesting thing
// Leaf: FrameUpdate
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F69B0, 306 bytes (plain `ret`). CTCTargetingAI vtable 0x012411FC slot 9 (the
//   manifest's "CTCTargetingAI::vfunc_9"; slot 9 is the CTCBase FrameUpdate).
// Callees: CTCBase::FrameUpdate @ 0x006868A0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the pointer's virtual
//   destructor (slot 0 with 0) and free @ 0x00BFEA14 (inline list erase), the const interface LowerBound @ 0x004365B0
//   (interface 0x36 = CTCStealth), CTCStealth::IsAvailableForAttack @ 0x00747B00, CThing::GetPos @ 0x004C73D0 (twice),
//   CTCTargetingAI::UpdatePBestEnemyCreatureForAttack(bool) @ 0x007F6200, UpdateNearestInterestingThing (slot 39, +0x9C).
//   25.0f from 0x0123003C.
// Layout: Ego_r.pdb CTCTargetingAI ThingsToNotAttack (STLport list<CIntelligentPointer<CThing> >, the sentinel pointer) +0x48
//   (the landed AddThingToNotAttack 0x007F61B0 model); node {next, prev, data +8}; retail CThing interface bitset +0x20 (0x36
//   = word +0x24 bit 22), map +0x44, dead bit +0x91.
// Shape: an `if (!empty())` guard around the iterator loop; liveness through the inline bool IS_THING_ALIVE (nested ifs,
//   so the erase block follows the tests; an open-coded `!thing || dead` puts it after the stealth block); the inline STLport erase (unlink, virtual destructor, free)
//   returns the next node; the XY distance is `d.X * d.X + d.Y * d.Y` (VC emits the Y term first).
#include "fable_compiler.h"
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CTCBase;
#include "engine/C3DVector.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCStealth { public:
    bool IsAvailableForAttack();                        // 0x00747B00
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CBaseIntelligentPointer _M_data;                    // +0x08
};
struct CThingList {                                     // STLport list<CIntelligentPointer<CThing> >
    SThingListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    SThingListNode* begin() const { return _M_node->_M_next; }
    SThingListNode* end() const { return _M_node; }
    SThingListNode* erase(SThingListNode* position)
    {
        SThingListNode* next = position->_M_next;
        SThingListNode* prev = position->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        position->_M_data.~CBaseIntelligentPointer();
        free(position);
        return next;
    }
};
// agentNF19: the base is the shared retail CTCBase model (engine/CTCBase.h, FABLE_RETAIL_CTCBASE: slots 0..27, vptr +0x00,
// ParentThing +0x04, size 0x0C), so the base call binds ?FrameUpdate@CTCBase@@UAEXXZ (0x006868A0, slot 9).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
class CTCTargetingAI : public CTCBase { public:
    virtual void FrameUpdate();                         // slot 9
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void UpdateNearestInterestingThing();       // slot 39 (+0x9c)
    unsigned char _pad0c[0x48 - 0x0c];
    CThingList    ThingsToNotAttack;                    // +0x48
    void UpdatePBestEnemyCreatureForAttack(bool force); // 0x007F6200
};

void CTCTargetingAI::FrameUpdate()
{
    CTCBase::FrameUpdate();
    if (!ThingsToNotAttack.empty()) {
        for (SThingListNode* it = ThingsToNotAttack.begin(); it != ThingsToNotAttack.end(); ) {
            CThing* thing = ((CThing*)it->_M_data.GetPItem());
            if (!IS_THING_ALIVE(thing)) {
                it = ThingsToNotAttack.erase(it);
                continue;
            }
            if (thing->HasTC(0x36) && thing->GetTC<CTCStealth>(0x36)->IsAvailableForAttack()) {
                C3DVector offset = thing->GetPos() - ParentThing->GetPos();
                if (offset.X * offset.X + offset.Y * offset.Y < 25.0f) {
                    it = ThingsToNotAttack.erase(it);
                    continue;
                }
            }
            it = it->_M_next;
        }
    }
    UpdatePBestEnemyCreatureForAttack(false);
    UpdateNearestInterestingThing();
}