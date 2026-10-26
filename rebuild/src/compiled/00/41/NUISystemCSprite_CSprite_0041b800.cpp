// NUISystem::CSprite::CSprite(long id) -- the sprite component: the draggable base, a null primitive handle, rendering on, the default 2D-sprite flag (2), no graphic or texture yet; then the def data is read
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041B800, 106 bytes; `ret 4`. Callees: NUISystem::CDraggableInto::CDraggableInto(long) @ 0x0052CC50,
//          CBaseClass::CBaseClass() @ 0x0099A2D0, NUISystem::CSprite::GetDataFromDef() @ 0x0041AC20.
//          Vtables 0x0122F5D4 / 0x0122F5AC / 0x0122F5A4 (CSprite at +0, +4, +0x18), CEnginePrimitiveHandle 0x0122F598.
// Layout: Ego_r.pdb CSprite minus 0x30 (the retail CDraggableInto is 0x15c bytes, as the landed CMovie ctor 0x00548350):
//         Handle +0x15c, Angle +0x164, Width +0x168, Height +0x16c, NoRenderNextFrame +0x170, Sprite2DFlag +0x174,
//         GraphicIndex +0x178, PTexture +0x17c (counted pointer: Data, Info).
// Shape: three vtable stores, then the members in declaration order sharing one zero register; Angle/Width/Height are
//        left alone; GetDataFromDef is the body.
// Leaf: ??0CSprite@NUISystem@@QAE@J@Z
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};
class CEnginePersistentPrimitiveBase;
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    CEnginePrimitiveHandle() { }
};
class CTexture;
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
enum EEngineSprite2DFlag { SPRITE_2D_DEFAULT = 2 };
namespace NUISystem {
struct CComponentInterface {
    virtual void v000();
};
struct CDrawableInterface {
    virtual void v000();
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000();
};
class CDraggableInto : public CComponentInterface, public CDrawableInterface, public CDraggableInterface { public:
    unsigned char _pad1c[0x15c - 0x1c];
    CDraggableInto(long id);                       // 0x0052CC50
};
class CSprite : public CDraggableInto { public:
    CEnginePrimitiveHandle    Handle;              // +0x15c
    float                     Angle;               // +0x164
    unsigned long             Width;               // +0x168
    unsigned long             Height;              // +0x16c
    unsigned char             NoRenderNextFrame;   // +0x170
    EEngineSprite2DFlag       Sprite2DFlag;        // +0x174
    unsigned long             GraphicIndex;        // +0x178
    CCountedPointer<CTexture> PTexture;            // +0x17c
    virtual void v000();
    CSprite(long id);
    void GetDataFromDef();                         // 0x0041AC20
};

CSprite::CSprite(long id)
    : CDraggableInto(id), NoRenderNextFrame(0), Sprite2DFlag(SPRITE_2D_DEFAULT), GraphicIndex(0)
{
    GetDataFromDef();
}
}