// NCameraModes::CTrack::CTrack(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point) -- the camera-mode root (0x0084E420), the inline CCameraPointModeBase (vtable, PCameraPoint built from `point`), this class's vtable, ZTargeting = false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069DF80, length 78 bytes (80 with padding).
// Leaf: ??0CTrack
// The chunk label (CSnapToPoint) is propagated from the byte-similar 0x0056E330: the final vtable stored
// here, 0x0125C50C, is NCameraModes::CTrack (rtti-vtables.tsv), and the flag cleared is +0xc8 (PDB CTrack:
// PTrack +0xe4, CurrentFractionAlongTrack +0xe8, ZTargeting +0xec; retail 0x24 bytes lower).
// Callees: 0x0084E420 (the NCameraModes::CBase constructor, two arguments), 0x00A01B10
// (CBaseIntelligentPointer::CBaseIntelligentPointer), 0x00A01B90 (CBaseIntelligentPointer::SetPItem).
// Vtables: 0x0124DDDC NCameraModes::CCameraPointModeBase, 0x01237F1C CIntelligentPointer, 0x0125C50C CTrack.
// Shape: as CSnapToPoint (0x0056E330): root constructor, the intermediate base's surviving vtable store, the
// inlined CIntelligentPointer converting constructor, the derived vtable, the flag from the initialiser
// list; PTrack and the fraction are left alone. `ret 0Ch`.
class CGameCameraManager;
class CCameraModeDef;
class CBaseObject;
class CThing;

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

class C3DVector { public: float x, y, z; };

namespace NCameraModes {

class CBase { public:
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

class CTrackPath;                               // the track object (PDB `CTrack *`)

struct CTrack : CCameraPointModeBase {
    CTrackPath* PTrack;                         // +0xc0
    float       CurrentFractionAlongTrack;      // +0xc4
    bool        ZTargeting;                     // +0xc8
    virtual void v00();
    CTrack(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point);
};

CTrack::CTrack(CGameCameraManager* manager, const CCameraModeDef* def, const CThing* point)
    : CCameraPointModeBase(manager, def, point), ZTargeting(false)
{
}

}