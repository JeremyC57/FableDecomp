// Behaviour test for CTCBoastingPosition::ActivatePosition (0x00781F10).
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
struct CCharString { const char* PStringData; CCharString(const char* t, long = -1) : PStringData(t) {} ~CCharString() {} };
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CTCGraphicAppearance { bool m_flag10; };
static bool g_drawable = false; static long g_forward = 0;
class CThing {
public:
    virtual void SetAsDrawable(bool drawable) { g_drawable = drawable; }
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    void ForwardToGraphicAppearance(long request) { g_forward = request; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
static CThing g_effect; static const C3DVector* g_at = 0; static bool g_nameOk = false;
static CThing* Create(const CCharString& defName, const C3DVector& position, bool flag)
{ g_nameOk = std::strcmp(defName.PStringData, "PODIUM_ORB_IDLE_02") == 0 && !flag; g_at = &position; return &g_effect; }
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CTCBoastingPosition {
public:
    CThing* ParentThing; C3DVector OrbPosition; bool PositionActivated; long BoastIndex; long Cost; bool BoastOn;
    CIntelligentPointer<CThing> PParticleEffect;
    CTCBoastingPosition() : PParticleEffect(0) {}
    void ActivatePosition(long boastIndex, long cost)
    {
        PositionActivated = true; BoastIndex = boastIndex; Cost = cost; BoastOn = true;
        {
            CCharString name("PODIUM_ORB_IDLE_02");
            PParticleEffect = CIntelligentPointer<CThing>(Create(name, OrbPosition, false));
        }
        ParentThing->SetAsDrawable(true);
        ParentThing->ForwardToGraphicAppearance(4);
        CTCGraphicAppearance* graphic;
        if (ParentThing->GetInterface(&graphic, 9))
            graphic->m_flag10 = true;
    }
};

int main()
{
    CTCGraphicAppearance app; app.m_flag10 = false;
    CInterfaceMap::SEntry entries[1] = { { 9, (CTCBaseInterface*)&app } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0] = 1UL << 9; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CTCBoastingPosition p; p.ParentThing = &thing; p.PositionActivated = false; p.BoastOn = false; p.BoastIndex = 0; p.Cost = 0;
    p.ActivatePosition(3, 400);
    bool ok = p.PositionActivated && p.BoastOn && p.BoastIndex == 3 && p.Cost == 400 && g_nameOk && g_at == &p.OrbPosition
        && p.PParticleEffect.GetPItem() == &g_effect && g_drawable && g_forward == 4 && app.m_flag10;
    if (!ok) { std::printf("00781f10_TEST FAIL\n"); return 1; }
    std::printf("00781f10_TEST PASS\n");
    return 0;
}
