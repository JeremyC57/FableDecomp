// CThingBuilding::IsUnlocked() const -- a building with a door-list component is unlocked when none of its live door things (those carrying the door component) is locked, barred or shut; a building without the component is never unlocked
// Leaf: IsUnlocked
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082DEF0, 193 bytes (+15 int3). `ret`, bool in al.
// Callees: CVectorMap::LowerBound(const long*) const 0x004365B0 (the building's own 0x21 lookup, const), CBaseIntelligentPointer::
//          GetPItem() const 0x00A01B50 (x3 per door: null test, Dead test, presence test), CVectorMap::LowerBound(const long*)
//          0x0040F020 (the door's 0x26 lookup).
// Shape: the 0x21 bit is word +0x24 bit 1, the 0x26 bit word +0x24 bit 6; the STLport list is walked from the header's next
//        node to the header, each node's intelligent pointer at +8 re-fetched for every test; the three door bytes are tested
//        in the order locked, barred, open.
// Layout: CThing presence bits +0x20, Components +0x44, Dead +0x91 bit 0; the door-list component keeps its list at +0x0C;
//         CTCDoor bytes +0x10 (open), +0x12 (locked), +0x13 (barred) (descriptive).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing;
struct CIntelligentPointer {                         // CIntelligentPointer<CThing>
    unsigned char _pad00[8];
    CThing* GetPItem() const;                        // 0x00A01B50 (CBaseIntelligentPointer)
};
struct SDoorNode {                                   // STLport list node
    SDoorNode*          _M_next;
    SDoorNode*          _M_prev;
    CIntelligentPointer _M_data;                     // +0x08
};
class CTCDoor { public:                                     // interface type 0x26
    unsigned char _pad00[0x10];
    bool          Open;                              // +0x10 (descriptive)
    unsigned char _pad11;
    bool          Locked;                            // +0x12 (descriptive)
    bool          Barred;                            // +0x13 (descriptive)
};
struct CTCDoorList {                                 // interface type 0x21 (descriptive)
    unsigned char _pad00[0x0c];
    SDoorNode*    Doors;                             // +0x0c list<CIntelligentPointer<CThing>> header node
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                          // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(const T** out, long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingBuilding : public CThing { public:
    bool IsUnlocked() const;
};
bool CThingBuilding::IsUnlocked() const
{
    const CTCDoorList* doors;
    if (GetInterface(&doors, 0x21)) {
        for (SDoorNode* node = doors->Doors->_M_next; node != doors->Doors; node = node->_M_next) {
            const CIntelligentPointer& door = node->_M_data;
            if (door.GetPItem() && !door.GetPItem()->Dead) {
                CThing* thing = door.GetPItem();
                CTCDoor* tc;
                if (thing->GetInterface(&tc, 0x26)) {
                    if (tc->Locked || tc->Barred || !tc->Open)
                        return false;
                }
            }
        }
        return true;
    }
    return false;
}