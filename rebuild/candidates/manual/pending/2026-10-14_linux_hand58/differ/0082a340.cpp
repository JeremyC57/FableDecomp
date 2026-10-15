// CMiniMapDisplay::SetMarkerGraphic(const CThing* thing, long graphicIndex) -- for a positive index, find the marker bound to the thing in the marker list (+0x20; SGI __find_if @ 0x0082C720 with the CIsMarkerBoundToThing predicate) and set its GraphicIndex (+8 of the marker)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A340, length 62 bytes.
// Callees: __find_if @ 0x0082C720 (`__fastcall`: the hidden iterator result in ecx, the iterator-category tag by const reference in edx, first / last / predicate on the stack; the two iterators have a user copy constructor and are built in their slots, the predicate is a plain word). The result temporary and the empty tag share the dead `graphicIndex` slot (the index lives in ebx). The retail list sentinel is at +0x20 (engine/CMiniMapDisplay.h places Markers at +0x28 behind a 0x10-byte MarkerPrimitiveHandles that retail does not have at that offset), so the layout is local. Shape: `this` in esi, the thing in edi, `ret 8`.
// Grade: DIFFER(ours v retail) -- only the hidden-result slot of __find_if: retail homes the iterator result in the dead `graphicIndex` slot (`lea ecx,[esp+20h]`, shared with the tag), the lane in the dead `thing` slot (`lea ecx,[esp+1Ch]`); bare begin()/end() arguments, named iterator locals, a by-reference predicate constructor and a predicate with a user destructor tried.
struct CThing;
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
struct CMarkerIterator {                           // list<CMarkerInfo>::iterator: one node pointer with a user copy constructor
    SMarkerNode* m_pNode;
    CMarkerIterator(SMarkerNode* node) : m_pNode(node) { }
    CMarkerIterator(const CMarkerIterator& other) : m_pNode(other.m_pNode) { }
    CMarkerInfo* operator->() const { return &m_pNode->m_value; }
    bool operator!=(const CMarkerIterator& other) const { return m_pNode != other.m_pNode; }
};
struct CMarkerList {                               // list<CMarkerInfo>: one sentinel pointer
    SMarkerNode* m_pHead;
    CMarkerIterator begin() { return CMarkerIterator(m_pHead->m_pNext); }
    CMarkerIterator end() { return CMarkerIterator(m_pHead); }
};
struct CIsMarkerBoundToThing {                     // the predicate: the thing to match (no user copy constructor: pushed as a word)
    const CThing* m_pThing;
    CIsMarkerBoundToThing(const CThing* thing) : m_pThing(thing) { }
};
struct bidirectional_iterator_tag_0082a340 { bidirectional_iterator_tag_0082a340() { } };

CMarkerIterator __fastcall FindIf_0082c720(CMarkerIterator first, CMarkerIterator last, CIsMarkerBoundToThing pred, const bidirectional_iterator_tag_0082a340& tag);   // 0x0082C720

struct CMiniMapDisplay {
    virtual void v00();
    unsigned char _pad04[0x1c];                    // +0x04 .. +0x20
    CMarkerList   Markers;                         // +0x20 (retail)
    void SetMarkerGraphic(const CThing* thing, long graphicIndex);
};

void CMiniMapDisplay::SetMarkerGraphic(const CThing* thing, long graphicIndex)
{
    if (graphicIndex > 0) {
        CMarkerIterator last = Markers.end();      // named iterators: both node loads precede the argument pushes
        CMarkerIterator first = Markers.begin();
        CMarkerIterator it = FindIf_0082c720(first, last, CIsMarkerBoundToThing(thing), bidirectional_iterator_tag_0082a340());
        if (it != Markers.end())
            it->GraphicIndex = graphicIndex;
    }
}
