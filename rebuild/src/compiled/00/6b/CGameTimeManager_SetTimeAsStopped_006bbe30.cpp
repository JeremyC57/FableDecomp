// CGameTimeManager::SetTimeAsStopped(bool stopped, long* handle) -- stopping hands out the next stopper handle (+0x38, post-incremented) through the pointer and inserts it into the stopper set at +0x2c (0x00512DA0 with an iterator slot); un-stopping removes the handle (0x00639590)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BBE30, length 66 bytes.
// Callees: the set insert @ 0x00512DA0 (on +0x2c: an 8-byte iterator out-slot by address, the handle by address; labelled "BinarySearchTree_Insert") and the
// erase @ 0x00639590 (the handle by address; labelled "Crime_RemoveEntry", propagated). Shape: the flag byte read before the 8-byte frame, `ret 8`.
struct SSetIterator_006bbe30 { long m_a, m_b; }; // 8 bytes

struct CStopperSet_006bbe30 {                      // a tree of handles at +0x2c (12 bytes)
    unsigned char _body[12];
    SSetIterator_006bbe30* Insert(SSetIterator_006bbe30* result, const long* handle);   // 0x00512DA0
    void Erase(const long* handle);                                                      // 0x00639590
};

class CGameTimeManager { public:
    unsigned char          _pad[0x2c];
    CStopperSet_006bbe30   m_stoppers;             // +0x2c
    long                   m_nextStopperHandle;    // +0x38
    void SetTimeAsStopped(bool stopped, long* handle);
};

void CGameTimeManager::SetTimeAsStopped(bool stopped, long* handle)
{
    if (stopped) {
        *handle = m_nextStopperHandle++;
        SSetIterator_006bbe30 where;
        m_stoppers.Insert(&where, handle);
    } else {
        m_stoppers.Erase(handle);
    }
}