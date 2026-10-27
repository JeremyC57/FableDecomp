// behaviour test for 0x006E8B10 CDefendingCombatantInfo::CProng::Reset() (model copy of the source; CBaseIntelligentPointer's out-of-line members are stubs)
#include <cstdio>
class CThingCreatureBase;
class CBaseObject;
static int g_ctor = 0, g_dtor = 0, g_set = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : Item(0) { ++g_ctor; }
    virtual ~CBaseIntelligentPointer() { ++g_dtor; }
    CBaseObject* GetPItem() const { return Item; }
    void SetPItem(CBaseObject* item) { Item = item; ++g_set; }
    CBaseObject* Item;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };              // 1-aligned in retail
#pragma pack(pop)
class CDefendingCombatantInfo {
public:
    class CProng {                                       // Ego_r.pdb, 28 bytes
    public:
        void Reset();
        bool Blocked;                                    // +0x00
        C3DVector Prong;                                 // +0x01
        long ProngBlockageTimer;                         // +0x10
        CIntelligentPointer<CThingCreatureBase> POccupier;   // +0x14
    };
};

void CDefendingCombatantInfo::CProng::Reset()
{
    POccupier = CIntelligentPointer<CThingCreatureBase>(0);
}
int main()
{
    static double storage[(sizeof(CDefendingCombatantInfo::CProng) + 7) / 8];   // raw storage: the members are not constructed by the test
    CDefendingCombatantInfo::CProng* o = (CDefendingCombatantInfo::CProng*)storage;
    int dummy = 0;
    o->POccupier.Item = (CBaseObject*)&dummy;
    g_ctor = g_dtor = g_set = 0;
    o->CDefendingCombatantInfo::CProng::Reset();
    int ok = o->POccupier.Item == 0 && g_ctor == 1 && g_dtor == 1 && g_set == 2;
    if (!ok) { std::printf("006e8b10_TEST FAIL\n"); return 1; }
    std::printf("006e8b10_TEST PASS\n");
    return 0;
}