// CTCTargetingAI::FrameUpdate() -- prune the things-not-to-attack list (dead or vanished things, and stealthed things that have come within 5 units), then refresh the best enemy creature for attack and the nearest interesting thing
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F69B0, 306 bytes (plain `ret`). CTCTargetingAI vtable 0x012411FC slot 9 (the
//   manifest's "CTCTargetingAI::vfunc_9"; slot 9 is the CTCBase FrameUpdate).
// Callees: CTCBase::FrameUpdate @ 0x006868A0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the pointer's virtual
//   destructor (slot 0 with 0) and free @ 0x00BFEA14 (inline list erase), the const interface LowerBound @ 0x004365B0
//   (interface 0x36 = CTCStealth), CTCStealth::IsAvailableForAttack @ 0x00747B00, CThing::GetPosition @ 0x004C73D0 (twice),
//   CTCTargetingAI::UpdatePBestEnemyCreatureForAttack(bool) @ 0x007F6200, UpdateNearestInterestingThing (slot 39, +0x9C).
//   25.0f from 0x0123003C.
// Layout: Ego_r.pdb CTCTargetingAI ThingsToNotAttack (STLport list<CIntelligentPointer<CThing> >, the sentinel pointer) +0x48
//   (the landed AddThingToNotAttack 0x007F61B0 model); node {next, prev, data +8}; retail CThing interface bitset +0x20 (0x36
//   = word +0x24 bit 22), map +0x44, dead bit +0x91.
// Shape: an `if (!empty())` guard around the iterator loop; liveness through the inline bool IS_THING_ALIVE (nested ifs,
//   so the erase block follows the tests; an open-coded `!thing || dead` puts it after the stealth block); the inline STLport erase (unlink, virtual destructor, free)
//   returns the next node; the XY distance is `d.X * d.X + d.Y * d.Y` (VC emits the Y term first).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
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
struct CTCStealth {
    bool IsAvailableForAttack();                        // 0x00747B00
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                 // slot 0
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
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
struct CTCTargetingAI {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                         // slot 9
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void UpdateNearestInterestingThing();       // slot 39 (+0x9c)
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x48 - 0x08];
    CThingList    ThingsToNotAttack;                    // +0x48
    void BaseFrameUpdate();                             // 0x006868A0 (CTCBase::FrameUpdate)
    void UpdatePBestEnemyCreatureForAttack(bool force); // 0x007F6200
};

void CTCTargetingAI::FrameUpdate()
{
    BaseFrameUpdate();
    if (!ThingsToNotAttack.empty()) {
        for (SThingListNode* it = ThingsToNotAttack.begin(); it != ThingsToNotAttack.end(); ) {
            CThing* thing = it->_M_data.GetPItem();
            if (!IS_THING_ALIVE(thing)) {
                it = ThingsToNotAttack.erase(it);
                continue;
            }
            if (thing->HasTC(0x36) && thing->GetTC<CTCStealth>(0x36)->IsAvailableForAttack()) {
                C3DVector offset = thing->GetPosition() - ParentThing->GetPosition();
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
