// CCombatActionBase::GetStrikeEventHelperName() const -- the helper-point name of the active strike event first to finish, or an empty string when there is none or it is a legacy event
// Leaf: GetStrikeEventHelperName
// Retail: Fable.exe (TLC, VC7.1) @ 0x008570D0, 115 bytes of code (CCharString returned through the hidden pointer, `ret 4`).
// Callees: std::for_each<list<CCountedPointer<CStrikeEvent>>::const_iterator, CGetActiveStrikeEventThatIsFirstToFinish>
//          @ 0x0085A960 (__fastcall, functor result through the hidden pointer in ecx), CCharString::CCharString(const
//          CCharString&) @ 0x0099EC30, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the "" literal @ 0x0122D70E).
// Shape: the prologue of the landed IsActionWithStrikeEventHelperName @ 0x00857150 (same inline GetPActiveStrikeEvent
//          helper and models); each return copy-constructs the hidden result in place, so both paths end with their own
//          epilogue. HelperPointName is the CCharString at +4 of the event (Ego_r.pdb).
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

struct CStrikeEvent {
    bool Legacy;                                // +0x00
    bool PrimaryLegacy;                         // +0x01
    bool Weapon;                                // +0x02
    CCharString HelperPointName;                // +0x04
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

struct CCombatActionBase {
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
    CCharString GetStrikeEventHelperName() const;
};

CCharString CCombatActionBase::GetStrikeEventHelperName() const
{
    CStrikeEvent* pEvent = GetPActiveStrikeEvent();
    if (pEvent != 0) {
        if (!pEvent->Legacy)
            return pEvent->HelperPointName;
    }
    return CCharString("", -1);
}
