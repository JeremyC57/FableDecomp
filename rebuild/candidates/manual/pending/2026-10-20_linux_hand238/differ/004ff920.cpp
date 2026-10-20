// Grade: DIFFER(319v319) -- the callback pointer local is stored before the loop guard in ours, after it (loop preheader) in retail; everything else matches
// CWorldMap::Update() -- per-frame world-map update: the level loader, every used loaded map, destroyed theme areas, a physics refresh for the things inside each height-changed area (then the list is cleared), and the passability overlay switched to follow its debug toggle
// Leaf: ?Update@CWorldMap@@UAEXXZ
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CThingPhysical;
struct CThingFilter_None { CThingFilter_None() { } };
typedef bool (__fastcall* ThingCallback)(CThingPhysical& thing);
bool __fastcall ResetPhysicsAfterHeightChange(CThingPhysical& thing);   // 0x004FB260
struct CThingSearchTools {
    template <class T, class F, class C> void ForEachInRoughArea(const C2DBoxI& area, const F& filter, C& callback);   // 0x00518130 <CThingPhysical, CThingFilter_None, bool (__fastcall*)(CThingPhysical&)>
};
struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;              // +0x20 (scoped_ptr)
};
struct CMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void Update();                             // slot 13 (+0x34)
};
struct CMapInfo {                                      // 0x48 bytes
    C2DBoxI       Area;                                // +0x00
    CMap*         PMap;                                // +0x10 (CCountedPointer<CMap> data word)
    void*         PMap_Info;                           // +0x14
    unsigned char _pad18[0x0c];
    bool          Used;                                // +0x24
    unsigned char _body[0x23];
};
template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(_M_start + i); }
};
template <class T> struct vector {                     // STLport vector of PODs (12 bytes in retail)
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
struct CLevelLoader {
    virtual void v00();
    virtual void Update();                             // slot 1 (+0x04)
};
extern bool g_bShowPassability;                        // 0x013B8A78
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;          // slot 10 (+0x28)
    unsigned char    _pad04[4];
    CWorld*          World;                            // +0x08
    unsigned char    _pad0c[0x14];
    CArray<CMapInfo> Maps;                             // +0x20
    unsigned char    _pad2c[0x84 - 0x2c];
    vector<C2DBoxI>  HeightChangedAreas;               // +0x84 (PDB +0x9C)
    unsigned char    _pad90[0xbc - 0x90];
    CLevelLoader*    PLevelLoader;                     // +0xBC (CCountedPointer data word; PDB +0xE0)
    unsigned char    _padc0[0xd5 - 0xc0];
    bool             PassabilityBeingShown;            // +0xD5 (PDB +0xF9)
    void UpdateDestroyedThemeAreas();                  // 0x004FEC80
    void ShowPassabilityOverlay(bool show);            // 0x004FF570
    void Update();
};
void CWorldMap::Update()
{
    PLevelLoader->Update();
    for (unsigned long map = 0; map < Maps.size(); ++map) {
        if (Maps[map].Used && IsMapLoaded(map))
            Maps[map].PMap->Update();
    }
    UpdateDestroyedThemeAreas();
    ThingCallback callback = ResetPhysicsAfterHeightChange;
    for (C2DBoxI* area = HeightChangedAreas.begin(); area != HeightChangedAreas.end(); ++area)
        World->PThingSearchTools->ForEachInRoughArea<CThingPhysical>(*area, CThingFilter_None(), callback);
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
