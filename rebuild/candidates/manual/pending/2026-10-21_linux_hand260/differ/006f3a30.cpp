// Grade: DIFFER(496v481) -- STLport list sort: retail keeps carry.Node in ebx with frame 0x10c
// SMarkerList::sort(CThingCompare_Nearest comp) -- STLport list<CThingMarker*>::sort with the inlined _S_sort: a list of two or more nodes is merge-sorted through a carry list and 64 counter lists (nearest marker first), the result swapped back in and the temporaries destroyed (nodes freed)
// Leaf: sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F3A30, 482 bytes (496 with padding), `ret 0Ch` (the 12-byte comparator by value).
// Label: the manifest's "CBinaryTree_Alt::Rebuild" is a placeholder; the body is STLport _S_sort over the list type
//   of the landed _S_merge (0x006F04C0, its callee here: __fastcall, the two lists in ecx/edx, the comparator copied
//   to the stack for each call).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the list nodes), `vector constructor iterator` 0x004038C0 with the list
//   default-constructor closure 0x006F2BB0 (the landed CRetailList_node0xc::Initialise: the allocator default argument
//   makes it a ??_F closure) for the 64 counters.
// Shape: STLport 4.x _S_sort verbatim (splice of the first node into carry, the merge/swap ladder, the final merges
//   and swap); list::splice / swap / _M_transfer inline; the counter array and the carry destroyed inline.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);               // 0x00BFEA14

struct CThingMarker;
struct SListNode {
    SListNode*    Next;                       // +0x00
    SListNode*    Prev;                       // +0x04
    CThingMarker* Value;                      // +0x08
};
struct CThingCompare_Nearest {                // 12 bytes: the reference position
    float X, Y, Z;
    long operator()(CThingMarker* a, CThingMarker* b);   // 0x006EF290
};
struct SMarkerAllocator { SMarkerAllocator() {} };

inline void _M_transfer(SListNode* position, SListNode* first, SListNode* last)
{
    if (position != last) {
        last->Prev->Next = position;
        first->Prev->Next = last;
        position->Prev->Next = first;
        SListNode* tmp = position->Prev;
        position->Prev = last->Prev;
        last->Prev = first->Prev;
        first->Prev = tmp;
    }
}

struct SMarkerList {                          // STLport list<CThingMarker*>: the header node pointer
    SListNode* Node;
    SMarkerList(const SMarkerAllocator& = SMarkerAllocator())
    {
        Node = 0;
        SListNode* header = (SListNode*)malloc(sizeof(SListNode));
        header->Next = header;
        header->Prev = header;
        Node = header;
    }
    ~SMarkerList()
    {
        clear();
        if (Node)
            free(Node);
    }
    void clear()
    {
        SListNode* cur = Node->Next;
        while (cur != Node) {
            SListNode* tmp = cur;
            cur = cur->Next;
            free(tmp);
        }
        Node->Next = Node;
        Node->Prev = Node;
    }
    bool empty() const { return Node->Next == Node; }
    void swap(SMarkerList& other) { SListNode* tmp = Node; Node = other.Node; other.Node = tmp; }
    void splice(SListNode* position, SMarkerList&, SListNode* i)
    {
        SListNode* j = i->Next;
        if (position == i || position == j)
            return;
        _M_transfer(position, i, j);
    }
    void sort(CThingCompare_Nearest comp);
};

void __fastcall _S_merge(SMarkerList& list, SMarkerList& other, CThingCompare_Nearest comp);   // 0x006F04C0

void SMarkerList::sort(CThingCompare_Nearest comp)
{
    if (Node->Next != Node && Node->Next->Next != Node) {
        SMarkerList carry;
        SMarkerList counter[64];
        int fill = 0;
        while (!empty()) {
            carry.splice(carry.Node->Next, *this, Node->Next);
            int i = 0;
            while (i < fill && !counter[i].empty()) {
                _S_merge(counter[i], carry, comp);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill)
                ++fill;
        }
        for (int i = 1; i < fill; ++i)
            _S_merge(counter[i], counter[i - 1], comp);
        swap(counter[fill - 1]);
    }
}
