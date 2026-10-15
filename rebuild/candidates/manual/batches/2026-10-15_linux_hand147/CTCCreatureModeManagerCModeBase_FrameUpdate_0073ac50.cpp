// CTCCreatureModeManager::CModeBase::FrameUpdate() -- counts FramesBeforeRemovingSelf down and, when it reaches zero, asks the owner's mode manager to remove this mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AC50, 112 bytes (padding included).
// Shape: a positive counter (+0x14) is decremented and re-read; at zero the owner's (ParentThing,
// +4) creature-mode manager (interface 0x31: present bit 17 of the +0x24 word, LowerBound
// 0x0040F020 with the key in the `push ecx` frame word) is fetched with the
// `T* p = 0; if (HasTC(k)) p = GetTC(k);` shape (the preloaded `xor esi,esi`), then
// RemoveMode(GetModeType()) (0x0073ABA0; GetModeType is vtable slot 1).
struct CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const     // the masked word, not a bool (a bool narrows to shr/and)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct CTCCreatureModeManager {                 // interface type 0x31 component
    void RemoveMode(long modeType);             // 0x0073ABA0

    struct CModeBase {
        virtual void v00();
        virtual long GetModeType();             // slot 1
        CThing* ParentThing;                    // +4
        unsigned char WeaponBasedFilter[0xc];   // +8 CAnimFilterOfGroup
        long FramesBeforeRemovingSelf;          // +0x14
        bool Active;                            // +0x18
        void FrameUpdate();
    };
};

void CTCCreatureModeManager::CModeBase::FrameUpdate()
{
    if (FramesBeforeRemovingSelf > 0)
        --FramesBeforeRemovingSelf;
    if (FramesBeforeRemovingSelf == 0) {
        CTCCreatureModeManager* modes = 0;
        if (ParentThing->HasTC(0x31))
            modes = (CTCCreatureModeManager*)ParentThing->GetTC(0x31);
        modes->RemoveMode(GetModeType());
    }
}
