// CCombatActionBase::IsStrikeEventActive() const -- an active strike event (the one first to finish) exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856FE0, length 67 bytes (+ int3 padding).
// Callees: std::for_each<list<CCountedPointer<CStrikeEvent>>::const_iterator, CGetActiveStrikeEventThatIsFirstToFinish>
// @ 0x0085A960 (__fastcall; the functor result through the hidden pointer in ecx, both iterators and the functor
// pushed; the functor temporary is built in registers (frame in eax, `xor edx,edx`, `mov esi,0F4240h`) and its
// three dwords are copied into the argument slot `sub esp,0Ch ; mov ebx,esp ; mov [ebx],eax ...` by the IMPLICIT
// copy constructor; the result is read through the returned pointer (`mov ...,[eax+4]`: an unnamed temporary). Same prologue as IsStrikeEvent
// @ 0x00856F80; StrikeEvents is the retail std::list at +0xbc, FramesRunning +0x1c (Ego_r.pdb names), retail
// CCombatActionBase sizeof 0x114 so the layout is modelled locally.
// Shape: `return pEvent != 0;` as `xor ecx,ecx ; test edx,edx ; setne cl ; mov al,cl`.
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
    bool IsStrikeEventActive() const;
};

bool CCombatActionBase::IsStrikeEventActive() const
{
    CStrikeEvent* pEvent = GetPActiveStrikeEvent();
    return pEvent != 0;
}