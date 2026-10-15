// CTCWillEnflame::CancelMove() -- if the owner has the type-0xA7 component and it is in state 5, clear it back to idle; then (unless that state check failed) mark the enflame move cancelled (state 3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C6A40, length 83 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inside CThing's inline GetInterface, the
// landed 0x007504E0 shape).
// Ego_r.pdb: ?CancelMove@CTCWillEnflame@@QAEXXZ. Layout: Ego_r.pdb CTCWillEnflame is 44 bytes (PTarget ends at +0x29);
// retail stores the move state at +0x20, so its intelligent pointers sit 4 bytes off the PDB: a local model with a
// descriptive name for that word. The component (interface type 0xA7) holds a flag byte at +0x0c and a state at +0x10.
// Shape: the presence bit (type 0xA7 = word 5 bit 7) folds to `mov cl,[eax+34h] ; test cl,cl ; jns`; the key is the
// `push ecx` frame slot; the component comes back through the out pointer (eax); a state other than 5 returns early,
// the two clears share one zero register and the `[edi+20h] = 3` store is the common tail.
struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template <class T> bool GetInterface(long type, T** out)   // inline: the landed 0x007504E0 shape
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

enum { TC_INTERFACE_ENFLAME_MOVE = 0xA7 };

struct CTCEnflameMoveComponent {            // the owner's type-0xA7 component
    unsigned char _pad[0x0c];
    bool          m_moving;                 // +0x0c
    long          m_state;                  // +0x10 (5 = moving)
};

struct CTCWillEnflame {
    void*         __vftable;                // +0x00
    CThing*       ParentThing;              // +0x04
    bool          ToBeRemoved;              // +0x08
    bool          IsInGlobalUpdateTCs;      // +0x09
    unsigned char _pad_0x0c[0x20 - 0x0c];
    long          m_moveState;              // +0x20 (3 = cancelled)
    void CancelMove();
};

void CTCWillEnflame::CancelMove()
{
    CTCEnflameMoveComponent* move;
    if (ParentThing->GetInterface(TC_INTERFACE_ENFLAME_MOVE, &move)) {
        if (move->m_state != 5)
            return;
        move->m_moving = false;
        move->m_state = 0;
    }
    m_moveState = 3;
}
