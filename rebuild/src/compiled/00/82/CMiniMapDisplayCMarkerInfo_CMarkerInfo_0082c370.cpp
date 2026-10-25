// CMiniMapDisplay::CMarkerInfo::CMarkerInfo(const CMarkerInfo& other) -- copy constructor of a mini-map marker: the marked thing's pointer, the graphic index and the auto-marker flag
// Leaf: ??0CMarkerInfo@CMiniMapDisplay@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082C370, 53 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<CIntelligentPointer<CThing>,long><CIntelligentPointer<CThing>,int>" is
//   propagated. A CIntelligentPointer<const CThing> (vtable 0x01237F1C) + long + bool = Ego_r.pdb CMiniMapDisplay::CMarkerInfo
//   (16 bytes: PThing +0, GraphicIndex +8, AutoMarker +0xC); FableWin.pdb ??0CMarkerInfo@CMiniMapDisplay@@QAE@ABV01@@Z.
//   It sits in the mini-map TU (the CMarkerInfo _Cons_val 0x0082C5E0 and remove_if 0x0082C6E0 follow). No direct callers.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Shape: the memberwise copy (pointer copy constructor inline, then the dword and the byte).
class CBaseObject;
class CThing;
class CThingAICreature;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                 // each instantiation has its own vtable
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CMiniMapDisplay {
public:
    class CMarkerInfo {
    public:
        CIntelligentPointer<const CThing> PThing;       // +0x00
        long                              GraphicIndex; // +0x08
        bool                              AutoMarker;   // +0x0C
        CMarkerInfo(const CMarkerInfo& other);
    };
};

CMiniMapDisplay::CMarkerInfo::CMarkerInfo(const CMarkerInfo& other)
    : PThing(other.PThing), GraphicIndex(other.GraphicIndex), AutoMarker(other.AutoMarker)
{
}