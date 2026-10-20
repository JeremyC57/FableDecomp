// Behaviour test for 0082a340: body copied from src/0082a340.cpp (CMiniMapDisplay::SetMarkerGraphic), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CThing { int id; };
struct CMarkerInfo {
    unsigned char PThing[8];
    long          GraphicIndex;
    bool          AutoMarker;
};
struct SMarkerNode {
    SMarkerNode* m_pNext;
    SMarkerNode* m_pPrev;
    CMarkerInfo  m_value;
};
struct CMarkerIterator {
    SMarkerNode* m_pNode;
    CMarkerIterator(SMarkerNode* node) : m_pNode(node) { }
    CMarkerIterator(const CMarkerIterator& other) : m_pNode(other.m_pNode) { }
    CMarkerInfo* operator->() const { return &m_pNode->m_value; }
    bool operator!=(const CMarkerIterator& other) const { return m_pNode != other.m_pNode; }
};
struct CMarkerList {
    SMarkerNode* m_pHead;
    CMarkerIterator begin() { return CMarkerIterator(m_pHead->m_pNext); }
    CMarkerIterator end() { return CMarkerIterator(m_pHead); }
};
struct CIsMarkerBoundToThing {
    const CThing* m_pThing;
    CIsMarkerBoundToThing(const CThing* thing) : m_pThing(thing) { }
};
struct bidirectional_iterator_tag_0082a340 { bidirectional_iterator_tag_0082a340() { } };
static int g_findCalls = 0; static SMarkerNode* g_findFirst = 0; static SMarkerNode* g_findLast = 0;
static const CThing* MarkerThing(const CMarkerInfo* marker) { return *(const CThing* const*)(marker->PThing + 4); }
CMarkerIterator __fastcall FindIf_0082c720(CMarkerIterator first, CMarkerIterator last, CIsMarkerBoundToThing pred, const bidirectional_iterator_tag_0082a340& tag)
{
    ++g_findCalls; g_findFirst = first.m_pNode; g_findLast = last.m_pNode;
    for (; first != last; first = CMarkerIterator(first.m_pNode->m_pNext))
        if (MarkerThing(first.operator->()) == pred.m_pThing)
            break;
    return first;
}
struct CMiniMapDisplay {
    virtual void v00() {}
    unsigned char _pad04[0x1c];
    CMarkerList   Markers;
    void SetMarkerGraphic(const CThing* thing, long graphicIndex);
};
void CMiniMapDisplay::SetMarkerGraphic(const CThing* thing, long graphicIndex)
{
    if (graphicIndex > 0) {
        CMarkerIterator last = Markers.end();
        CMarkerIterator first = Markers.begin();
        CMarkerIterator it = FindIf_0082c720(first, last, CIsMarkerBoundToThing(thing), bidirectional_iterator_tag_0082a340());
        if (it != Markers.end())
            it->GraphicIndex = graphicIndex;
    }
}
int main()
{
    static CThing a, b, c; static SMarkerNode head, n[2];
    head.m_pNext = &n[0]; n[0].m_pNext = &n[1]; n[1].m_pNext = &head;
    *(const CThing**)(n[0].m_value.PThing + 4) = &a; n[0].m_value.GraphicIndex = 1;
    *(const CThing**)(n[1].m_value.PThing + 4) = &b; n[1].m_value.GraphicIndex = 2;
    CMiniMapDisplay d; d.Markers.m_pHead = &head;
    // 1: the marker bound to b gets the new graphic
    d.SetMarkerGraphic(&b, 7); CHECK(n[1].m_value.GraphicIndex == 7 && n[0].m_value.GraphicIndex == 1); CHECK(g_findCalls == 1 && g_findFirst == &n[0] && g_findLast == &head);
    // 2: an unbound thing changes nothing
    d.SetMarkerGraphic(&c, 9); CHECK(n[0].m_value.GraphicIndex == 1 && n[1].m_value.GraphicIndex == 7); CHECK(g_findCalls == 2);
    // 3: a non-positive index never searches
    d.SetMarkerGraphic(&a, 0); d.SetMarkerGraphic(&a, -3); CHECK(g_findCalls == 2); CHECK(n[0].m_value.GraphicIndex == 1);
    d.SetMarkerGraphic(&a, 5); CHECK(n[0].m_value.GraphicIndex == 5);
    if (fails == 0) printf("0082a340_TEST PASS\n"); else printf("0082a340_TEST FAIL\n");
    return fails;
}