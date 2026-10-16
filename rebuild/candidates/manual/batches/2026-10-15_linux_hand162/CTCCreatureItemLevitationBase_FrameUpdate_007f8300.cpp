// CTCCreatureItemLevitationBase::FrameUpdate() -- refresh the levitation target, then pin every levitating item at the creature's position plus the levitation offset (drawable, physics awake and moving)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8300, length 135 bytes (+ int3 padding). Vtable 0x0124156C slot 9.
// Callees: own virtual slot 31 (+0x7C; 0x007F84C0 in CTCCreatureItemLevitationNymph, which recomputes its hover direction),
// CThing::GetPosition @ 0x004C73D0, CThing::SetAsDrawable(bool) (slot 28, +0x70), CTCPhysicsBase slot 36 (+0x90, bool; an empty
// `ret 4` in the base), CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130, CTCPhysicsBase::SetPosition(const C3DVector&) (slot 30, +0x78).
// Globals: the levitation offset C3DVector @ 0x01384494 (x, y, z).
// Layout: Ego_r.pdb CTCCreatureItemLevitationBase: LevitatingItems +0xC (retail single-pointer STLport list<CThing*>,
// node {next, prev, value}), NumItems +0x10, LastAddFrame +0x14 (PDB +0x18/+0x1C). Retail CThing keeps its physics
// component pointer at +0x60.
// Shape: the list end is re-read from the member each pass (the calls may alias it); the position sum is built X first
// (`fld g ; fadd [pos]`) component by component into the named local.
struct C3DVector {
    float x, y, z;
};
extern C3DVector g_levitationOffset;            // 0x01384494

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetSlot36(bool on);            // slot 36 (+0x90; empty in CTCPhysicsBase)
    void SetAsMoving(bool moving);              // 0x006B0130
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetAsDrawable(bool drawable);  // slot 28 (+0x70)
    unsigned char _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct SThingListNode { SThingListNode* next; SThingListNode* prev; CThing* value; };
struct CThingList {                             // list<CThing*> (retail: one sentinel pointer)
    SThingListNode* head;
};
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                 // slot 9
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
struct CTCCreatureItemLevitationBase : CTCBase {
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30();
    virtual void UpdateLevitationTarget();      // slot 31 (+0x7c)
    CThingList LevitatingItems;                 // +0x0c
    long NumItems;                              // +0x10
    long LastAddFrame;                          // +0x14
    virtual void FrameUpdate();
};

void CTCCreatureItemLevitationBase::FrameUpdate()
{
    UpdateLevitationTarget();
    const C3DVector& position = ParentThing->GetPosition();
    for (SThingListNode* node = LevitatingItems.head->next; node != LevitatingItems.head; node = node->next) {
        CThing* item = node->value;
        C3DVector itemPosition;
        itemPosition.x = g_levitationOffset.x + position.x;
        itemPosition.y = g_levitationOffset.y + position.y;
        itemPosition.z = g_levitationOffset.z + position.z;
        item->SetAsDrawable(true);
        item->PhysicsTC->SetSlot36(true);
        item->PhysicsTC->SetAsMoving(true);
        item->PhysicsTC->SetPosition(itemPosition);
    }
}
