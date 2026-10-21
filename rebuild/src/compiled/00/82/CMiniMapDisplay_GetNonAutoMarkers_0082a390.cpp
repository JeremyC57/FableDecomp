// CMiniMapDisplay::GetNonAutoMarkers(CMarkerPtrList& found) -- append a pointer to every marker of the marker list (+0x20) whose AutoMarker byte (+0xc) is clear to the output pointer list (an STLport list push_back inlined: malloc @ 0x00BFEA0E for the 12-byte node, placement construction of the pointer, the node linked before the sentinel)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A390, length 75 bytes.
// Callees: malloc @ 0x00BFEA0E (the STLport malloc allocator; `add esp,4`). The retail list sentinel is at +0x20 (engine/CMiniMapDisplay.h places Markers at +0x28), so the layout is local. Shape: `this` in ebp, the node in edi, the output list in ebx and its sentinel in esi (read before the allocation: the insert position is the end() iterator built first), the placement-new null test, `ret 4`.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CMarkerInfo {                               // PDB: CIntelligentPointer<CThing const> PThing +0 (8 bytes), long GraphicIndex +8, bool AutoMarker +0xc
    unsigned char PThing[8];
    long          GraphicIndex;                    // +0x08
    bool          AutoMarker;                      // +0x0c
};
struct SMarkerNode {
    SMarkerNode* m_pNext;
    SMarkerNode* m_pPrev;
    CMarkerInfo  m_value;                          // +0x08
};
struct CMarkerList {                               // list<CMarkerInfo>: one sentinel pointer
    SMarkerNode* m_pHead;
};

struct SMarkerPtrNode {
    SMarkerPtrNode* m_pNext;
    SMarkerPtrNode* m_pPrev;
    CMarkerInfo*    m_value;                       // +0x08
};
struct CMarkerPtrList {                            // list<CMarkerInfo*> (STLport: sentinel pointer, malloc node allocator)
    struct iterator {
        SMarkerPtrNode* m_node;
        iterator(SMarkerPtrNode* n) : m_node(n) {}
        iterator(const iterator& o) : m_node(o.m_node) {}
    };
    SMarkerPtrNode* m_pHead;
    iterator end() { return iterator(m_pHead); }
    SMarkerPtrNode* CreateNode(CMarkerInfo* const& value)
    {
        SMarkerPtrNode* node = (SMarkerPtrNode*)malloc(sizeof(SMarkerPtrNode));
        new (&node->m_value) CMarkerInfo*(value);
        return node;
    }
    void Insert(iterator position, CMarkerInfo* const& value)
    {
        SMarkerPtrNode* node = CreateNode(value);
        SMarkerPtrNode* n = position.m_node;
        SMarkerPtrNode* prev = n->m_pPrev;
        node->m_pNext = n;
        node->m_pPrev = prev;
        prev->m_pNext = node;
        n->m_pPrev = node;
    }
    void push_back(CMarkerInfo* const& value) { Insert(end(), value); }
};

struct CMiniMapDisplay {
    virtual void v00();
    unsigned char _pad04[0x1c];                    // +0x04 .. +0x20
    CMarkerList   Markers;                         // +0x20 (retail)
    void GetNonAutoMarkers(CMarkerPtrList& found);
};

void CMiniMapDisplay::GetNonAutoMarkers(CMarkerPtrList& found)
{
    for (SMarkerNode* node = Markers.m_pHead->m_pNext; node != Markers.m_pHead; node = node->m_pNext) {
        if (!node->m_value.AutoMarker)
            found.push_back(&node->m_value);
    }
}