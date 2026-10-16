// CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) -- true when the given idle state group is the preferred one; on a new frame an unset or just-matched preference is re-chosen first
// Leaf: IsPreferredIdle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00801BE0, 112 bytes; default /O2 TU, no frame, `ret 4`.
// Callees: CTCBase::GetWorld() @ 0x00686A70 (result unused), CWorld::GetWorldFrame() @ 0x0049D870 (static),
//          map<EIdleStateGroup, CAIStateGroupBase*>::operator[](const EIdleStateGroup&) @ 0x00802110 (twice;
//          manifest "BinaryTree_FindOrInsert"), ChooseNewIdle() @ 0x00801A50.
// Shape: group 1 short-circuits to `mov al,1`; on a frame other than FrameLastChecked an unset preference, or one
// that maps to this idle, is cleared and re-chosen (`Preferred == 0 || IdleStates[Preferred] == idle`); the result
// is an inline bool member `return Preferred != 0 && IdleStates[Preferred] == idle;`, whose && form gives the
// widened `mov eax,1` / `xor eax,eax` exits.
// The map key is passed by the member's address (edi = &Preferred).
// Layout: PDB CTCIdleScheduler with retail 12-byte maps: IdleStates +0x0C, IdleStatesWeight +0x18, Preferred +0x24,
// FrameLastChecked +0x28 (PDB +0x0C/+0x1C/+0x2C/+0x30).
struct CAIStateGroupBase;

struct CWorld {
    static long GetWorldFrame();                     // 0x0049D870
};

struct CIdleStateMap {                               // std::map<EIdleStateGroup, CAIStateGroupBase*> (retail 12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    CAIStateGroupBase*& operator[](const long& group);   // 0x00802110
};

struct CTCIdleScheduler {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    bool          ToBeRemoved;                       // +0x08
    bool          IsInGlobalUpdateTCs;               // +0x09
    unsigned char _pad_0x0a[0x2];
    CIdleStateMap IdleStates;                        // +0x0C
    CIdleStateMap IdleStatesWeight;                  // +0x18 (map<EIdleStateGroup, long>)
    long          Preferred;                         // +0x24 EIdleStateGroup
    long          FrameLastChecked;                  // +0x28

    CWorld* GetWorld() const;                        // 0x00686A70 (CTCBase)
    void ChooseNewIdle();                            // 0x00801A50
    bool IsPreferred(CAIStateGroupBase* idle) { return Preferred != 0 && IdleStates[Preferred] == idle; }
    bool IsPreferredIdle(CAIStateGroupBase* idle);
};

bool CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase* idle)
{
    if (Preferred == 1)
        return true;
    if (FrameLastChecked != GetWorld()->GetWorldFrame()) {
        if (Preferred == 0 || IdleStates[Preferred] == idle) {
            Preferred = 0;
            ChooseNewIdle();
        }
    }
    return IsPreferred(idle);
}