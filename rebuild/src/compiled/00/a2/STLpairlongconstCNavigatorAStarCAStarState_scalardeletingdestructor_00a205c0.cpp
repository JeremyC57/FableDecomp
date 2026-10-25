// std::pair<const long, CAStarState>::`scalar deleting destructor'(unsigned int flags) -- the A* state map's value pair: the inline ~CAStarState counts the state out of the live-state counters, then the pair is freed for flag bit 0
// Leaf: ??_G?$pair@$$CBJVCAStarState@CNavigatorAStar@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A205C0, 57 bytes, thiscall `ret 4`: `dec [0x013D27B8]`, then `dec [0x013D27BC]` if
//   OnOpenList (+0x14) and `dec [0x013D27C0]` if OnClosedList (+0x15), then `if (flags & 1) operator delete(this)`
//   (0x00BFE9BC), returning this.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 456 "landed 0xA205C0 SCountedInstance has pair<const long,CAStarState>'s
//   layout"; hand456 README): the record is {long; CNavigatorAStar::CAStarState at +4} and its destructor is exactly the
//   ~CAStarState that the landed STLport map erase _Rb_tree<long,pair<const long,CAStarState>,...>::_M_erase 0x00A21640
//   inlines on the node's value pair (node +0x10: the same three statics, flags at node +0x24 / +0x25 = pair +0x14 / +0x15);
//   Ego_r.pdb CAStarState (0x14 bytes, OnOpenList +0x10, OnClosedList +0x11) and the CNavigatorAStar TU's
//   map<long, CAStarState> (Ego_r pair<const long, CAStarState> instantiations). No code references reach it (an unreferenced
//   COMDAT, like the pair ??_G 0x00CD22C0 that the map<long,CCharString> erase calls with 0). The landed name was descriptive.
// Namespace: std as in this TU's landed tree rows (0x00A21640 / 0x00A21D90).
// Model: the landed row's CAStarState; the counter names are descriptive (not in the PDB name sets).
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

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                     // +0x00
    _T2 second;                                    // +0x04
};
}

void Kill_00a205c0(std::pair<const long, CNavigatorAStar::CAStarState>* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
