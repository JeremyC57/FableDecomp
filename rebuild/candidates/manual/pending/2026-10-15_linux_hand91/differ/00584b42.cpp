// Grade: DIFFER(70v68) -- register choice only at the virtual call: ours `mov eax,[eax+4] ; mov edx,[eax] ; mov ecx,eax ; call [edx+7Ch]`, retail `mov ecx,[eax+4] ; mov eax,[ecx] ; call [eax+7Ch]`
#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::ClosePCInventoryMenu() -- asks the owner's in-game-menu component (interface type 0x12) to close its PC inventory menu
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584B42, length 68 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `leave`).
// Callees: std::bitset::test 0x00410DE0 on the thing's interface-presence bits (+0x20), CVectorMap::LowerBound
//          0x0040F020 on its sorted {type, component} map (+0x44), then virtual slot 31 (`call [eax+7Ch]`) on the
//          component found.
// Ego_r PDB: ?ClosePCInventoryMenu@CTCInventory@@QAEXXZ.
// The lookup is the thing's inlined GetInterface template (the landed out-of-line copies, e.g. 0x007BEA08, have
// the same bitset-then-lower-bound shape with a by-address key in the frame slot); a miss on the map yields
// the end entry, whose value is handed out regardless.
// Retail layout: CTCBase ParentThing at +0x04 (the PDB header layout of CTCInventory is wider), so `this` is
// modelled locally; the component and its slot are named by use.
// Shape: absent interface skips the call entirely (`je` to the epilogue); the found component is called with no
// null test. A size TU only inlines the lookup when it is `__forceinline`.
struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(long wanted, T** out)
    {
        if (m_present.test(wanted)) {
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
};

struct CTCInGameMenu {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ClosePCInventoryMenu();             // slot 31
};

struct CTCInventory {
    virtual void v00();
    CThing* ParentThing;                             // +0x04
    void ClosePCInventoryMenu();
};

void CTCInventory::ClosePCInventoryMenu()
{
    CTCInGameMenu* menu;
    if (ParentThing->GetInterface(0x12, &menu))
        menu->ClosePCInventoryMenu();
}
