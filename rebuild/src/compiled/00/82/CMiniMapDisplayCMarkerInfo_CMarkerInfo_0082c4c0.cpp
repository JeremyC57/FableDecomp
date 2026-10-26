// CMiniMapDisplay::CMarkerInfo::CMarkerInfo(const CThing& thing, long graphicIndex, bool autoMarker) -- a minimap marker: intelligent pointer to the marked thing, its marker graphic and the auto-marker flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082C4C0, length 46 bytes.
// Leaf: ??0CMarkerInfo
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90. Vtable
// 0x01237F1C = CIntelligentPointer<const CThing> (landed sources).
// Label correction: the manifest's "CIntelligentPointer<const CThing>::CIntelligentPointer" is propagated. The
// layout is Ego_r.pdb CMarkerInfo (16 bytes): CIntelligentPointer<CThing const> PThing +0, long GraphicIndex
// +8, bool AutoMarker +0xc (the landed CMiniMapDisplay rows 0x0082A210.. read the same offsets), and
// FableWin.pdb names the constructor ??0CMarkerInfo@CMiniMapDisplay@@QAE@ABVCThing@@J_N@Z -- the thing by
// reference (its address goes straight to SetPItem), the graphic and the flag by value (`ret 0xc`).
// Shape: base constructor, vtable store between the argument push and SetPItem, then both remaining
// arguments loaded before the two stores (dword, then byte).
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01237F1C for const CThing
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
};

class CMiniMapDisplay { public:
    class CMarkerInfo {
    public:
        CIntelligentPointer<const CThing> PThing;         // +0x00
        long                              GraphicIndex;   // +0x08
        bool                              AutoMarker;     // +0x0c
        CMarkerInfo(const CThing& thing, long graphicIndex, bool autoMarker);
    };
};

CMiniMapDisplay::CMarkerInfo::CMarkerInfo(const CThing& thing, long graphicIndex, bool autoMarker)
    : PThing(&thing), GraphicIndex(graphicIndex), AutoMarker(autoMarker)
{
}