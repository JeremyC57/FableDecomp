#include <cstdio>
class CBaseObject { public: int id; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { };
class CTCScriptedControl { public:
    unsigned char _pad[0xc];
    CBaseIntelligentPointer m_pController;
    unsigned char m_locked;
    void ClearLocked()
    {
        {
            CIntelligentPointer pointer;
            pointer.SetPItem(0);
            m_pController.SetPItem(pointer.GetPItem());
        }
        m_locked = 0;
    }
};
int main()
{
    CBaseObject thing = { 7 };
    CTCScriptedControl c; c.m_pController.SetPItem(&thing); c.m_locked = 1;
    c.ClearLocked();
    if (c.m_pController.GetPItem() != 0 || c.m_locked != 0) { std::printf("00712c80_TEST FAIL\n"); return 1; }
    std::printf("00712c80_TEST PASS\n");
    return 0;
}