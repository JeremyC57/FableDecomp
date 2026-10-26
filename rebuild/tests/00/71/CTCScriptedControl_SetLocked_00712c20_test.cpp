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
    long m_lockType;
    void SetLocked(CBaseObject* thing, long lockType)
    {
        {
            CIntelligentPointer pointer;
            pointer.SetPItem(thing);
            m_pController.SetPItem(pointer.GetPItem());
        }
        m_locked = 1;
        m_lockType = lockType;
    }
};
int main()
{
    CBaseObject thing = { 7 };
    CTCScriptedControl c; c.m_locked = 0; c.m_lockType = 0;
    c.SetLocked(&thing, 3);
    if (c.m_pController.GetPItem() != &thing || c.m_locked != 1 || c.m_lockType != 3) { std::printf("00712c20_TEST FAIL\n"); return 1; }
    std::printf("00712c20_TEST PASS\n");
    return 0;
}