// CManager::EventExists(long animation, const CEvent32& event) -- whether the animation's event list (the sorted animation map at +0x2c) holds an event of the same type and top flag at (within 0.0001 of) the same time
// Grade: DIFFER(146v146) -- one register copy: ours materialises the list's end as `mov eax,ecx ; lea edi,[eax+edx*8]` (the begin copied before the add), retail `lea edi,[ecx+edx*8]`; the extra byte changes the loop-alignment filler (`lea esp,[esp]` v `lea ebx,[ebx]`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F9430, length 146 bytes.
// Callees: the map's LowerBound(const unsigned long&) @ 0x006FAF80 (as in the landed 0x006F9560); float 0x0129BA3C = 0.0001f.
// Layout: NAnimationEvents::CManager map at +0x2c (CVectorMap<unsigned long, CSmallVector<CEvent32>>: first/last/end, 12-byte entries
//   {key, data pointer, count}); CEvent32 = 30-bit type + two flag bits, then the float time.
// Shape: the inline Find takes the key BY VALUE as a long and converts it for LowerBound (`(unsigned long)key` temporary homed in the
//   argument slot) while the compare keeps the key in ebx; the type compare is the masked 30-bit field, the bit-31 flag compare is
//   `xor ; js`, the time test `fabs(a - b) < 0.0001f`.
// Tried: the end() call in the loop condition (recomputed), a hoisted end local after or before the begin (both copy the begin first).
#include <math.h>
struct CEvent32 {
    unsigned long Type    : 30;
    unsigned long Flag30  : 1;
    unsigned long Flag31  : 1;
    float         Time;
};
struct CEventList {                                // CSmallVector<CEvent32>: data pointer + count
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
struct CManager {
    unsigned char _pad00[0x2c];
    CEntryMap     m_animEvents;                    // +0x2c
    bool EventExists(long animation, const CEvent32& event);
};
bool CManager::EventExists(long animation, const CEvent32& event)
{
    SEntry* it = m_animEvents.Find(animation);
    if (it != m_animEvents.End()) {
        CEvent32* e = it->second.begin();
        CEvent32* end = it->second.end();
        for (; e != end; ++e) {
            if (e->Type == event.Type && e->Flag31 == event.Flag31 && fabs(e->Time - event.Time) < 0.0001f)
                return true;
        }
    }
    return false;
}
