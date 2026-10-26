// CCombatActionBase::IsStrikeEvent(long index) const -- the active strike event that is first to finish exists and has this StrikeEventIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856F80, length 86 bytes (+ int3 padding).
// Callees: std::for_each<list<CCountedPointer<CStrikeEvent>>::const_iterator, CGetActiveStrikeEventThatIsFirstToFinish>
// @ 0x0085A960 (__fastcall; the functor result through the hidden pointer in ecx, both iterators and the functor
// pushed; the functor temporary is built in registers (frame in eax, `xor edx,edx`, `mov esi,0F4240h`) and its
// three dwords are copied into the argument slot `sub esp,0Ch ; mov ebx,esp ; mov [ebx],eax ...` by the IMPLICIT
// copy constructor; the result is read through the returned pointer (`mov ...,[eax+4]`: an unnamed temporary).
// StrikeEvents is the retail std::list at +0xbc (head pointer first, size +0xc0); FramesRunning is +0x1c;
// CStrikeEvent::StrikeEventIndex is +8 (Ego_r.pdb names). The functor is the anonymous-namespace class of
// the manifest (`_anon_AD05D24C::CGetActiveStrikeEventThatIsFirstToFinish`): current frame, found event, best end.
// Shape: argument order functor, end (`push ecx` = head), begin (`push edi` = head->next); then `mov eax,[eax+4]`
// off the returned result pointer, nested ifs, `mov al,1` / `xor al,al`. Retail base sizeof 0x114: local model.
struct CStrikeEvent {
    bool Legacy;                                // +0x00
    bool PrimaryLegacy;                         // +0x01
    bool Weapon;                                // +0x02
    void* HelperPointName;                      // +0x04 (CCharString)
    long StrikeEventIndex;                      // +0x08
    long StartFrame;                            // +0x0c
    long EndFrame;                              // +0x10
};

struct CStrikeEventNode {
    CStrikeEventNode* m_pNext;                  // +0x00
    CStrikeEventNode* m_pPrev;                  // +0x04
    CStrikeEvent* m_pEvent;                     // +0x08 (CCountedPointer<CStrikeEvent>)
};

struct CStrikeEventIterator {                   // std::list<...>::const_iterator (4 bytes, has a constructor)
    CStrikeEventNode* m_pNode;
    CStrikeEventIterator(CStrikeEventNode* pNode) : m_pNode(pNode) { }
};

struct CStrikeEventList {                       // retail std::list<CCountedPointer<CStrikeEvent>>
    CStrikeEventNode* m_pHead;                  // +0x00
    long m_size;                                // +0x04
    CStrikeEventIterator begin() const { return CStrikeEventIterator(m_pHead->m_pNext); }
    CStrikeEventIterator end() const { return CStrikeEventIterator(m_pHead); }
};

struct CGetActiveStrikeEventThatIsFirstToFinish {
    long m_currentFrame;                        // +0x00
    CStrikeEvent* m_pFirstToFinish;             // +0x04
    long m_earliestEndFrame;                    // +0x08
    CGetActiveStrikeEventThatIsFirstToFinish(long currentFrame)
        : m_currentFrame(currentFrame), m_pFirstToFinish(0), m_earliestEndFrame(1000000) { }
    // IMPLICIT copy constructor: VC builds the temporary in registers and copies its three dwords into the
    // argument slot (`xor edx,edx` / `mov esi,0F4240h`); a user-declared copy constructor makes it construct
    // the argument in place with immediate stores (5 bytes shorter).
};

CGetActiveStrikeEventThatIsFirstToFinish __fastcall for_each(CStrikeEventIterator first, CStrikeEventIterator last,
                                                             CGetActiveStrikeEventThatIsFirstToFinish functor);   // 0x0085A960

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[0x1c - 4];             // +0x04
    long FramesRunning;                         // +0x1c
    unsigned char _pad20[0xbc - 0x20];          // +0x20
    CStrikeEventList StrikeEvents;              // +0xbc
    unsigned char _padc4[0x114 - 0xc4];         // +0xc4
    __forceinline CStrikeEvent* GetPActiveStrikeEvent() const     // inlined helper: the for_each result is a temporary read through its returned pointer
    {
        return for_each(StrikeEvents.begin(), StrikeEvents.end(),
                        CGetActiveStrikeEventThatIsFirstToFinish(FramesRunning)).m_pFirstToFinish;
    }
    bool IsStrikeEvent(long index) const;
};

bool CCombatActionBase::IsStrikeEvent(long index) const
{
    CStrikeEvent* pEvent = GetPActiveStrikeEvent();
    if (pEvent != 0) {
        if (pEvent->StrikeEventIndex == index)
            return true;
    }
    return false;
}