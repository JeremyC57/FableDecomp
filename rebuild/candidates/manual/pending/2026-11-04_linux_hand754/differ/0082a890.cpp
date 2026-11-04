// CMiniMapDisplay::AddMarker(const CThing& thing, long graphicIndex) -- add a (non-automatic) minimap marker for the thing unless the index is not positive or the same thing/graphic pair is already listed
// Grade: DIFFER(206v206) -- esi/edi swap in the inline push_back (retail node edi, &value esi); PDB identity fixed AddMarker(const CThing&, long), CMarkerInfo(const CThing&, long, bool) + GetPThing/GetGraphicIndex, retail CIntelligentPointer (+COPY overlay), STLport std::list; the loop through a CMarkerInfo reference fixed the scan registers (dl 19 -> 12)
// Leaf: ?AddMarker@CMiniMapDisplay@@QAEXABVCThing@@J@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CMiniMapDisplay {
public:
    class CMarkerInfo {
    public:
        CMarkerInfo(const CThing& thing, long graphicIndex, bool autoMarker)
            : PThing(&thing), GraphicIndex(graphicIndex), AutoMarker(autoMarker) { }
        const CThing* GetPThing() const { return PThing.Get(); }
        long GetGraphicIndex() const { return GraphicIndex; }
    private:
        CIntelligentPointer<const CThing> PThing;   // +0x00
        long GraphicIndex;                          // +0x08
        bool AutoMarker;                            // +0x0c
    };
    void AddMarker(const CThing& thing, long graphicIndex);
    virtual void v00();
    unsigned char MapPrimitiveHandles[0xc];         // +0x04
    unsigned char MarkerPrimitiveHandles[0xc];      // +0x10
    long MiniMapGraphic;                            // +0x1c
    std::list<CMarkerInfo> Markers;                 // +0x20
};

void CMiniMapDisplay::AddMarker(const CThing& thing, long graphicIndex)
{
    if (graphicIndex <= 0)
        return;
    for (std::list<CMarkerInfo>::iterator it = Markers.begin(); it != Markers.end(); ++it) {
        const CMarkerInfo& info = *it;
        if (info.GetPThing() == &thing && info.GetGraphicIndex() == graphicIndex)
            return;
    }
    CMarkerInfo marker(thing, graphicIndex, false);
    Markers.push_back(marker);
}
