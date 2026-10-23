// CTCLook::IsCurrentlyTurningViaAnimation() -- true when the owner's interface-90 component reports a state of kind 7 whose list is non-empty and whose +0x51 bit 1 is clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753B40, length 95 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the interface-map search), the component's
// accessor @ 0x0070B470 (`return [this+0Ch]`; the manifest's CWorld::DrawGetEnvironment is propagated).
// Shape: the thing's inline GetInterface(type, &out) of the landed 0x007504E0 / 0x0075D940 family with type 90 (word 2,
// bit 26: `test [eax+28h],4000000h`; key 5Ah in the `push ecx` frame slot). The bool result is ignored, so the absent
// path reads the uninitialised out word from the same slot (`mov eax,[esp]`) -- the out pointer must NOT be
// initialised. The state test is an `&&` chain with two exits (`mov al,1` / `xor al,al`).
// Layout: retail CThing has the interface bitset at +0x20 and the vector map at +0x44 (landed relatives); CTCBase owner at +4.
struct CTCBase;

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44

    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTurnState                                   // the state object handed out by the interface-90 component
{
    unsigned char  _pad_0x00[4];
    void*          m_pFirst;                        // +0x04
    void*          m_pLast;                         // +0x08
    unsigned char  _pad_0x0c[0x50 - 0x0c];
    unsigned char  m_kind;                          // +0x50
    unsigned char  m_flags;                         // +0x51
};

struct CTCTurnSource                                // interface 90
{
    CTurnState* GetState();                         // 0x0070B470
};

enum { TC_INTERFACE_90 = 90 };

struct CTCLook
{
    void*   __vftable;                              // +0x00 (CTCBase)
    CThing* m_pThing;                               // +0x04 (CTCBase owner)
    bool IsCurrentlyTurningViaAnimation();
};

bool CTCLook::IsCurrentlyTurningViaAnimation()
{
    CTCTurnSource* source;
    m_pThing->GetInterface(TC_INTERFACE_90, &source);
    CTurnState* state = source->GetState();
    if (state->m_kind == 7 && state->m_pFirst != state->m_pLast && !(state->m_flags & 2))
        return true;
    return false;
}