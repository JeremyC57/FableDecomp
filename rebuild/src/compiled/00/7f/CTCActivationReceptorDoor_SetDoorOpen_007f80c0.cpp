// CTCActivationReceptorDoor::SetDoorOpen(bool) -- forward an open/close request to the parent thing's door component (interface 0x26)
// Leaf: SetDoorOpen
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F80C0, length 86 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup,
// type 0x26 = bit 6 of the bitset dword at +0x24, tested with `test byte ptr [eax+24h],40h`), then
// CTCDoor::ScriptTriggerOpen(bool) @ 0x007305D0 (stores the flag at the door's +0x10, notifying the thing
// when it changes).
// Identity: the manifest label "CTCActionUseAcceptBoast::OnActionUse" is propagated over the byte-identical
// GetInterface shape. The only callers are CTCActivationReceptorDoor's vtable slots 28/29 (0x007F8120 passes
// true, 0x007F8140 the other state) right after it; the method name is descriptive.
//
// Shape: the inlined GetInterface template (landed 0x00949320) with an UNINITIALISED component local: hit
// path -> the lookup result in eax; miss path -> the never-written local, homed in the argument's own
// stack slot (`mov eax,[esp+8]` beside the argument load). The bool argument is forwarded as its whole
// stack dword (`mov ecx,[esp+..] ; push ecx`). Frame: one `push ecx` (the key temporary).
// Header note: engine/CTCDoor.h holds the layout only; the callee is modelled as CTCDoor_Calls.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCDoor_Calls {                                // interface type 0x26 component
    void ScriptTriggerOpen(bool open);          // 0x007305D0
};

struct CTCActivationReceptorDoor {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    void SetDoorOpen(bool open);
};

void CTCActivationReceptorDoor::SetDoorOpen(bool open)
{
    CTCDoor_Calls* door;
    ParentThing->GetInterface(&door, 0x26);
    door->ScriptTriggerOpen(open);
}