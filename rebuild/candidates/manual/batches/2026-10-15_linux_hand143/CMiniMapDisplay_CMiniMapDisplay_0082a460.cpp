// CMiniMapDisplay::CMiniMapDisplay() -- mini-map display constructor: the CBase root, this vtable, one map primitive handle, empty marker handles / marker list / minimap list, a fade-in/fade-out flash fading to opaque black, the region minimap, no region
// Leaf: ??0CMiniMapDisplay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A460, length 150 bytes.
// Label correction: catalogued as CHeroMorphDef::CHeroMorphDef, but the vtable stored is 0x01231768 =
// CMiniMapDisplay (rtti-vtables.tsv) and the members fit Ego_r.pdb CMiniMapDisplay with retail sizes.
// Callees: CBase::CBase @ 0x0099A2F0 (root vtable 0x0129A7C4), vector<CEnginePrimitiveHandle>::vector(size_type)
// @ 0x00485D30 (labelled "map::operator[]"), malloc @ 0x00BFEA0E (the list's 0x18-byte sentinel node),
// CRegionMinimap::CRegionMinimap @ 0x0066CCC0. Vtable 0x01231768.
// Layout (retail: 12-byte vectors, a one-word list, a 0x64-byte CRegionMinimap): MapPrimitiveHandles +0x04,
// MarkerPrimitiveHandles +0x10, MiniMapGraphic +0x1c, Markers +0x20, MarkerFlashFade +0x24 (CFadeInFadeOutBase:
// Active +0, FadeInTime +4, FadeOutTime +8, Closing +0xc, Opening +0xd, OpenTimer +0x10, CloseTimer +0x14,
// ToColour +0x18), the four prison/hill flags +0x40, MinimapList +0x44, RegionMinimapTexture +0x50,
// RegionMinimap +0x54, PRegion +0xb8, UpdateRequired +0xbc.
// Shape: the list sentinel is the inlined VC6 list constructor (head zeroed, malloc, self-linked); the fade's
// colour is a CRGBColour(R, G, B, A) frame temporary (stored R, G, B, then A) copied as one word.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};
struct CEnginePrimitiveHandleVector {              // vector<CEnginePrimitiveHandle>, built out of line with a count
    void* m_first; void* m_last; void* m_end;
    CEnginePrimitiveHandleVector(unsigned long count);   // 0x00485D30
};
template <class T> struct CRetailVector {          // retail vector: first / last / end
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
struct CEnginePrimitiveHandle;
struct CMinimapData;
struct SMarkerNode {                               // list<CMarkerInfo> node: next, prev, 16-byte value
    SMarkerNode* m_next;
    SMarkerNode* m_prev;
    long         m_value[4];
};
struct CMarkerList {                               // retail list<CMiniMapDisplay::CMarkerInfo>: the sentinel pointer
    SMarkerNode* m_head;
    CMarkerList() : m_head(0)
    {
        SMarkerNode* node = (SMarkerNode*)malloc(sizeof(SMarkerNode));
        node->m_next = node;
        node->m_prev = node;
        m_head = node;
    }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CFadeInFadeOutBase {
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0c
    bool       Opening;                            // +0x0d
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
    CFadeInFadeOutBase()
    {
        Active = false;
        CloseTimer = 0;
        OpenTimer = 0;
        Opening = false;
        Closing = false;
        ToColour = CRGBColour(0, 0, 0, 0xff);
    }
};
struct CRegionMinimap {
    unsigned char _body[0x64];
    CRegionMinimap();                              // 0x0066CCC0
};
struct CTexture;
struct CRegion;

struct CMiniMapDisplay : CBase {
    CEnginePrimitiveHandleVector         MapPrimitiveHandles;      // +0x04
    CRetailVector<CEnginePrimitiveHandle> MarkerPrimitiveHandles;  // +0x10
    long                                 MiniMapGraphic;           // +0x1c
    CMarkerList                          Markers;                  // +0x20
    CFadeInFadeOutBase                   MarkerFlashFade;          // +0x24
    bool                                 MiniMapIsPrisonCourtyard1;   // +0x40
    bool                                 MiniMapIsPrisonCourtyard2;   // +0x41
    bool                                 MiniMapIsHeadsmansHillNorth; // +0x42
    bool                                 MiniMapIsHeadsmansHillSouth; // +0x43
    CRetailVector<CMinimapData>          MinimapList;              // +0x44
    CTexture*                            RegionMinimapTexture;     // +0x50
    CRegionMinimap                       RegionMinimap;            // +0x54
    CRegion*                             PRegion;                  // +0xb8
    bool                                 UpdateRequired;           // +0xbc
    virtual void v00();
    CMiniMapDisplay();
};

CMiniMapDisplay::CMiniMapDisplay()
    : MapPrimitiveHandles(1), MiniMapGraphic(0),
      MiniMapIsPrisonCourtyard1(false), MiniMapIsPrisonCourtyard2(false), MiniMapIsHeadsmansHillNorth(false), MiniMapIsHeadsmansHillSouth(false),
      RegionMinimapTexture(0), PRegion(0), UpdateRequired(false)
{
}
