// NUISystem::CSprite::CSprite(const CSprite& other) -- copy constructor: copy the changing-state component base, then start a fresh sprite (empty primitive handle, default flags, no texture) and reload its data from the def
// Leaf: ??0CSprite@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe @ 0x0041B870, 106 bytes, __fastcall `ret 4`.
// Identity (proven): stores CSprite's three vtables 0x0122F5D4 / 0x0122F5AC / 0x0122F5A4 (RTTI NUISystem::CSprite) after
// calling CChangingStateComponent's copy constructor 0x0052CCA0 (it stores the CChangingStateComponent vtables
// 0x01245DE4 / 0x01245DBC); Ego_r.pdb / FableWin.pdb hold exactly two CSprite constructors, (long) = 0x0041B800 (same
// body over ??0CChangingStateComponent@NUISystem@@QAE@J@Z 0x0052CC50) and this ??0CSprite@NUISystem@@QAE@ABV01@@Z.
// Callers: the copy constructors of its derived classes (CListArrow 0x0054C200 passes its own argument through), plus
// CSlider / CMorphingSprite / CMouseCursor constructors.
// Callees: CChangingStateComponent(const CChangingStateComponent&) @ 0x0052CCA0, CBaseClass::CBaseClass() @ 0x0099A2D0
// (the handle's root), CSprite::GetDataFromDef() @ 0x0041AC20.
// Shape: a user copy constructor that copies only the base: Handle is default-constructed (inline
// CEnginePrimitiveHandle(): root ctor call, vtable 0x0122F598, Data = 0), NoRenderNextFrame = false, Sprite2DFlag = 2,
// GraphicIndex = 0, PTexture = {0, 0}, then GetDataFromDef(). Angle / Width / Height are left to the def load.
// Layout: retail CChangingStateComponent is 0x15C bytes (RTTI bases CUIInterface @0, CObserver @4, CActionParam @0x18);
// CSprite members at Ego_r.pdb offset - 0x30 (STLport containers in the bases): Handle +0x15C, Angle +0x164, Width
// +0x168, Height +0x16C, NoRenderNextFrame +0x170, Sprite2DFlag +0x174, GraphicIndex +0x178, PTexture +0x17C; size 0x184.
// engine/CSprite.h is an all-padding flattened header (no vptrs, no names), so the class is modelled locally as a
// polymorphic partial view like the landed CListArrow constructor 0x0054C1D0 (NOTES.md "Header debt").
class CTexture;
class CCPPointerInfo;
class CEnginePersistentPrimitiveBase;

class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();
};

template <class T> class CIVCountedPointer {
public:
    T* Data;
    CIVCountedPointer() : Data(0) { }
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass {          // vtable 0x0122F598
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    CEnginePrimitiveHandle() { }
    virtual ~CEnginePrimitiveHandle();
};

namespace NUISystem {
class CUIInterface {                                        // RTTI base @0 (primary vtable)
public:
    virtual ~CUIInterface();
};
class CObserver {                                           // RTTI base @4, 0x14 bytes
public:
    virtual ~CObserver();
    unsigned char EventsToObserve[0xc];                     // +0x04 set<NUISystem::EEvent> (STLport, 12 bytes)
    bool PreventObservation;                                // +0x10
};
class CActionParam {                                        // RTTI base @0x18, 8 bytes
public:
    virtual ~CActionParam();
    long ActionParamType;                                   // +0x04 EActionParamType
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {   // partial view
public:
    CComponent(const CComponent& other);
    unsigned char ComponentBody[0x15c - 0x20];              // CComponent / CChangingStateComponent members (not modelled)
};
class CChangingStateComponent : public CComponent {         // partial view, 0x15C bytes
public:
    CChangingStateComponent(const CChangingStateComponent& other);   // 0x0052CCA0
};
class CSprite : public CChangingStateComponent {
public:
    CEnginePrimitiveHandle Handle;                          // +0x15C
    float Angle;                                            // +0x164
    unsigned long Width;                                    // +0x168
    unsigned long Height;                                   // +0x16C
    unsigned char NoRenderNextFrame;                        // +0x170
    long Sprite2DFlag;                                      // +0x174 EEngineSprite2DFlag
    unsigned long GraphicIndex;                             // +0x178
    CCountedPointer<CTexture> PTexture;                     // +0x17C
    CSprite(const CSprite& other);
    virtual ~CSprite();
    void GetDataFromDef();                                  // 0x0041AC20
};

CSprite::CSprite(const CSprite& other)
    : CChangingStateComponent(other),
      NoRenderNextFrame(0),
      Sprite2DFlag(2),
      GraphicIndex(0)
{
    GetDataFromDef();
}
}