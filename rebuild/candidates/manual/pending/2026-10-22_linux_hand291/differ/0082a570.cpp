// Grade: DIFFER(788v792) -- register plan: retail keeps `this` in ebx and the world map in ebp and spills the cached CWorld* to a frame word (0xa0 frame), here the world sits in ebp and `this` is spilled (0xa4 frame), which shifts every frame offset; the sprite-box x87 sequence (top stored over drawPos.y, left kept on the FPU stack, the uv default-argument zeros stored late) and the GetDrawPosition copy also differ. 4 grades (C2DVector copy ctor, in-place drawPos, const world-map pointer, inline fade accessor); behaviourally complete (visibility/region test, rotation from the camera or driver slot 63, pulse scale, layer 0x43/0x44/0x45, ToScreenBox + 2D sprite + SetPersistentPrimitive)
// CMiniMapDisplay::DrawMarker(CEnginePrimitivePrepareInfo& info, const CMarkerInfo& marker, long index, const C2DBoxF& box, unsigned char alpha, bool front)
// Leaf: DrawMarker
struct C2DVector {
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
    C2DVector(const C2DVector& other) : x(other.x), y(other.y) { }
};
struct C3DVector { float X, Y, Z; };
struct C2DBoxF {
    C2DVector TopLeft, BottomRight;
    C2DBoxF(float left, float top, float right, float bottom) : TopLeft(left, top), BottomRight(right, bottom) { }
    C2DBoxF(const C2DBoxF& other) : TopLeft(other.TopLeft), BottomRight(other.BottomRight) { }
};
C2DBoxF __fastcall ToScreenBox(const C2DBoxF& box);   // 0x00643BEF
struct CEnginePrimitive2DSprite {                  // 0x54 bytes
    unsigned char _data[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long graphic,
        unsigned long frame, long blend, long interpolation, float a, float b, unsigned char alpha,
        unsigned char red, unsigned char green, unsigned char blue, unsigned long flags,
        const C2DVector& uv0 = C2DVector(0.0f, 0.0f), const C2DVector& uv1 = C2DVector(0.0f, 0.0f));   // 0x0041BEB0
};
struct CEnginePrimitiveHandle { unsigned char _data[8]; };
struct CIEngine {
    virtual void e00(); virtual void e01(); virtual void e02(); virtual void e03(); virtual void e04(); virtual void e05(); virtual void e06(); virtual void e07();
    virtual void e08(); virtual void e09(); virtual void e10(); virtual void e11(); virtual void e12(); virtual void e13(); virtual void e14(); virtual void e15();
    virtual void e16(); virtual void e17(); virtual void e18(); virtual void e19(); virtual void e20(); virtual void e21(); virtual void e22();
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive2DSprite& primitive, unsigned long flags);   // slot 23 (+0x5C)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                          // +0x34
};
struct CGraphicBank {
    virtual void g00(); virtual void g01(); virtual void g02(); virtual void g03(); virtual void g04(); virtual void g05(); virtual void g06(); virtual void g07();
    virtual void g08(); virtual void g09(); virtual void g10(); virtual void g11(); virtual void g12(); virtual void g13(); virtual void g14(); virtual void g15();
    virtual void g16(); virtual void g17(); virtual void g18(); virtual void g19(); virtual void g20();
    virtual unsigned long GetWidth(long graphic, long frame);    // slot 21 (+0x54)
    virtual unsigned long GetHeight(long graphic, long frame);   // slot 22 (+0x58)
};
struct CCamera {
    unsigned char _pad00[4];
    C3DVector Forward;                             // +0x04
};
float Math_ComputeAngleXY_this(const C3DVector* v);
struct C3DVectorAngle : C3DVector { float ComputeAngleXY() const; };   // 0x00A0C380
struct CCameraPointer {
    CCamera* P;
    CCameraPointer();
};
struct CCameraManager {
    virtual void c00(); virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04();
    virtual void c05(); virtual void c06(); virtual void c07(); virtual void c08(); virtual void c09();
    virtual CCameraPointer GetCurrentCamera();     // slot 10 (+0x28)
};
struct CWorldMap {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14(); virtual void m15();
    virtual long GetMapAt(const C3DVector& pos);   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map);         // 0x004FC190
    long GetCurrentRegionNumber();                 // 0x004FB150
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13
    unsigned char _pad04[0x30];
    CCameraManager* CameraManager;                 // +0x34
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld* World;                                 // +0x24
    unsigned char _pad28[0x161c8 - 0x28];
    CGraphicBank* GraphicBank;                     // +0x161C8
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CDriver {
    virtual void d00();
    float GetFacingAngle();
};
struct CThing {
    unsigned char _pad00[0x60];
    struct CThingDriver* Driver;                   // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char Flags6c;                         // +0x6C
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char Flags91;                         // +0x91
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CThingDriver {
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07();
    virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15();
    virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23();
    virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30(); virtual void t31();
    virtual void t32(); virtual void t33(); virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39();
    virtual void t40(); virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46(); virtual void t47();
    virtual void t48(); virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54(); virtual void t55();
    virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62();
    virtual float GetAngleXY();                    // slot 63 (+0xFC)
};
struct CIntelligentPointer {
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CFadeInFadeOutBase {
    unsigned char _data[0x1c];
    float GetFadeInOutAlpha();                     // 0x004348D0
};
struct CMarkerInfo {
    CIntelligentPointer Thing;                     // +0x00
    unsigned char _pad04[4];
    long Graphic;                                  // +0x08
    bool IsRotatedOnMap() const;                   // 0x0082A230
    bool IsPulsingOnMap() const;                   // 0x0082A270
    bool IsReversedOnMap() const;                  // 0x0082A3E0
};

struct CMiniMapDisplay {
    void*                   __vftable;
    CEnginePrimitiveHandle* MapPrimitiveHandles[3];       // +0x04
    CEnginePrimitiveHandle* MarkerPrimitiveHandles[3];    // +0x10
    long                    MiniMapGraphic;               // +0x1C
    void*                   Markers;                      // +0x20
    CFadeInFadeOutBase      MarkerFlashFade;              // +0x24
    bool IsPositionInsideClipEdges(const C3DVector& pos) const;              // 0x00829D20
    C2DVector GetDrawPosition(const C3DVector& pos, const C2DBoxF& box);     // 0x0082A020
    bool DrawMarker(CEnginePrimitivePrepareInfo& info, const CMarkerInfo& marker, long index, const C2DBoxF& box,
                    unsigned char alpha, bool front);
};

bool CMiniMapDisplay::DrawMarker(CEnginePrimitivePrepareInfo& info, const CMarkerInfo& marker, long index,
                                 const C2DBoxF& box, unsigned char alpha, bool front)
{
    CWorld* world = g_pMainGameComponent->World;
    CWorldMap* worldMap = world->GetWorldMap();
    float angle = 0.0f;
    bool fixedMarker = false;
    CThing* thing = marker.Thing.GetPItem();
    if (thing && !(thing->Flags91 & 0x21)) {
        C3DVector pos = thing->GetPosition();
        if (thing->Flags6c & 0x20)
            fixedMarker = true;
        else if (!IsPositionInsideClipEdges(pos))
            return false;
        if (worldMap->GetRegionNumberMapIsIn(worldMap->GetMapAt(pos)) != worldMap->GetCurrentRegionNumber())
            return false;
        if (thing->Driver) {
            if (marker.IsRotatedOnMap()) {
                if (fixedMarker) {
                    CCameraPointer camera = world->CameraManager->GetCurrentCamera();
                    angle = 1.0f - static_cast<const C3DVectorAngle&>(camera.P->Forward).ComputeAngleXY();
                } else
                    angle = 1.0f - thing->Driver->GetAngleXY();
            }
            if (marker.IsReversedOnMap())
                angle += 0.5f;
        }
        C2DVector drawPos = GetDrawPosition(pos, box);
        long layer = 0x44;
        float scaleX = 1.0f;
        float scaleY = 1.0f;
        if (!fixedMarker) {
            layer = 0x45;
            scaleX = (box.BottomRight.x - box.TopLeft.x) * 0.00390625f;
            scaleY = (box.BottomRight.y - box.TopLeft.y) * 0.00390625f;
        }
        if (marker.IsPulsingOnMap()) {
            scaleX = (MarkerFlashFade.GetFadeInOutAlpha() + 0.5f) * scaleX;
            scaleY = (MarkerFlashFade.GetFadeInOutAlpha() + 0.5f) * scaleY;
        }
        if (front)
            layer = 0x43;
        CGraphicBank* bank = g_pMainGameComponent->GraphicBank;
        scaleX = (float)bank->GetWidth(marker.Graphic, 0) * scaleX;
        scaleY = (float)bank->GetHeight(marker.Graphic, 0) * scaleY;
        float left = drawPos.x - scaleX * 0.5f;
        drawPos.y = drawPos.y - scaleY * 0.5f;
        C2DBoxF spriteBox(left, drawPos.y, left + scaleX, drawPos.y + scaleY);
        CEnginePrimitive2DSprite sprite(0, ToScreenBox(spriteBox), layer, marker.Graphic, 0, 2, 0, angle, angle, alpha,
            0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(MarkerPrimitiveHandles[0][index], sprite, 0xc0);
    }
    return fixedMarker;
}
