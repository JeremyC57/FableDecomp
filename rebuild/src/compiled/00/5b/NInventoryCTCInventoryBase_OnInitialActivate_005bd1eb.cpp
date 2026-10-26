// NInventory::CTCInventoryBase::OnInitialActivate() -- unless the quest manager is between a quest's start and finish screens, strips every category of its non-quest-persistent items
// Leaf: ?OnInitialActivate@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD1EB, length 74 bytes; size-optimised TU (`push 2Ch ; pop ecx` divisor).
// Callees: CQuestManager::IsBetweenQuestStartAndFinishScreens @ 0x004AF290 on the global quest manager @ 0x013B89FC;
// CCategory::RemoveAllNonQuestPersistentItems @ 0x005BC836 (landed).
// Retail CTCInventoryBase: no engine header; Categories (std::vector<NInventory::CCategory>, 0x2C-byte elements,
// {first, last, end}) sits at +0x20 (PDB +0x24 minus the 4-byte-narrower Components vector). The loop re-reads
// size() every iteration (`sub ; cdq ; idiv`, unsigned `jb` compare) and re-indexes `first` through the stride register.
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 6);
// Ego_r.exe ?OnInitialActivate@CTCInventoryBase@NInventory@@UAEXXZ 0x0062011E is 74 bytes / 35 instructions like retail.
#pragma optimize("s", on)
class CQuestManager { public:
    bool IsBetweenQuestStartAndFinishScreens();  // 0x004AF290
};
extern CQuestManager* g_pQuestManager;           // 0x013B89FC

struct CCategory_Retail {
    unsigned char _body[0x2C];
    void RemoveAllNonQuestPersistentItems();     // 0x005BC836
};
struct CCategoryVector {                         // retail std::vector<CCategory>
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();                                  // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void OnInitialActivate();                                 // slot 6 (+0x18)
    unsigned char   _pad_0x04[0x1C];
    CCategoryVector Categories;                  // +0x20
};

} // namespace NInventory

void NInventory::CTCInventoryBase::OnInitialActivate()
{
    if (!g_pQuestManager->IsBetweenQuestStartAndFinishScreens()) {
        for (unsigned long i = 0; i < Categories.size(); ++i)
            Categories[i].RemoveAllNonQuestPersistentItems();
    }
}