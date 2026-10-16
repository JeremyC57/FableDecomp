// NUISystem::CUIStateDef::CUIStateDef() -- default constructor: CResource, the inlined CDefClassBase / CParentDefClassBase defaults (no def manager, flags clear, no instantiation name, empty sub-def map), own vtable, then the state defaults (graphic 0, change flag 7, zero position, unit zoom, white colour, update time -1, no children, not linear)
// Leaf: ??0CUIStateDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00630350, length 112 bytes, `ret`.
// Callees: CResource::CResource @ 0x009FBEC0. Vtable 0x0125871C (NUISystem::CUIStateDef; the novtable def bases store
// nothing, as in the landed CAIScratchpadDef ctor 0x007ABB30).
// Layout: as the copy ctor 0x006250B0 (byte-packed def bases: PDefManager +0x1c, flag bits +0x24, InstantiationName
// +0x25, SubDefInfoMap +0x29 with Dirty +0x36; GraphicIndex +0x3c, StateChangeFlag +0x40, StateChangeType +0x44,
// Position +0x48, Zoom +0x50, Colour +0x58, UpdateTime +0x68, ChildrenNotAffected +0x6c, LinearChange +0x78).
// Shape: body assignments; the zero group's last use (LinearChange) sits after the 1.0f stores in the source so the
// 1.0f constant takes ecx, and the two immediates (UpdateTime, StateChangeFlag) keep their source order at the end.
struct CResource {
    virtual void v00();
    unsigned char _body[0x18];                     // +0x04..+0x1c
    CResource();                                   // 0x009FBEC0
};
#pragma pack(push, 1)
struct __declspec(novtable) CDefClassBase : CResource {
    void*         PDefManager;                     // +0x1c
    unsigned long GlobalIndex;                     // +0x20 (not initialised here)
    unsigned char Setup : 1;                       // +0x24
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    CDefClassBase() : CResource(), PDefManager(0), Setup(0), Template(0), DefaultValsApplied(0) { }
};
#pragma pack(pop)
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T> (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};
struct CSubDefInfoPair;
struct CKeyPairCompareLess { };
struct CSubDefInfoMap {                            // CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, ...>
    CRetailVector<CSubDefInfoPair> Vector;         // +0x00
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                Dirty;                     // +0x0d
    CSubDefInfoMap() : Dirty(false) { }
};
#pragma pack(push, 1)
struct __declspec(novtable) CParentDefClassBase : CDefClassBase {
    CDefString     InstantiationName;              // +0x25
    CSubDefInfoMap SubDefInfoMap;                  // +0x29
    CParentDefClassBase() { }
};
#pragma pack(pop)
struct C2DVector { float X, Y; };
struct CRGBFloatColour { float R, G, B, A; };
struct CUIStateDef : CParentDefClassBase {         // vtable 0x0125871C
    unsigned long   GraphicIndex;                  // +0x3c
    unsigned long   StateChangeFlag;               // +0x40
    long            StateChangeType;               // +0x44 EStateChangeType
    C2DVector       Position;                      // +0x48
    C2DVector       Zoom;                          // +0x50
    CRGBFloatColour Colour;                        // +0x58
    float           UpdateTime;                    // +0x68
    CRetailVector<unsigned long> ChildrenNotAffected;   // +0x6c
    bool            LinearChange;                  // +0x78
    virtual void v00();
    CUIStateDef();
};
CUIStateDef::CUIStateDef()
{
    GraphicIndex = 0;
    Position.X = 0.0f;
    Position.Y = 0.0f;
    StateChangeType = 0;
    Zoom.X = 1.0f;
    Zoom.Y = 1.0f;
    Colour.R = 1.0f;
    Colour.G = 1.0f;
    Colour.B = 1.0f;
    Colour.A = 1.0f;
    UpdateTime = -1.0f;
    StateChangeFlag = 7;
    LinearChange = false;
}
