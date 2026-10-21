// CTCLight::OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) -- an active light draws a light at every light helper and a flare at every flare helper of the loaded appearance; with no light helper drawn it draws one light at the thing (lifted 1.0 and pushed along the physics offset for a class-2 thing)
// Grade: DIFFER(528v490) -- the fallback-position copy (thing position lifted 1.0, plus the physics offset for a class-2 thing) is not tail-merged into one DrawLight call
// Leaf: OnAppearanceDraw
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CCharString { void* PStringData; };
extern const CCharString g_LightHelperName;             // 0x013BACC8 (the light helper-name string object)
extern const CCharString g_FlareHelperName;             // 0x013BACCC (the flare helper-name string object)

namespace NGraphicAppearance {
struct CGraphicAppearanceDrawInfo {
    float Interpolate;                                  // +0x00
};
struct CTCGraphicAppearance {
    bool IsResourcesLoaded(bool flag);                  // 0x004BE3E0
    long GetNoDummyObjects(const CCharString& name, float scale);   // 0x004BDE90
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float interpolate);   // 0x004C0560
};
}
using NGraphicAppearance::CTCGraphicAppearance;
using NGraphicAppearance::CGraphicAppearanceDrawInfo;

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual C3DVector GetInterpolatedVelocity(float t); // slot 69 (+0x114) (name descriptive)
    void*     m_tcBase[2];                              // +0x04 CTCBase body
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    C3DVector GetInterpolatedPosition(float t) { return (Position - OldPosition) * t + OldPosition; }
};
class CThing {
public:
    unsigned char   _pad00[0x10];
    unsigned char   ThingClass;                         // +0x10
    unsigned char   _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    long    m_baseWord;                                 // +0x08
};
struct CAppearanceListener {                            // secondary base at +0x0c
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);   // slot 0
    virtual void OnAppearanceClear();                   // slot 1
};
struct CTCLight : CTCBase, CAppearanceListener {
    void*    PLightInfo[2];                             // +0x10
    const void* PLightDef;                              // +0x18
    bool     Active;                                    // +0x1c
    void DrawLight(long index, const C3DVector& position, CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);   // 0x006414A0
    void DrawFlare(long index, const C3DVector& position, CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);   // 0x00641AE0
    __forceinline C3DVector GetLightPosition(const CGraphicAppearanceDrawInfo& info) const
    {
        if (ParentThing->ThingClass == 2) {
            C3DVector offset = ParentThing->PhysicsTC->GetInterpolatedVelocity(info.Interpolate) * 0.35f;
            C3DVector position = ParentThing->PhysicsTC->GetInterpolatedPosition(info.Interpolate) + offset;
            C3DVector lifted(position.X, position.Y, position.Z + 1.0f);
            return lifted;
        }
        return ParentThing->GetPosition();
    }
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
};

bool CTCLight::OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info)
{
    if (Active) {
        if (!appearance.IsResourcesLoaded(true))
            return false;
        long lightsDrawn = 0;
        long lights = appearance.GetNoDummyObjects(g_LightHelperName, 1.0f);
        for (long i = 0; i < lights; ++i) {
            C3DVector position;
            if (appearance.GetHelperPoint(g_LightHelperName, i, 0, &position, info.Interpolate)) {
                DrawLight(i, position, appearance, info);
                ++lightsDrawn;
            }
        }
        long flares = appearance.GetNoDummyObjects(g_FlareHelperName, 1.0f);
        for (long i = 0; i < flares; ++i) {
            C3DVector position;
            if (appearance.GetHelperPoint(g_FlareHelperName, i, 0, &position, info.Interpolate))
                DrawFlare(i, position, appearance, info);
        }
        if (lightsDrawn == 0) {
            C3DVector position;
            if (ParentThing->ThingClass == 2) {
                C3DVector offset = ParentThing->PhysicsTC->GetInterpolatedVelocity(info.Interpolate) * 0.35f;
                C3DVector moved = ParentThing->PhysicsTC->GetInterpolatedPosition(info.Interpolate) + offset;
                position = C3DVector(moved.X, moved.Y, moved.Z + 1.0f);
            } else {
                position = ParentThing->GetPosition();
            }
            DrawLight(0, C3DVector(position.X, position.Y, position.Z), appearance, info);
        }
    }
    return true;
}
