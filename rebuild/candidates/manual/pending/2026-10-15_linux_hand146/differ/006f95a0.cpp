// CManager::GetExtraEventsBetweenTimes(long animation, CEventArray& events, float start, float end) -- append to `events` (through a for_each collector) every event of the animation's event list that falls between two times; true when anything was added
// Grade: DIFFER(138v151) -- (1) retail tests the list pointer in place (`add eax,4 ; je`) where ours forms it in ecx (`lea ecx,[eax+4]`, count read into eax); (2) retail builds the collector's two time words in a frame local ([esp+14h]/[esp+18h], reloaded for the by-value copy, and that local's slot is the for_each hidden result) where ours forwards the arguments straight into the argument area and has a 13-byte smaller body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F95A0, length 151 bytes.
// Callees: the map's LowerBound(const unsigned long&) @ 0x006FAF80, for_each<CEvent32*, collector> @ 0x006FB640 (`__fastcall`: hidden
//   result in ecx, first in edx, last and the 12-byte collector by value on the stack).
// Layout: NAnimationEvents::CManager map at +0x2c (as in 0x006F9430); the output array's count at +4 (the `setg` compare is on that signed count).
// Tried: a temporary collector argument, a named collector local, a time-range sub-object local + ternary accessor, `collector = for_each(...)`
//   (all four compile to the same forwarded copy). next: the time range as a by-value struct parameter, or the list accessor returning a reference.
struct CEvent32 {
    unsigned long Type    : 30;
    unsigned long Flag30  : 1;
    unsigned long Flag31  : 1;
    float         Time;
};
struct CEventArray {
    CEvent32* m_pData;
    long      m_count;
};
struct CEventList {
    CEvent32*     m_pData;
    unsigned long m_count;
    CEvent32* begin() const { return m_pData; }
    CEvent32* end() const { return m_pData + m_count; }
};
struct SEntry {
    unsigned long first;
    CEventList    second;
};
struct CEntryMap {
    SEntry* m_pFirst;
    SEntry* m_pLast;
    SEntry* m_pEnd;
    SEntry* LowerBound(const unsigned long& key);   // 0x006FAF80
    SEntry* End() { return m_pLast; }
    SEntry* Find(long key)
    {
        SEntry* it = LowerBound((unsigned long)key);
        if (it == End() || (unsigned long)key < it->first)
            it = End();
        return it;
    }
};
struct CTimeRange {
    float m_start;
    float m_end;
    CTimeRange(float start, float end) : m_start(start), m_end(end) { }
    CTimeRange(const CTimeRange& o) : m_start(o.m_start), m_end(o.m_end) { }
};
struct CEventCollector {
    CTimeRange   m_range;
    CEventArray* m_pEvents;
    CEventCollector(const CTimeRange& range, CEventArray* events) : m_range(range), m_pEvents(events) { }
    CEventCollector(const CEventCollector& o) : m_range(o.m_range), m_pEvents(o.m_pEvents) { }
};
CEventCollector __fastcall for_each(CEvent32* first, CEvent32* last, CEventCollector f);   // 0x006FB640
struct CManager {
    unsigned char _pad00[0x2c];
    CEntryMap     m_animEvents;                    // +0x2c
    __forceinline CEventList* GetAnimEvents(long animation)
    {
        SEntry* it = m_animEvents.Find(animation);
        return it == m_animEvents.End() ? 0 : &it->second;
    }
    bool GetExtraEventsBetweenTimes(long animation, CEventArray& events, float start, float end);
};
bool CManager::GetExtraEventsBetweenTimes(long animation, CEventArray& events, float start, float end)
{
    long oldCount = events.m_count;
    CEventList* list = GetAnimEvents(animation);
    if (list && list->m_count) {
        CTimeRange range(start, end);
        for_each(list->begin(), list->end(), CEventCollector(range, &events));
    }
    return events.m_count > oldCount;
}
