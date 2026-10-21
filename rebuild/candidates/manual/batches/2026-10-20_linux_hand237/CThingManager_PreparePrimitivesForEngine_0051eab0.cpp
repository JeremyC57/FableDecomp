// CThingManager::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) -- prepare every thing on the render list (bullet-time things get a rescaled copy of the info) and drop the ones no longer rendered
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051EAB0, length 276 bytes (+ int3 padding), `ret 4`.
// Callees: CCamera copy constructor @ 0x0049D380 (out of line), CBulletTime::ConvertInterpolateToBulletTimeInterpolate
// @ 0x004C6010 (landed), CThing::IsRenderedThisFrame @ 0x004C78F0 (landed), CThing::IsAffectedByBulletTime @ 0x004C9920,
// CThing virtual slot 36 (+0x90, PreparePrimitivesForEngine), CThingManager::RemoveFromRenderList @ 0x0051E000 (landed).
// Globals: 0x013B8A14 the primitive force-resend latch (landed name g_LatchForceResendPrimitives, read by
// CTCGraphicAppearance::PrepareForRenderIfNeeded 0x004BE490), set from FirstRenderOfFrame.
// Layout (retail CThingManager, as the landed RemoveFromRenderList / AllocateNewThingUID rows): PWorld +0x24,
// FirstRenderOfFrame +0x56, RenderList head +0x84, UpdateAndRenderAllFlag +0x99; CWorld bullet time +0x68 (active byte
// +0x0c, controller +0x04, the landed IsActive() model); CThing render next +0x80, Dead bit +0x91.0.
// CEnginePrimitivePrepareInfo (Ego_r.pdb, 0x74 bytes = retail): Interpolate (13 dwords, rep movsd), Engine +0x34,
// Edit2DEngine +0x38, Camera +0x3c (CCamera, user copy ctor) -- the implicit copy is the frame block at esp+14h.
// Shape: the bullet-time flag is a named bool in frame byte 3; the Current/Last WFInterpolate of the copy are rescaled.
// The bullet-time object is reached through an inline GetBulletTime() (direct `PWorld->m_pBulletTime` loads the
// argument first and lets VC cross-jump the two virtual calls into one).
#pragma pack(push, 4)
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
class CCamera {
public:
    C3DVector       WorldPosition;
    CRightHandedSet RHSet;
    bool            HeightLocked;
    bool            ViewVecZLocked;
    unsigned long   FOVFlags;
    float           HorizontalFOV;
    float           VerticalFOV;
    float           Zoom;
    CCamera(const CCamera& other);                             // 0x0049D380
};
struct CInterpolationInfoSet {
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};
struct CInterpolationInfo {
    CInterpolationInfoSet Current;
    CInterpolationInfoSet Last;
    CInterpolationInfoSet PausedCurrent;
    CInterpolationInfoSet PausedLast;
    CInterpolationInfoSet BulletTimeCurrent;
    CInterpolationInfoSet BulletTimeLast;
    long                  WFServerCurrentTime;
};
class CIEngine;
struct CEnginePrimitivePrepareInfo {
    CInterpolationInfo Interpolate;                            // +0x00
    CIEngine*          Engine;                                 // +0x34
    bool               Edit2DEngine;                           // +0x38
    CCamera            Camera;                                 // +0x3c
};

class CBulletTime {
public:
    unsigned char _pad00[4];
    void*         m_pController;                               // +0x04
    unsigned char _pad08[4];
    bool          m_active;                                    // +0x0c
    float ConvertInterpolateToBulletTimeInterpolate(float interpolate);   // 0x004C6010
    bool IsActive() const { return m_active || m_pController != 0; }
};
class CWorld {
public:
    unsigned char _pad00[0x68];
    CBulletTime*  m_pBulletTime;                               // +0x68
};

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info);   // slot 36 (+0x90)
    unsigned char _pad04[0x80 - 0x04];
    CThing*       m_pNextRender;                               // +0x80
    unsigned char _pad84[0x91 - 0x84];
    unsigned char Dead : 1;                                    // +0x91 bit 0
    bool IsRenderedThisFrame();                                // 0x004C78F0
    bool IsAffectedByBulletTime();                             // 0x004C9920
};

struct ThingLList {
    CThing* m_pHead;
    CThing* m_pTail;
};

class CThingManager {
public:
    void*         __vftable;                                   // +0x00
    unsigned char _pad04[0x24 - 0x04];
    CWorld*       PWorld;                                      // +0x24
    unsigned char _pad28[0x56 - 0x28];
    bool          FirstRenderOfFrame;                          // +0x56
    unsigned char _pad57[0x84 - 0x57];
    ThingLList    RenderList;                                  // +0x84
    unsigned char _pad8c[0x99 - 0x8c];
    bool          UpdateAndRenderAllFlag;                      // +0x99
    void RemoveFromRenderList(CThing* thing);                  // 0x0051E000
    CBulletTime* GetBulletTime() { return PWorld->m_pBulletTime; }
    void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info);
};
#pragma pack(pop)

extern bool g_LatchForceResendPrimitives;                      // 0x013B8A14

void CThingManager::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info)
{
    g_LatchForceResendPrimitives = FirstRenderOfFrame;
    CEnginePrimitivePrepareInfo bulletTimeInfo = info;
    bool bulletTime;
    if (!GetBulletTime()->IsActive()) {
        bulletTime = false;
    } else {
        bulletTime = true;
        bulletTimeInfo.Interpolate.Current.WFInterpolate =
            GetBulletTime()->ConvertInterpolateToBulletTimeInterpolate(bulletTimeInfo.Interpolate.Current.WFInterpolate);
        bulletTimeInfo.Interpolate.Last.WFInterpolate =
            GetBulletTime()->ConvertInterpolateToBulletTimeInterpolate(bulletTimeInfo.Interpolate.Last.WFInterpolate);
    }
    CThing* thing = RenderList.m_pHead;
    while (thing) {
        CThing* next = thing->m_pNextRender;
        if (!thing->Dead && (thing->IsRenderedThisFrame() || UpdateAndRenderAllFlag)) {
            if (bulletTime && thing->IsAffectedByBulletTime())
                thing->PreparePrimitivesForEngine(bulletTimeInfo);
            else
                thing->PreparePrimitivesForEngine(info);
            if (!thing->IsRenderedThisFrame() && !UpdateAndRenderAllFlag)
                RemoveFromRenderList(thing);
        } else {
            RemoveFromRenderList(thing);
        }
        thing = next;
    }
    FirstRenderOfFrame = false;
}
