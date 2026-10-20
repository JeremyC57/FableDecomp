// Behaviour test for CTCInventoryExperience::GetExperienceStatPic 0x0056F177 (model copy with stub manager/bank/component)
#include <cstdio>
#include <cstring>

struct CCharString {
    char m_text[64];
    CCharString() { m_text[0] = 0; }
    CCharString(const char* text, long length = -1) { (void)length; std::strcpy(m_text, text); }
    CCharString& operator=(const char* text) { std::strcpy(m_text, text); return *this; }
};

struct CGraphicDataBank {
    long FindEntry(const CCharString& name)
    {
        if (std::strcmp(name.m_text, "UI_RING_PIC_HEALTH") == 0) return 11;
        if (std::strcmp(name.m_text, "UI_RING_PIC_MAGIC_POWER") == 0) return 19;
        return 0;
    }
};

static int g_refs = 0;
template<class T> struct CCountedPointer {
    T* m_pObject;
    CCountedPointer(T* p) : m_pObject(p) {}
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject) { ++g_refs; }
    ~CCountedPointer() { --g_refs; }
    T* operator->() const { return m_pObject; }
};

struct CComponent {
    char m_def[64];
    long m_graphic;
    virtual void SetGraphic(long graphicIndex) { m_graphic = graphicIndex; }
};

static CComponent g_component;
static CGraphicDataBank g_bank;

struct CFrontEndManager {
    CCountedPointer<CGraphicDataBank> GraphicsBank;
    CFrontEndManager() : GraphicsBank(&g_bank) {}
    CComponent* CreateComponent(const CCharString& definition, bool) { std::strcpy(g_component.m_def, definition.m_text); return &g_component; }
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
};

struct CTCInventoryExperience {
    CComponent* GetExperienceStatPic(long stat);
};

CComponent* CTCInventoryExperience::GetExperienceStatPic(long stat)
{
    CCharString picName;
    if (stat == 0) picName = "UI_RING_PIC_PHYSIQUE";
    if (stat == 1) picName = "UI_RING_PIC_HEALTH";
    if (stat == 2) picName = "UI_RING_PIC_TOUGHNESS";
    if (stat == 3) picName = "UI_RING_PIC_SPEED";
    if (stat == 4) picName = "UI_RING_PIC_ACCURACY";
    if (stat == 5) picName = "UI_RING_PIC_GUILE";
    if (stat == 9) picName = "UI_RING_PIC_MAGIC_POWER";
    CFrontEndManager* manager = CFrontEndManager::GetInstance();
    CComponent* pic = manager->CreateComponent("UI_RING_PIC", false);
    CCountedPointer<CGraphicDataBank> bank(manager->GraphicsBank);
    pic->SetGraphic(bank->FindEntry(picName));
    return pic;
}

int main()
{
    CTCInventoryExperience e;
    CComponent* c = e.GetExperienceStatPic(1);
    if (c != &g_component || c->m_graphic != 11 || std::strcmp(c->m_def, "UI_RING_PIC") != 0 || g_refs != 0) { std::printf("0056f177_TEST FAIL\n"); return 1; }
    if (e.GetExperienceStatPic(9)->m_graphic != 19 || e.GetExperienceStatPic(7)->m_graphic != 0) { std::printf("0056f177_TEST FAIL\n"); return 1; }
    std::printf("0056f177_TEST PASS\n");
    return 0;
}