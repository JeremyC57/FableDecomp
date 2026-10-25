// Behaviour test for 0x0082C370 CMiniMapDisplay::CMarkerInfo copy constructor.
#include <cstdio>
#include <cstring>

class CBaseObject {
public:
    int Id;
};
class CBaseIntelligentPointer {
public:
    CBaseObject* PData;
    static int s_sets;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; ++s_sets; }
};
int CBaseIntelligentPointer::s_sets = 0;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    explicit CIntelligentPointer(CBaseObject* p) { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer<T>& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
class CThing;
class CThingAICreature;
class CMarkerInfo {
public:
    CIntelligentPointer<const CThing> PThing;
    long                              GraphicIndex;
    bool                              AutoMarker;
    CMarkerInfo(CBaseObject* p, long g, bool a) : PThing(p), GraphicIndex(g), AutoMarker(a) {}
    CMarkerInfo(const CMarkerInfo& other) : PThing(other.PThing), GraphicIndex(other.GraphicIndex), AutoMarker(other.AutoMarker) {}
};

int main()
{
    CBaseObject obj; obj.Id = 1;
    CMarkerInfo a(&obj, 12, true);
    CBaseIntelligentPointer::s_sets = 0;
    CMarkerInfo b(a);
    if (!(CBaseIntelligentPointer::s_sets == 1 && b.PThing.GetPItem() == &obj && b.GraphicIndex == 12 && b.AutoMarker)) {
        std::printf("0082c370_TEST FAIL\n");
        return 1;
    }
    std::printf("0082c370_TEST PASS\n");
    return 0;
}