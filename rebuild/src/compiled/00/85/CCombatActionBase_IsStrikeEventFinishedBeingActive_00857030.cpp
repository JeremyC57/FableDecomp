// CCombatActionBase::IsStrikeEventFinishedBeingActive() const -- true when there are strike events and none of them ends after the current running frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00857030, length 58 bytes (+ int3 padding).
// No callees. StrikeEvents is the retail std::list at +0xbc (head pointer, size at +0xc0) of counted
// CStrikeEvent pointers (node value at +8 is the object pointer); CStrikeEvent::EndFrame is +0x10 (Ego_r.pdb);
// FramesRunning is +0x1c (CCreatureActionBase).
// Shape: `if (size <= 0) return false;` then the inlined find_if loop (`mov edi,edi` is VC's loop-head
// alignment pad) from an inlined std::find_if template whose predicate copies FramesRunning (esi) before the
// begin==end test; the result `it == end()` re-reads the head from this (`cmp eax,[ecx+0BCh] ; sete al`)
// because the loop compared against the template's by-value `last` copy. A hand-written loop CSEs the two
// loads and frees ecx (5 bytes shorter). Local layout model
// (retail CCombatActionBase is 0x114 bytes, the PDB build's 0x12c).
struct CStrikeEvent {
    unsigned char _pad[0x10];
    long EndFrame;                              // +0x10
};

struct CStrikeEventNode {
    CStrikeEventNode* m_pNext;                  // +0x00
    CStrikeEventNode* m_pPrev;                  // +0x04
    CStrikeEvent* m_pEvent;                     // +0x08 (CCountedPointer<CStrikeEvent>)
};

struct CStrikeEventIterator {                   // std::list<...>::const_iterator
    CStrikeEventNode* m_pNode;
    CStrikeEventIterator(CStrikeEventNode* pNode) : m_pNode(pNode) { }
    bool operator!=(const CStrikeEventIterator& other) const { return m_pNode != other.m_pNode; }
    bool operator==(const CStrikeEventIterator& other) const { return m_pNode == other.m_pNode; }
    CStrikeEventIterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
    CStrikeEvent* operator*() const { return m_pNode->m_pEvent; }
};

struct CStrikeEventList {                       // retail std::list<CCountedPointer<CStrikeEvent>>
    CStrikeEventNode* m_pHead;                  // +0x00
    long m_size;                                // +0x04
    CStrikeEventIterator begin() const { return CStrikeEventIterator(m_pHead->m_pNext); }
    CStrikeEventIterator end() const { return CStrikeEventIterator(m_pHead); }
};

struct CIsStrikeEventActiveAfterFrame {         // find_if predicate: the event ends after the frame
    long m_frame;
    CIsStrikeEventActiveAfterFrame(long frame) : m_frame(frame) { }
    bool operator()(const CStrikeEvent* pEvent) const { return pEvent->EndFrame > m_frame; }
};

template<class Iterator, class Predicate>       // std::find_if, inlined: `last` is a by-value copy, so the
__forceinline Iterator find_if(Iterator first, Iterator last, Predicate pred)   // caller's end() is re-read after the loop
{
    for (; first != last; ++first) {
        if (pred(*first))
            break;
    }
    return first;
}

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[0x1c - 4];             // +0x04
    long FramesRunning;                         // +0x1c
    unsigned char _pad20[0xbc - 0x20];          // +0x20
    CStrikeEventList StrikeEvents;              // +0xbc
    unsigned char _padc4[0x114 - 0xc4];         // +0xc4
    bool IsStrikeEventFinishedBeingActive() const;
};

bool CCombatActionBase::IsStrikeEventFinishedBeingActive() const
{
    if (StrikeEvents.m_size <= 0)
        return false;
    CStrikeEventIterator it = find_if(StrikeEvents.begin(), StrikeEvents.end(), CIsStrikeEventActiveAfterFrame(FramesRunning));
    bool finished = (it == StrikeEvents.end());     // named bool: `sete al` without eax zeroing
    return finished;
}