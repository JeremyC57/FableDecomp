// CWorldMap::Update() -- per-frame world-map update: run the level loader, update every used loaded map, update the destroyed theme areas, set moving the physical things inside each height-changed area (then clear that list), and switch the passability overlay to follow its debug toggle
// Leaf: ?Update@CWorldMap@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FF920, 319 bytes, plain `ret` (virtual: CWorldMap vtable 0x01244AEC slot 27).
// Name: Ego_r.pdb ?Update@CWorldMap@@UAEXXZ (public virtual; the PDB name sets carry no other CWorldMap::Update).
// Callees: PLevelLoader->RunSuspendableProcess() (CLevelLoader vtable 0x0125D9B0 slot 1 = CSuspendableProcessMicrothread::
//   RunSuspendableProcess 0x00A44880), IsMapLoaded (slot 10, +0x28), CMap::Update (protected virtual ?Update@CMap@@MAEXXZ,
//   CMap vtable 0x0127252C slot 13, +0x34), CWorldMap::UpdateDestroyedThemeAreas 0x004FEC80 (?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ),
//   CThingSearchTools::ForEachInRoughArea<CThingPhysical, CThingFilter_None, bool (__fastcall*)(CThingPhysical&)> 0x00518130
//   (Ego_r.pdb ??$ForEachInRoughArea@VCThingPhysical@@VCThingFilter_None@@P6I_NAAV1@@Z@CThingSearchTools@@QBE_NABVC2DBoxI@@
//   ABVCThingFilter_None@@ABQ6I_NAAVCThingPhysical@@@Z@Z: const, bool result, the callback by const reference),
//   CWorldMap::ShowPassabilityOverlay(bool) 0x004FF570.
// Callback 0x004FB260 (`mov ecx,[ecx+60h] ; test ; push 1 ; call` = set the thing's physics body moving): the only
//   `bool __fastcall (CThingPhysical&)` in the PDB name sets is ?CThingPhysical_SetAsMoving@?A0x37019565@@ (an anonymous
//   namespace: TU-local, so the ?A0x hash is per TU) -- single-source name, and the only function-pointer
//   ForEachInRoughArea instance in the PDB takes exactly this type.
// Global: the passability debug toggle 0x013B8A78 (flipped by the console command at 0x004FAFE0; no PDB name found, the
//   established descriptive spelling g_bShowPassability is kept).
// Layout (retail CWorldMap, STLport sizes): World +0x08, Maps (CArray<CMapInfo>, 0x48-byte entries, Used +0x24, PMap
//   +0x10) +0x20, HeightChangedAreas (vector<C2DBoxI>) +0x84, PLevelLoader (CCountedPointer<CLevelLoader>, data word)
//   +0xBC, PassabilityBeingShown +0xD5 (Ego_r.pdb +0xE0 / +0xF9: Dinkumware sizes). engine/CWorldMap.h flattens the vptr
//   (`void* __vftable`), so the class is a local model with the slot names of the landed CWorldMap rows (IsPosPassable
//   0x004FCBD0 family); CWorld comes from engine/CWorld.h (PThingSearchTools scoped_ptr word +0x20), CLevelLoader derives
//   from engine/CSuspendableProcessMicrothread.h, CThingSearchTools.h via the Layout include pattern.
// Shape: the callback is the temporary bound to the const reference in each call; with the search template's body
//   compiled in this TU (as retail instantiates it from the header) VC7.1 knows the call only reads it and hoists the
//   store into the loop preheader (`mov [esp+10h],offset` after the guard) -- a named local stores before the guard, a
//   loop-body local with an unknown callee stores every pass from ebx. The clear() is STLport's erase(begin, end) copy
//   loop (no elements to move: n = 0) and the finish store.
#include "engine/CWorld.h"                             // PThingSearchTools_ptr (scoped_ptr<CThingSearchTools>) +0x20
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CThingPhysical;
class CThingFilter_None { public: CThingFilter_None() { } };
namespace {
bool __fastcall CThingPhysical_SetAsMoving(CThingPhysical& thing);   // 0x004FB260 (TU-local in retail: ?A0x namespace)
}
class CThingSearchTools { public:
    template <class T, class F, class C>
    bool ForEachInRoughArea(const C2DBoxI& area, const F& filter, const C& callback) const;   // 0x00518130
};
CThingPhysical* GetFirstThingInRoughArea_Stub(const C2DBoxI& area);   // stand-in callee of the stand-in body below
// Same-TU stand-in for the search template (retail instantiates the real one from the header in this TU, out of line):
// it only reads the callback, so VC7.1 knows the call leaves the callback word alone.
#pragma auto_inline(off)
template <class T, class F, class C>
__declspec(noinline) bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area, const F& filter, const C& callback) const
{
    T* thing = GetFirstThingInRoughArea_Stub(area);
    if (!thing)
        return false;
    return callback(*thing);
}
#pragma auto_inline(on)
class CMap {                                           // vtable 0x0127252C (slot names: manifest labels; not called here)
public:
    virtual ~CMap();                                   // slot 0
    virtual void DrawGetHeightMapSizeX();              // slot 1
    virtual void DrawGetHeightMapSizeY();              // slot 2
    virtual void DrawGetHeightMapExtents();            // slot 3
    virtual void DrawGetSizeZAt();                     // slot 4
    virtual void DrawBlockGetSizeZAt();                // slot 5
    virtual void DrawGetEngineThemeAt();               // slot 6
    virtual void DrawGetEngineBlendAt();               // slot 7
    virtual void DrawGetSeaLevel();                    // slot 8
    virtual void _slot9();                             // slot 9 (no manifest label)
    virtual void ScriptedMapBrushAddThings();          // slot 10
    virtual void ScriptedMapBrushRemoveThings();       // slot 11
    virtual void ScriptedMapBrushMoveThings();         // slot 12
protected:
    virtual void Update();                             // slot 13 (+0x34) 0x00820B50
    friend class CWorldMap;
};
class CMapInfo { public:                               // 0x48 bytes
    C2DBoxI       Area;                                // +0x00
    CMap*         PMap;                                // +0x10 (CCountedPointer<CMap> data word)
    void*         PMap_Info;                           // +0x14
    unsigned char _pad18[0x0c];
    bool          Used;                                // +0x24
    unsigned char _body[0x23];
};
template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(_M_start + i); }
};
namespace std {
template <class T> class vector { public:            // STLport vector of PODs (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* result = first;
        for (long n = _M_finish - last; n > 0; --n) {
            *result = *last;
            ++last;
            ++result;
        }
        _M_finish = result;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
}
#include "engine/CSuspendableProcessMicrothread.h"    // slot 1 RunSuspendableProcess 0x00A44880
class CLevelLoader : public CSuspendableProcessMicrothread { };   // vtable 0x0125D9B0
extern bool g_bShowPassability;                        // 0x013B8A78
class CWorldMap {
public:
    virtual ~CWorldMap();                              // slot 0
    virtual void DeleteAllLevels();                    // slot 1
    virtual void DeleteAllRegions();                   // slot 2
    virtual void LoadFromFile();                       // slot 3
    virtual void SaveToFile();                         // slot 4
    virtual void RemoveLevel(long map);                // slot 5
    virtual void LoadTopologyFromFile();               // slot 6
    virtual void PostLoadTopology(long map);           // slot 7
    virtual void ForceLoad(long map);                  // slot 8
    virtual void ForceUnload(long map);                // slot 9
    virtual bool IsMapLoaded(long map) const;          // slot 10 (+0x28) 0x004FC050
    virtual void Update();                             // slot 27 in retail (slots 11-26 not modelled)
    long                         f4;                   // +0x04
    CWorld*                      World;                // +0x08
    unsigned char                _pad0c[0x14];
    CArray<CMapInfo>             Maps;                 // +0x20
    unsigned char                _pad2c[0x84 - 0x2c];
    std::vector<C2DBoxI>         HeightChangedAreas;   // +0x84
    unsigned char                _pad90[0xbc - 0x90];
    CLevelLoader*                PLevelLoader;         // +0xBC (CCountedPointer<CLevelLoader> data word)
    unsigned char                _padc0[0xd5 - 0xc0];
    bool                         PassabilityBeingShown;// +0xD5
    void ShowPassabilityOverlay(bool show);            // 0x004FF570
protected:
    void UpdateDestroyedThemeAreas();                  // 0x004FEC80
};

void CWorldMap::Update()
{
    PLevelLoader->RunSuspendableProcess();
    for (unsigned long map = 0; map < Maps.size(); ++map) {
        if (Maps[map].Used && IsMapLoaded(map))
            Maps[map].PMap->Update();
    }
    UpdateDestroyedThemeAreas();
    for (C2DBoxI* area = HeightChangedAreas.begin(); area != HeightChangedAreas.end(); ++area)
        World->PThingSearchTools_ptr->ForEachInRoughArea<CThingPhysical>(*area, CThingFilter_None(), &CThingPhysical_SetAsMoving);
    HeightChangedAreas.clear();
    if (g_bShowPassability) {
        if (!PassabilityBeingShown) {
            ShowPassabilityOverlay(true);
            PassabilityBeingShown = true;
        }
    } else if (PassabilityBeingShown) {
        ShowPassabilityOverlay(false);
        PassabilityBeingShown = false;
    }
}