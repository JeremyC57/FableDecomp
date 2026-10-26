// Behaviour test for 0082c4c0 CMiniMapDisplay::CMarkerInfo(const CThing&, long, bool) (copy of the model).
#include <cstdio>
class CThing { public: int id; };
class CThingObject : public CThing { };
static int g_ctor = 0, g_get = 0, g_set = 0;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
    CThing* GetPItem() const { ++g_get; return m_pItem; }
    void SetPItem(CThing* item) { ++g_set; m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
class CMarkerInfo {
public:
    CIntelligentPointer<const CThing> PThing;
    long                              GraphicIndex;
    bool                              AutoMarker;
    CMarkerInfo(const CThing& thing, long graphicIndex, bool autoMarker);
};
CMarkerInfo::CMarkerInfo(const CThing& thing, long graphicIndex, bool autoMarker)
    : PThing(&thing), GraphicIndex(graphicIndex), AutoMarker(autoMarker)
{
}

int main()
{
    CThing t; t.id = 9;
    g_ctor = g_get = g_set = 0;
    CMarkerInfo m(t, 42, true);
    int ok = 1;
    if (m.PThing.m_pItem != &t || m.GraphicIndex != 42 || !m.AutoMarker || g_ctor != 1 || g_set != 1) ok = 0;
    CMarkerInfo n(t, -1, false);
    if (n.GraphicIndex != -1 || n.AutoMarker || n.PThing.m_pItem != &t) ok = 0;
    if (!ok) { std::printf("0082c4c0_TEST FAIL\n"); return 1; }
    std::printf("0082c4c0_TEST PASS\n");
    return 0;
}