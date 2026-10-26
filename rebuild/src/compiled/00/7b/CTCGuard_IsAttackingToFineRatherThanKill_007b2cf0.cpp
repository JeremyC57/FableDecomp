// CTCGuard::IsAttackingToFineRatherThanKill() const -- true when the guard's village (via its CTCVillageMember interface, 0x23) exists and has a positive fine for the current crimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2CF0, length 92 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,..>::LowerBound @ 0x004365B0 (the thing's interface map at +0x44), CTCVillageMember::
// GetPVillage @ 0x007EE540 (manifest "IS_THING_ALIVE<..>" is propagated), CTCVillage::GetFineAmountForCurrentCrimes @ 0x0065A41C.
// Shape: CThing::GetInterface(T** out, type) is inline: the presence bit (`test byte [thing+24h],8` = word 1, bit 3 of the
// bitset at +0x20), then `LowerBound(&key)` with the key in the frame slot, the `it == end || type < it->type ? end : it`
// select and `*out = it->component`. The out pointer is NOT initialised (when the bit is clear retail passes the stale frame
// slot `mov eax,[esp+4]`). `result = false` lives in bl (`xor bl,bl`), `mov al,1 ; jg` + `mov al,bl` is `result = true` under
// `village && fine > 0`.
// Local model: no engine header exists for CTCGuard or CThing; the interface-map members are descriptive.
// CTCVillage: engine/CTCVillage.h holds the PDB data layout of CTCVillage; only its member call(s) are needed here, so the
// callee is declared on a separate call-only struct.
class CTCBase;
class CTCVillage { public:
    long GetFineAmountForCurrentCrimes();                   // 0x0065A41C
};
class CTCVillageMember { public:
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;                   // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCGuard { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool IsAttackingToFineRatherThanKill() const;
};

bool CTCGuard::IsAttackingToFineRatherThanKill() const
{
    bool result = false;
    CTCVillageMember* member;
    ParentThing->GetInterface(&member, 0x23);               // TC_VILLAGE_MEMBER
    CTCVillage* village = member->GetPVillage();
    if (village != 0 && village->GetFineAmountForCurrentCrimes() > 0)
        result = true;
    return result;
}