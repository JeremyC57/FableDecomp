// Behaviour test for CTCInventory::UnAssignThingPC (0x00584F15): the slot is emptied; a found assignment icon gets
// the {1, empty} action parameter and a zero alpha (other channels kept); the container receives a fresh
// "PC_EXPRESSION_ITEM_INSIDE_CONTAINER" component; every temporary is destroyed.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; ++g_live; }
    CCharString(const CCharString& o) : Text(o.Text) { ++g_live; }
    ~CCharString() { --g_live; }
};
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour() {} };
struct CComponentList { int n; ~CComponentList() {} };
struct SActionParamPair {
    unsigned long first; CComponentList second;
    SActionParamPair() { second.n = 0; ++g_live; }
    SActionParamPair(const SActionParamPair& o) : first(o.first) { second.n = o.second.n; ++g_live; }
    ~SActionParamPair() { --g_live; }
};
template<class T> class CCountedPointer { public:
    T* m_pObject;
    CCountedPointer(T* object) : m_pObject(object) { ++g_live; }
    ~CCountedPointer() { --g_live; }
};
namespace NUISystem {
class CComponent { public:
    const char* ChildName; CComponent* Child; CRGBColour Colour; unsigned long Param; int ParamCount; CComponent* Contents;
    CComponent() : ChildName(""), Child(0), Param(0), ParamCount(0), Contents(0) { Colour.B = 1; Colour.G = 2; Colour.R = 3; Colour.A = 255; }
    virtual CComponent* FindChild(CCharString name) { return std::strcmp(name.Text, ChildName) == 0 ? Child : 0; }
    virtual void SetColour(const CRGBColour& c) { Colour = c; }
    virtual CRGBColour GetColour() const { return Colour; }
    virtual void SetContents_slot60(const CCountedPointer<CComponent>& child) { Contents = child.m_pObject; }
    virtual void SetActionParameter_slot62(SActionParamPair p) { Param = p.first; ParamCount = p.second.n; }
};
static CComponent g_created; static const char* g_createdName = 0;
class CManager { public: CComponent* CreateComponent(const CCharString& d, bool) { g_createdName = d.Text; return &g_created; } };
class CFrontEndManager : public CManager { public: static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; } };
}
struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; long& operator[](unsigned long n) { return _M_start[n]; } };
class CTCInventory { public:
    CLongVector QuickAccessItems;
    void UnAssignThingPC(unsigned long slot, NUISystem::CComponent* container);
};
void CTCInventory::UnAssignThingPC(unsigned long slot, NUISystem::CComponent* container)
{
    QuickAccessItems[slot] = 0;
    NUISystem::CComponent* icon = container->FindChild(CCharString("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER", -1));
    if (icon) {
        SActionParamPair parameter;
        parameter.first = 1;
        icon->SetActionParameter_slot62(parameter);
        CRGBColour colour = icon->GetColour();
        colour.A = 0;
        icon->SetColour(colour);
    }
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER", false));
    container->SetContents_slot60(item);
}
int main()
{
    long slots[3] = { 5, 6, 7 };
    CTCInventory inv; inv.QuickAccessItems._M_start = slots; inv.QuickAccessItems._M_finish = inv.QuickAccessItems._M_end_of_storage = slots + 3;
    NUISystem::CComponent container, icon;
    inv.UnAssignThingPC(1, &container);
    if (slots[1] != 0 || slots[0] != 5 || container.Contents != &NUISystem::g_created || std::strcmp(NUISystem::g_createdName, "PC_EXPRESSION_ITEM_INSIDE_CONTAINER") || g_live != 0) { std::printf("00584f15_TEST FAIL\n"); return 1; }
    container.ChildName = "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER"; container.Child = &icon;
    inv.UnAssignThingPC(2, &container);
    if (slots[2] != 0 || icon.Param != 1 || icon.ParamCount != 0 || icon.Colour.A != 0 || icon.Colour.R != 3 || icon.Colour.B != 1 || g_live != 0) { std::printf("00584f15_TEST FAIL\n"); return 1; }
    std::printf("00584f15_TEST PASS\n");
    return 0;
}