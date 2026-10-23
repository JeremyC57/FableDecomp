// Grade: DIFFER(826v806) -- control flow, calls, frame (0x40) and stack slots match; register allocation differs: retail gives the hoisted zero to ebp (pushed late, after the early returns) and &OptimalBlockList to ebx, VC7.1 here gives the zero to ebx, so ebp is pushed in the prologue and the inner group loop is not strength-reduced to a pointer + count-down
// CVertexGroupBlockOptimiser::AddBlocks(unsigned long start) -- one step of the vertex-group block packing search: stop when over the iteration budget, greedy, or unable to beat the best list; record the current list as optimal when no block remains; otherwise seed an empty block with the largest unused group block, or try every remaining block that fits (sorted when greedy) and recurse, or open a new block and recurse
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD9F10, 0x324 bytes, `ret 4`.
// Callees: 0x00AD8FE0 / 0x00AD9D10 (lower-bound block counts, manifest VertexGroup*_CountComponents), FindLargestUnusedBlock
//   0x00AD94F0 (manifest VertexGroupOptimiser_BuildOptimisationTree), CanInsertMoreBlocks 0x00AD8DD0, InsertBlock 0x00AD92E0,
//   CVertexGroupBlock::RemoveLast 0x00AD9470 / ctor 0x00AD9030 / dtor 0x00A96DA0 (manifest FinishWithDynamicTextures),
//   the group map's find 0x006699C0, list<CVertexGroupBlock> size 0x00ADB570 (manifest SortTreeRecursively) / operator=
//   0x00ADD020 / push_back 0x00ADCE10 / pop_back 0x00ADC790, the candidate vector's reserve 0x00ADB960 / push_back 0x00ADBFB0,
//   sort 0x00ADC890 (__fastcall), free 0x00BFEA14.
// Layout (retail = Ego_r.pdb - 0x24; 12-byte vectors, 4-byte lists): InputList +0x00, OptimalBlockList +0x0c, Blocks +0x10,
//   CurrentBlock +0x14, RemainingBlocks +0x18, MaxN +0x44, MaxIterations +0x4c, Greedy +0x50, Iterations +0x54.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
template <class T> struct CArray {                      // retail vector: 12 bytes
    T* Begin;
    T* End;
    T* Last;
    unsigned long size() const { return End - Begin; }
    T& operator[](unsigned long i) { return Begin[i]; }
};
struct CGroupMap {                                      // map<unsigned long, long>
    struct node;
    struct iterator {
        node* Node;
        bool operator==(const iterator& other) const { return Node == other.Node; }
    };
    iterator end() { iterator it; it.Node = Header; return it; }
    node* Header;
    unsigned long Count;
    unsigned char Compare;
    iterator find(const unsigned long& key);            // 0x006699C0
};
struct CVertexGroupBlock {                              // 0x18 bytes
    CArray<unsigned long> Groups;                       // +0x00
    CGroupMap GroupToIndexMap;                          // +0x0c
    CVertexGroupBlock();                                // 0x00AD9030
    ~CVertexGroupBlock();                               // 0x00A96DA0
    void RemoveLast(unsigned long count);               // 0x00AD9470
    __forceinline bool HasGroup(unsigned long group)
    {
        CGroupMap::iterator it = GroupToIndexMap.find(group);
        return !(it == GroupToIndexMap.end());
    }
    __forceinline unsigned long CountNewGroups(const CVertexGroupBlock& other)
    {
        unsigned long count = other.Groups.size();
        unsigned long added = 0;
        for (unsigned long k = 0; k < count; ++k)
            if (!HasGroup(other.Groups.Begin[k]))
                ++added;
        return added;
    }
};
struct CBlockList {                                     // list<CVertexGroupBlock> (node pointer only)
    struct node { node* Next; node* Prev; CVertexGroupBlock Value; };
    node* Head;
    bool empty() const { return Head->Next == Head; }
    unsigned long size() const;                         // 0x00ADB570
    CBlockList& operator=(const CBlockList& other);     // 0x00ADD020
    void push_back(const CVertexGroupBlock& block);     // 0x00ADCE10
    void pop_back();                                    // 0x00ADC790
    CVertexGroupBlock& back() { return Head->Prev->Value; }
};
struct CCandidate {
    unsigned long Index;
    unsigned long NewGroups;
    unsigned long Groups;
};
struct CCandidateArray : CArray<CCandidate> {
    CCandidateArray() { Begin = 0; End = 0; Last = 0; }
    ~CCandidateArray() { if (Begin) free(Begin); }
    void reserve(unsigned long n);                      // 0x00ADB960
    void push_back(const CCandidate& c);                // 0x00ADBFB0
};
void __fastcall SortCandidates(CCandidate* first, CCandidate* last);   // 0x00ADC890

struct CVertexGroupBlockOptimiser {
    CArray<CVertexGroupBlock> InputList;                // +0x00
    CBlockList            OptimalBlockList;             // +0x0c
    CBlockList            Blocks;                       // +0x10
    CVertexGroupBlock*    CurrentBlock;                 // +0x14
    CArray<bool>          RemainingBlocks;              // +0x18
    unsigned char         _pad24[0x44 - 0x24];
    unsigned long         MaxN;                         // +0x44
    unsigned long         OptimalSize;                  // +0x48
    unsigned long         MaxIterations;                // +0x4c
    bool                  Greedy;                       // +0x50
    unsigned long         Iterations;                   // +0x54
    unsigned long MinimumBlocksForCurrent();            // 0x00AD8FE0
    unsigned long MinimumBlocksForRemaining();          // 0x00AD9D10
    unsigned long FindLargestUnusedBlock();             // 0x00AD94F0
    bool CanInsertMoreBlocks();                         // 0x00AD8DD0
    void InsertBlock(unsigned long index, unsigned long& added);   // 0x00AD92E0
    bool HasMoreBlocks()
    {
        unsigned long count = InputList.size();
        for (unsigned long i = 0; i < count; ++i)
            if (RemainingBlocks[i])
                return true;
        return false;
    }
    void AddBlocks(unsigned long start);
};

void CVertexGroupBlockOptimiser::AddBlocks(unsigned long start)
{
    ++Iterations;
    if (!OptimalBlockList.empty()) {
        if (Iterations > MaxIterations || Greedy)
            return;
        unsigned long optimal = 0;
        for (CBlockList::node* n = OptimalBlockList.Head->Next; n != OptimalBlockList.Head; n = n->Next)
            ++optimal;
        if (MinimumBlocksForCurrent() >= optimal)
            return;
    }
    if (!HasMoreBlocks()) {
        OptimalBlockList = Blocks;
        return;
    }
    unsigned long space = MaxN - CurrentBlock->Groups.size();
    if (CurrentBlock->Groups.size() == 0) {
        unsigned long largest = FindLargestUnusedBlock();
        InsertBlock(largest, start);
        AddBlocks(0);
        RemainingBlocks[largest] = true;
        CurrentBlock->RemoveLast(start);
        return;
    }
    CCandidateArray candidates;
    candidates.reserve(InputList.size());
    for (unsigned long j = start; j < InputList.size(); ++j) {
        if (!RemainingBlocks[j])
            continue;
        CVertexGroupBlock& block = InputList[j];
        const unsigned long* groups = block.Groups.Begin;
        unsigned long count = block.Groups.size();
        start = 0;
        CVertexGroupBlock* current = CurrentBlock;
        for (unsigned long k = 0; k < count; ++k)
            if (!current->HasGroup(groups[k]))
                ++start;
        if (start <= space) {
            CCandidate c;
            c.Index = j;
            c.NewGroups = start;
            c.Groups = count;
            candidates.push_back(c);
        }
    }
    CCandidate* first = candidates.Begin;
    CCandidate* last = candidates.End;
    if (Greedy)
        SortCandidates(first, last);
    if (first == last) {
        if (!CanInsertMoreBlocks()) {
            if (!OptimalBlockList.empty()) {
                if (Blocks.size() >= OptimalBlockList.size())
                    return;
                if (MinimumBlocksForRemaining() >= OptimalBlockList.size())
                    return;
            }
            CVertexGroupBlock* saved = CurrentBlock;
            Blocks.push_back(CVertexGroupBlock());
            CurrentBlock = &Blocks.back();
            AddBlocks(0);
            Blocks.pop_back();
            CurrentBlock = saved;
        }
    } else {
        unsigned long n = last - first;
        for (unsigned long c = 0; c < n; ++c) {
            InsertBlock(first[c].Index, start);
            unsigned long next = 0;
            if (!Greedy)
                next = first[c].Index + 1;
            AddBlocks(next);
            RemainingBlocks[first[c].Index] = true;
            CurrentBlock->RemoveLast(start);
        }
    }
}
