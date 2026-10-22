// CAtmosProcess::UpdateInsideBuildings() -- take the reverb environment from the current region's def; every fourth frame test whether the camera is inside a building near it (whose reverb then wins) and record it; when the inside flag changes, cross-fade the four atmos sound groups and touch every SOUND_THEME def; when the reverb changed, store it and reload the themes
// Grade: DIFFER(811v811) -- every instruction right except frame layout: retail reserves 0x7c (ours 0x44; 0x38 more, likely dead inline-helper temporaries), keeps the camera-position return temporary above the search box (+0x54 v +0x2c) and the checked bool at +0x13 (sharing the empty filter's byte) v +0x12
// Leaf: UpdateInsideBuildings
#include <stdlib.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct C2DCoordI { long X, Y; };
struct C2DBoxI { C2DCoordI TopLeft; C2DCoordI BottomRight; };

template <class T> struct vector {              // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

struct CDefPointeeBase {
    virtual void Destroy0() const;
    virtual void Destroy() const;               // slot 1 (+0x04)
    mutable long RefCount;                      // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};
// Layout: retail region def reverb type +0x7c, level +0x80; sound-setup def float +0x138.
struct CRegionDef : CDefPointeeBase {
    unsigned char _pad08[0x7c - 0x08];
    long ReverbType;                            // +0x7c
    float ReverbLevel;                          // +0x80
};
struct CSoundThemeDef : CDefPointeeBase { };
struct CSoundSetupDef {
    unsigned char _pad00[0x138];
    float InsideDamping;                        // +0x138
};
struct CRegion {                                // 0x58-byte entry
    unsigned char _pad00[0x24];
    CDefPointer<const CRegionDef> Def;          // +0x24
};
struct CRegionList {
    long GetCurrentRegionIndex();               // 0x004FB150
    CRegion* GetRegionWithIndex(long index);    // 0x004FB490
};
struct CDefinitionManager {
    unsigned char _pad00[0xd0];
    CSoundSetupDef* SoundSetup;                 // +0xd0
    static CDefinitionManager& __fastcall Get();                    // 0x0044C6B0
    long GetNoDefsOfType(const CCharString& type);                  // 0x009AD3B0
    bool GetDef(const CCharString& type, long index, CDefPointer<const CSoundThemeDef>& out);   // 0x006B25E0
};

const CSoundThemeDef* FindSoundTheme_006b25e0(CDefinitionManager* manager, const CCharString* type, long index);
// Same-TU stand-in for GetDef<CSoundThemeDef> 0x006B25E0 (not a reconstruction): it only fills its out-parameter.
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& type, long index, CDefPointer<const CSoundThemeDef>& out)
{
    out.Object = FindSoundTheme_006b25e0(this, &type, index);
    return true;
}
#pragma auto_inline(on)

class CThing;
class CThingBuilding {
public:
    bool IsPointInside(const C3DVector& point);                     // 0x0082D000
    void GetReverbEnvironment(long* type, float* level);            // 0x0082D8C0
};
struct CThingFilter_IsBuilding { };
struct CThingSearchTools {
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsBuilding& filter, vector<CThingBuilding*>& out);   // 0x006B2720
};
bool SearchRoughArea_006b2720(CThingSearchTools* tools, const C2DBoxI* area, const CThingFilter_IsBuilding* filter,
                              CThingBuilding** first, CThingBuilding** last, CThingBuilding** end);
CThingBuilding** SearchResultFirst_006b2720();
CThingBuilding** SearchResultLast_006b2720();
CThingBuilding** SearchResultEnd_006b2720();
// Same-TU stand-in for the GetAllThingsInRoughArea<IsBuilding> instantiation 0x006B2720 (not a reconstruction of it, as
// the landed 0x00726BE0): the result vector's words go by value so the vector does not escape.
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsBuilding& filter,
                                                                     vector<CThingBuilding*>& out)
{
    bool found = SearchRoughArea_006b2720(this, &area, &filter, out._M_start, out._M_finish, out._M_end_of_storage);
    out._M_start = SearchResultFirst_006b2720();
    out._M_finish = SearchResultLast_006b2720();
    out._M_end_of_storage = SearchResultEnd_006b2720();
    return found;
}
#pragma auto_inline(on)
class CCameraBase {
public:
    virtual void v00();
    virtual C3DVector GetPosition(float t);     // slot 1 (+0x04)
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CRegionList* GetRegionList();       // slot 12 (+0x30)
    unsigned char _pad04[0x20 - 0x04];
    CThingSearchTools* SearchTools;             // +0x20
    unsigned char _pad24[0x34 - 0x24];
    CCameraBase* Camera;                        // +0x34
};
struct CMainGameComponent {
    unsigned char _pad[0x24];
    CWorld* World;                              // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long __fastcall GFGetCurrentFrame();        // 0x0049D870

class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void FadeGroup(unsigned long group, float volume, float time);   // slot 46 (+0xb8)
};
extern CSoundSystem* g_pSoundSystem;            // 0x013B8394

class CAtmosProcess {
public:
    long DayNightState;                         // +0x00
    void* AtmosMapVtbl;                         // +0x04 (CMyAtmosMap)
    bool RainCurrentlyPlaying;                  // +0x08
    bool AtmosSoundsMuted;                      // +0x09
    unsigned char PRainPlayer[8];               // +0x0a
    long InsideBuilding;                        // +0x14 (at +0x12 after the pack(1) pointer, aligned to +0x14)
    long ReverbType;                            // +0x18
    float ReverbLevel;                          // +0x1c
    void LoadThemes();                          // 0x006B1600
    void UpdateInsideBuildings();
};

void CAtmosProcess::UpdateInsideBuildings()
{
    CWorld* world = g_pMainGameComponent->World;
    long reverbType = ReverbType;
    float reverbLevel = ReverbLevel;
    const CRegionDef* def = world->GetRegionList()->GetRegionWithIndex(world->GetRegionList()->GetCurrentRegionIndex())->Def.Object;
    if (def) {
        ++def->RefCount;
        reverbType = def->ReverbType;
        reverbLevel = def->ReverbLevel;
    }
    long wasInside = InsideBuilding;
    bool checked = false;
    if (GFGetCurrentFrame() % 4 == 0) {
        checked = true;
        C3DVector pos;
        pos = world->Camera->GetPosition(1.0f);
        vector<CThingBuilding*> buildings;
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI area;
        area.TopLeft.X = x - 3;
        area.TopLeft.Y = y - 3;
        area.BottomRight.X = x + 3;
        area.BottomRight.Y = y + 3;
        CThingFilter_IsBuilding filter;
        world->SearchTools->GetAllThingsInRoughArea(area, filter, buildings);
        long inside = 0;
        for (CThingBuilding** it = buildings._M_start; it != buildings._M_finish; ++it) {
            CThingBuilding* building = *it;
            if (building->IsPointInside(pos)) {
                building->GetReverbEnvironment(&reverbType, &reverbLevel);
                inside = 1;
                break;
            }
        }
        InsideBuilding = inside;
    }
    if (wasInside != InsideBuilding) {
        float damping = CDefinitionManager::Get().SoundSetup->InsideDamping;
        if (InsideBuilding != 0) {
            g_pSoundSystem->FadeGroup(0x800, 1.0f, 1000.0f);
            g_pSoundSystem->FadeGroup(0x1000, 0.0f, 1000.0f);
            g_pSoundSystem->FadeGroup(0x2000, 1.0f, 1000.0f);
            g_pSoundSystem->FadeGroup(0x4000, damping, 1000.0f);
        } else {
            g_pSoundSystem->FadeGroup(0x1000, 1.0f, 1000.0f);
            g_pSoundSystem->FadeGroup(0x800, 0.0f, 1000.0f);
            g_pSoundSystem->FadeGroup(0x4000, 1.0f, 1000.0f);
            g_pSoundSystem->FadeGroup(0x2000, damping, 1000.0f);
        }
        long count;
        {
            CCharString type("SOUND_THEME", -1);
            count = CDefinitionManager::Get().GetNoDefsOfType(type);
        }
        for (long i = 1; i < count; ++i) {
            CDefPointer<const CSoundThemeDef> theme;
            CCharString type("SOUND_THEME", -1);
            CDefinitionManager::Get().GetDef(type, i, theme);
        }
    }
    if (checked) {
        if (reverbType != ReverbType || reverbLevel != ReverbLevel) {
            ReverbType = reverbType;
            ReverbLevel = reverbLevel;
            LoadThemes();
        }
    }
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}
