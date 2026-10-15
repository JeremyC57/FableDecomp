// COpenDoor::operator()(CThing* thing) const -- script door functor: if the thing has a door component, trigger it open/closed -- to the functor's own state when it is forced, otherwise to the opposite of the door's current (settled) state
// Leaf: ??RCOpenDoor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AD530, length 142 bytes (padded to 144).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCDoor::ScriptTriggerOpen(bool) @ 0x007305D0
//          (tail-jumped in both arms with the argument written over the caller's slot).
// Layout: the functor is two bytes (m_open +0, m_force +1; landed callers 0x00895DB0 OpenDoor / 0x00895DE0 CloseDoor);
// CTCDoor (TC type 0x26 = bit 6 of presence word 1, PDB): CurrentOpen +0x10, OldOpen +0x11, Opening +0x12, Closing +0x13.
// Shape: the inline IsOpen() returns the `!Opening && !Closing && CurrentOpen` expression (materialised in a full
// register, `mov eax,1` / `xor eax,eax`; an if/return-true form gives `mov al,1`), then the caller's
// `test al,al ; sete al` for the negation; the two calls are separate if/else arms.
struct CTCBase;
struct CTCDoor {
    unsigned char _pad00[0x10];
    bool CurrentOpen;                       // +0x10
    bool OldOpen;                           // +0x11
    bool Opening;                           // +0x12
    bool Closing;                           // +0x13
    void ScriptTriggerOpen(bool open);      // 0x007305D0
    bool IsOpen() const
    {
        return !Opening && !Closing && CurrentOpen;
    }
};
struct CInterfaceMap {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];         // +0x20 bitset of the interface types present
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

enum { TC_DOOR = 0x26 };

struct COpenDoor {
    bool m_open;                            // +0
    bool m_force;                           // +1
    void operator()(CThing* thing) const;
};

void COpenDoor::operator()(CThing* thing) const
{
    if (thing && thing->HasTC(TC_DOOR))
    {
        CTCDoor* door = (CTCDoor*)thing->GetTC(TC_DOOR);
        if (m_force)
            door->ScriptTriggerOpen(m_open);
        else
            door->ScriptTriggerOpen(!door->IsOpen());
    }
}