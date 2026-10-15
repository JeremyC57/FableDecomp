#include <cstdio>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CTCTargetingPlayer {
    unsigned char _pad[0xe4];
    CBaseIntelligentPointer PTargetLockedThing;
    bool LockedOntoEnemy;
    void ClearLockedTarget()
    {
        {
            CIntelligentPointer pointer;
            pointer.SetPItem(0);
            PTargetLockedThing.SetPItem(pointer.GetPItem());
        }
        LockedOntoEnemy = 0;
    }
};
int main()
{
    CBaseObject thing = { 7 };
    CTCTargetingPlayer c; c.PTargetLockedThing.SetPItem(&thing); c.LockedOntoEnemy = 1;
    c.ClearLockedTarget();
    if (c.PTargetLockedThing.GetPItem() != 0 || c.LockedOntoEnemy != 0) { std::printf("00802650_TEST FAIL\n"); return 1; }
    std::printf("00802650_TEST PASS\n");
    return 0;
}
