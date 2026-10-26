// CTCEntityEvents::HasEventOfType(NEntityEvents::EEventType type) const -- whether any event on the component's event list
// reports the given type (virtual slot 1)
// Leaf: ?HasEventOfType@CTCEntityEvents@@QBE_NW4EEventType@NEntityEvents@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811340, length 54 bytes.
// Identity (sweep3, FINDINGS chunk 481; was "CTCCreatureModeManager::IsModeActive", a second copy of the byte-identical
//   0x0073A8A0): retail has no identical-code folding, so the two bodies are two functions. Ego_r.pdb / FableWin.pdb
//   ?HasEventOfType@CTCEntityEvents@@QBE_NW4EEventType@NEntityEvents@@@Z has the same shape over Ego_r
//   CTCEntityEvents::Events (list<CCountedPointer<NEntityEvents::CEventBase>> +0x0C, the offset of the mode manager's
//   PModes too); the caller CThingCreatureBase::IsAbleToRecoverFromStunned 0x00664C60 fetches the thing's TC interface
//   0x42 (CTCEntityEvents in the landed component lookups; the mode manager is 0x31) and calls this with event type 0xD,
//   and the landed caller Talk 0x008643A0 already binds this exact PDB symbol here. 0x0073A8A0 stays IsModeActive.
// Callees: CEventBase's virtual type getter (slot 1, `call [eax+4]`).
// Shape: an inlined find over the list with the head (end) cached in edi for the loop, the type in
// ebx (pushed only on the non-empty path), and `found != Events.end()` re-reading the head after
// the loop (`mov ecx,[ebp+0Ch]`): the comparison is written against the member, not the cached
// end; the bool lands as `xor eax,eax ; setne al`.
namespace NEntityEvents {

enum EEventType { EVENT_TYPE_0 = 0 };

class CEventBase {
public:
    virtual void v00();
    virtual EEventType GetType() const;            // slot 1
};

}

struct SEventNode {                                // list node: next, prev, CCountedPointer<CEventBase>
    SEventNode*                m_pNext;            // +0x00
    SEventNode*                m_pPrev;            // +0x04
    NEntityEvents::CEventBase* m_pEvent;           // +0x08 (CCountedPointer::Data)
    void*                      m_pEventInfo;       // +0x0c (CCountedPointer::Info)
};

class CTCEntityEvents {
public:
    unsigned char _pad00[0xc];
    SEventNode*   Events;                          // +0x0c list head (the sentinel)
    bool HasEventOfType(NEntityEvents::EEventType type) const;
};

bool CTCEntityEvents::HasEventOfType(NEntityEvents::EEventType type) const
{
    SEventNode* end = Events;
    SEventNode* node = end->m_pNext;
    for (; node != end; node = node->m_pNext) {
        if (node->m_pEvent->GetType() == type)
            break;
    }
    return node != Events;
}
