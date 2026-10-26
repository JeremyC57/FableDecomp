// CTCKraken::HasTargetBeenHitByKraken() -- drop dead tentacle pointers from the list, then ask the live target's entity-events component whether one of the live tentacles has hit it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FB500, length 262 bytes (+ int3 padding), bool in al. FableWin.pdb
// ?HasTargetBeenHitByKraken@CTCKraken@@QAE_NXZ.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / ~ @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), list<CIntelligentPointer<CThing> >::remove(const value&) @ 0x0065DE8A
// (catalog "CEditControlCentre::SetScriptBrushThingsToMove" is propagated), the const component-map LowerBound @ 0x004365B0,
// CTCEntityEvents::GetEvent<NEntityEvents::CEventHitBy, NEntityEvents::CIsHitEventHitBy>(CEventHitBy**, const
// CIsHitEventHitBy&) @ 0x007FCE10.
// Shape: the null intelligent-pointer temporary built for remove() and destroyed straight after (direct base dtor call);
// STLport list::size() as a counting walk (`> 0`, unsigned); IS_THING_ALIVE on the target is the landed double-GetPItem
// helper, then a third GetPItem result feeds HasTC(0x42) (UsingComponents[2] bit 2) and the const map lookup; the tentacle
// walk re-reads end() every pass; the hit predicate holds the tentacle and sits in the lookup key's frame word, the event
// out-pointer in the dead temporary's word.
// Layout: Ego_r.pdb CTCKraken: PTarget +0x20 (CIntelligentPointer<CThing>), Tentacles +0x30 (STLport list: one node
// pointer). No engine header.
class CBaseObject;
class CThing;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CTCBase_Retail;
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 UsingComponents
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                       // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
namespace NEntityEvents {
class CEventHitBy;
class CIsHitEventHitBy { public:
    CThing* HitBy;
    CIsHitEventHitBy(CThing* thing) : HitBy(thing) { }
};
}
class CTCEntityEvents { public:
    bool GetEvent(NEntityEvents::CEventHitBy** event, const NEntityEvents::CIsHitEventHitBy& isHit);   // 0x007FCE10 (template instantiation)
};
struct SThingListNode {                         // STLport _List_node<CIntelligentPointer<CThing> >
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CIntelligentPointer _M_data;                // +0x08
};
struct CThingPointerList {                      // STLport list<CIntelligentPointer<CThing> > (the node pointer only)
    SThingListNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SThingListNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    void remove(const CIntelligentPointer& value);   // 0x0065DE8A
};

class CTCKraken { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x20 - 0x08];
    CBaseIntelligentPointer PTarget;            // +0x20
    unsigned char _pad28[0x30 - 0x28];
    CThingPointerList Tentacles;                // +0x30
    bool HasTargetBeenHitByKraken();
};

bool CTCKraken::HasTargetBeenHitByKraken()
{
    Tentacles.remove(CIntelligentPointer(0));
    if (Tentacles.size() > 0 && IS_THING_ALIVE(PTarget)) {
        CThing* target = PTarget.GetPItem();
        if (target->HasTC(0x42)) {
            CTCEntityEvents* events = (CTCEntityEvents*)target->m_tcs.Find(0x42);
            for (SThingListNode* it = Tentacles._M_node->_M_next; it != Tentacles._M_node; it = it->_M_next) {
                CThing* tentacle = it->_M_data.GetPItem();
                if (tentacle && !(tentacle->m_flags91 & 1)) {
                    NEntityEvents::CEventHitBy* event;
                    if (events->GetEvent(&event, NEntityEvents::CIsHitEventHitBy(tentacle)))
                        return true;
                }
            }
        }
    }
    return false;
}