// CCombatActionBase::GetWeaponUsedInStrikeAction() const -- the weapon of the active strike event first to finish, when that weapon is still alive; else null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00857070, length 85 bytes (+ int3 padding).
// Callees: std::for_each<list<CCountedPointer<CStrikeEvent>>::const_iterator, CGetActiveStrikeEventThatIsFirstToFinish>
// @ 0x0085A960 (__fastcall; the functor result through the hidden pointer in ecx, both iterators and the functor
// pushed; the functor temporary is built in registers (frame in eax, `xor edx,edx`, `mov esi,0F4240h`) and its
// three dwords are copied into the argument slot `sub esp,0Ch ; mov ebx,esp ; mov [ebx],eax ...` by the IMPLICIT
// copy constructor; the result is read through the returned pointer (`mov ...,[eax+4]`: an unnamed temporary). Same prologue as IsStrikeEvent
// @ 0x00856F80; StrikeEvents is the retail std::list at +0xbc, FramesRunning +0x1c (Ego_r.pdb names), retail
// CCombatActionBase sizeof 0x114 so the layout is modelled locally.
// Also: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on the event's PWeapon (+0x14, Ego_r.pdb); the dead bit is
// +0x91 & 1 of the thing (the IS_THING_DEAD test of the landed 0x00836BE0).
// Shape: nested ifs, one `xor eax,eax` null tail shared by the three failing paths, the live weapon returned in eax.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char DeadFlags;                    // +0x91 (bit 0: dead)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CStrikeEvent {
    bool Legacy;                                // +0x00
    bool PrimaryLegacy;                         // +0x01
    bool Weapon;                                // +0x02
    void* HelperPointName;                      // +0x04 (CCharString)
    long StrikeEventIndex;                      // +0x08
    long StartFrame;                            // +0x0c
    long EndFrame;                              // +0x10
    CBaseIntelligentPointer PWeapon;            // +0x14 (CIntelligentPointer<const CThing>)
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
    CThing* GetWeaponUsedInStrikeAction() const;
};

CThing* CCombatActionBase::GetWeaponUsedInStrikeAction() const
{
    CStrikeEvent* pEvent = GetPActiveStrikeEvent();
    if (pEvent != 0) {
        CThing* weapon = pEvent->PWeapon.GetPItem();
        if (weapon != 0) {
            if (!(weapon->DeadFlags & 1))
                return weapon;
        }
    }
    return 0;
}