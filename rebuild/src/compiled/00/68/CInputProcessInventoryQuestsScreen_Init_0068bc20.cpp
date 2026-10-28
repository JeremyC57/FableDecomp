// CInputProcessInventoryQuestsScreen::Init() -- look the player's creature up for its type-0x19 component (the inlined bitset + sorted-map GetInterface) and keep it in PInventory; then reset the screen state words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068BC20, length 77 bytes (+ int3 padding). Vtable 0x01237DF4 slot 2.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18; `add ecx,2Ch ; jmp CBaseIntelligentPointer::GetPItem`), CVectorMap::LowerBound @ 0x0040F020
// (the thing's {type, component} map at +0x44; type 0x19 is bit 25 of dword 0 (+0x20, `test dword ptr [eax+20h],2000000h`) of the bitset at +0x20: the landed GetInterface template of 0x008CDCF0).
// Layout: Ego_r.pdb CInputProcessInventoryQuestsScreen (64 bytes: PInventory +0x34, CurrentQuestMovement +0x38, CurrentQuestSelectMovement +0x3c); CInputProcessGameBase::MyPlayer +0x18 (engine header), modelled locally with the base names.
// Shape: one `push ecx` frame slot for the lookup key, the map pointer advanced into esi once, the member itself is the out pointer of the
// lookup (`mov ecx,[eax+4] ; mov [edi+N],ecx`; a local out pointer copied to the member loads through eax instead); the state words are cleared after the lookup block from one zero register.
class CTCBase;
struct CInterfaceMap {                              // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
class CThingPlayerCreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;     // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;      // 0x00487DC0
};
class CTCInventoryQuests;
class CInputProcessInventoryQuestsScreen { public:                                         // vtable 0x01237DF4
    virtual void v00(); virtual void v01();
    virtual void Init();                            // slot 2
    unsigned char _pad04[0x10];                     // +0x04 CAInputProcess
    void*         World;                            // +0x14
    CPlayer*      MyPlayer;                         // +0x18
    void*         DefinitionManager;                // +0x1c
    void*         Component;                        // +0x20
    void*         PGamePlayerInterface;             // +0x24
    unsigned char _pad28[0x0c];                     // +0x28
    CTCInventoryQuests* PInventory;                 // +0x34
    long          CurrentQuestMovement;             // +0x38 ECurrentQuestMovement
    long          CurrentQuestSelectMovement;       // +0x3c ECurrentQuestSelectMovement
};

void CInputProcessInventoryQuestsScreen::Init()
{
    MyPlayer->GetPControlledCreature()->GetInterface(&PInventory, 0x19);
    CurrentQuestMovement = 0;
    CurrentQuestSelectMovement = 0;
}