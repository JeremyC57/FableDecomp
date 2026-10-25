// SCountedInstance_00a205c0::`scalar deleting destructor'(unsigned int flags) -- destroy a record holding a
// CNavigatorAStar::CAStarState at +4 (the layout of the A* state map's pair<const long, CAStarState>): the inline
// ~CAStarState counts the state out of CAStarState::NoOfStates (0x013D27B8) and, for its OnOpenList / OnClosedList flags
// (+0x14 / +0x15), out of NoOfStatesOnOpenList (0x013D27BC) / NoOfStatesOnClosedList (0x013D27C0); then frees the record
// for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A205C0, length 57 bytes.
// Leaf: ??_GSCountedInstance_00a205c0@@QAEPAXI@Z
// Evidence: Ego_r.pdb CAStarState layout (OnOpenList +0x10, OnClosedList +0x11, 0x14 bytes); the STLport map _M_erase
//   0x00A21640 inlines the same ~CAStarState on the same three statics at node +0x24 / +0x25 (value pair at +0x10).
//   The counters' names are not in the PDB name sets: NoOfStates / NoOfStatesOnOpenList / NoOfStatesOnClosedList are
//   descriptive (chosen to match the chunk 416 sources). No xrefs reach this function.
// The chunk label is propagated; named by effect. Shape: `dec dword ptr [global]` three times, the flag bytes in al, `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

class CANavQuadTreeNode;

class CNavigatorAStar {
public:
    class CAStarState {                            // 0x14 bytes (Ego_r.pdb)
    public:
        CAStarState*       ParentState;            // +0x00
        CANavQuadTreeNode* Node;                   // +0x04
        float              CostToThisState;        // +0x08
        float              TotalCost;              // +0x0C
        bool               OnOpenList;             // +0x10
        bool               OnClosedList;           // +0x11
        static long NoOfStates;                    // 0x013D27B8 (descriptive)
        static long NoOfStatesOnOpenList;          // 0x013D27BC (descriptive)
        static long NoOfStatesOnClosedList;        // 0x013D27C0 (descriptive)
        ~CAStarState()
        {
            --NoOfStates;
            if (OnOpenList)
                --NoOfStatesOnOpenList;
            if (OnClosedList)
                --NoOfStatesOnClosedList;
        }
    };
};

struct SCountedInstance_00a205c0 {                 // pair<const long, CNavigatorAStar::CAStarState> layout
    long                         first;            // +0x00
    CNavigatorAStar::CAStarState second;           // +0x04
    ~SCountedInstance_00a205c0() { }
};

void Kill_00a205c0(SCountedInstance_00a205c0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
