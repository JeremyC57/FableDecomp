// NCameraModes::CSnapToPoint::CSnapToPoint(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point) -- the camera-mode root (0x0084E420), the inline CCameraPointModeBase (vtable, PCameraPoint built from `point`), this class's vtable, TrackThing = true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056E330, length 78 bytes (80 with padding).
// Leaf: ??0CSnapToPoint
// Callees: 0x0084E420 (the NCameraModes::CBase constructor, two arguments; its manifest label
// CActionParam_Constructor is garbage), 0x00A01B10 (CBaseIntelligentPointer::CBaseIntelligentPointer),
// 0x00A01B90 (CBaseIntelligentPointer::SetPItem). Vtables (rtti-vtables.tsv): 0x0124DDDC
// NCameraModes::CCameraPointModeBase, 0x01237F1C CIntelligentPointer, 0x0124DE5C NCameraModes::CSnapToPoint.
// Layout: the PDB puts PCameraPoint at +0xc8 and TrackThing at +0xe4; the retail root base is 0x24 bytes
// narrower, so PCameraPoint sits at +0xa4 and TrackThing at +0xc0 (the stored offsets).
// Shape: the root constructor with the first two arguments, the intermediate base's vtable store (an inline
// constructor of a polymorphic, NOT novtable, base: the member constructor call keeps it alive), the
// CIntelligentPointer member's converting constructor inlined (base ctor, its vtable, SetPItem(point)), the
// derived vtable, the flag byte from the initialiser list, `ret 0Ch`.
struct CGameCameraManager;
struct CCameraModeDef;
struct CBaseObject;
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {     // CIntelligentPointer<CThing const>
    virtual void v00();
    CIntelligentPointer(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct C3DVector { float x, y, z; };

namespace NCameraModes {

struct CBase {
    virtual void v00();
    unsigned char _body[0xa0];                  // +0x04..+0xa3 (retail; the PDB body is 0x24 bytes wider)
    CBase(CGameCameraManager* manager, const CCameraModeDef* def);   // 0x0084E420
};

struct CCameraPointModeBase : CBase {
    CIntelligentPointer PCameraPoint;           // +0xa4
    C3DVector           Pos;                    // +0xac
    float               FOV;                    // +0xb8
    bool                CutInto;                // +0xbc
    bool                CutOutOf;               // +0xbd
    virtual void v00();
    CCameraPointModeBase(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point)
        : CBase(manager, def), PCameraPoint(point) { }
};

struct CSnapToPoint : CCameraPointModeBase {
    bool      TrackThing;                       // +0xc0
    C3DVector LookDirection;                    // +0xc4
    virtual void v00();
    CSnapToPoint(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point);
};

CSnapToPoint::CSnapToPoint(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point)
    : CCameraPointModeBase(manager, def, point), TrackThing(true)
{
}

}
