// NUISystem::CText::CText(long id) -- construct a UI text component: draggable base, empty value, no font, fresh primitive handles, then read the def
// Leaf: ??0CText@NUISystem@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054F5C0, 131 bytes. __thiscall, `ret 4`.
// Callees: NUISystem::CDraggableInto::CDraggableInto(long) 0x0052CC50; CWideString::CWideString() 0x0099AED0 (catalog
//          "CCharString" is a byte twin); CBaseClass::CBaseClass() 0x0099A2D0 (x2); NUISystem::CText::GetDataFromDef()
//          0x0054ED90. Vtables (RTTI .?AVCText@NUISystem@@) 0x01249CCC / 0x01249CA4 / 0x01249C9C at +0 / +4 / +0x18;
//          CEnginePrimitiveHandle vtable 0x0122F598 (render-layer handle; see owner note on graphics replacement).
// Shape: the three vtable stores, then members in declaration order (Value ctor, PFont zeroed through one `xor ebx,ebx`,
//        Handle and HandleDropShadowBottomRight each = out-of-line CBaseClass ctor + own vtable + null Data,
//        NoRenderNextFrame = 0); the body calls GetDataFromDef() and only then clears Unavailable.
// Layout: retail = Ego_r.pdb minus 0x30 (retail CDraggableInto is 0x15C bytes): Value +0x15C, PFont +0x160 (Data) / +0x164
//         (Info), Handle +0x168, HandleDropShadowBottomRight +0x170, Window (C2DBoxF) +0x178, ScaleText +0x188,
//         Unavailable +0x189, NoRenderNextFrame +0x18A (PDB +0x18C/+0x190/+0x198/+0x1A0/+0x1A8/+0x1B8/+0x1B9/+0x1BA).
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CEnginePersistentPrimitiveBase;
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
struct CWideString {
    void* PStringData;
    CWideString();                                 // 0x0099AED0
};
struct CCPPointerInfo;
struct CFontBank;
template <class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };

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
struct CDraggableInto : CComponentInterface, CDrawableInterface, CDraggableInterface {
    unsigned char _pad1c[0x15c - 0x1c];
    CDraggableInto(long id);                       // 0x0052CC50
};
struct CText : CDraggableInto {
    CWideString            Value;                  // +0x15c
    CCountedPointer<CFontBank> PFont;              // +0x160
    CEnginePrimitiveHandle Handle;                 // +0x168
    CEnginePrimitiveHandle HandleDropShadowBottomRight;   // +0x170
    C2DBoxF                Window;                 // +0x178
    bool                   ScaleText;              // +0x188
    bool                   Unavailable;            // +0x189
    unsigned char          NoRenderNextFrame;      // +0x18a
    CText(long id);
    virtual void v000();
    void GetDataFromDef();                         // 0x0054ED90
};
CText::CText(long id)
    : CDraggableInto(id), NoRenderNextFrame(0)
{
    GetDataFromDef();
    Unavailable = false;
}
}
