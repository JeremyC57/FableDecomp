// CWorldMap::GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const -- the navigation layer under a
// Grade: DIFFER(340v340) -- one store: the Bind3 branch stores F3 (&overlap, [esp+1Ch]) one instruction before `lea ecx,[esp+14h]` (retail after); chunk 613 tried Bind3 body-assign, named tools in the Bind3 branch, QBE ForEachInRoughArea + const finder (PDB spelling), PDB F1/F2/F3 non-const members, named overlap filter (26), branch swap (360), named IsNot local: all inert or worse
// world position: with per-map navigation enabled, no thing to ignore and the engine's slot-9 query false, the loaded
// map's own GetNavigationLayerAt answers (0 when the map is not loaded); otherwise every physical thing with
// navigation information whose bounding box overlaps the position (radius 0.1), skipping `ignore`, is asked through a
// CThingFindNavigationLayerAt finder in a 2x2 box round the position
// Leaf: GetNavigationLayerAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E590, 340 bytes, `ret 8`.
//   PDB ?GetNavigationLayerAt@CWorldMap@@QBEJABVC3DVector@@PBVCThing@@@Z.
// Callees: engine singleton 0x013B86A0 slot 9 (+0x24, bool), GetMapNumberAt(const C3DVector&) slot 16 (+0x40),
//   IsMapLoaded slot 10 (+0x28), PeekMap slot 19 (+0x4c), CMap::GetNavigationLayerAt(const C3DVector&) 0x0081E920
//   (labelled _Cons_val<...CParticleMorphs...>), __ftol2 0x00BFEA70 (x3),
//   CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind2<HasNavigationInformation,
//   OverlappingBoundingBox>, CThingFindNavigationLayerAt> 0x0063F4B0 and the CFilter_Bind3<IsNot, ...> form 0x0063F7C0.
// Layout: CWorldMap::World +8 (retail, the local model of the landed siblings), CWorld +0x20 thing search tools;
//   CThingFindNavigationLayerAt {Pos, Layer} 16 bytes, GetLayer() const returns float (PDB QBEMXZ: fild + ftol);
//   CThingFilter_OverlappingBoundingBox {Pos, Radius}; CFilter_Bind2/3 hold pointers to their filters (Ego_r.pdb).
//   The per-map flag byte 0x0137571C is unnamed in the labels.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C2DBoxI { public:
    long x0, y0, x1, y1;
    C2DBoxI(long ax0, long ay0, long ax1, long ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
};
class CThing;
class CThingPhysical;
class CThingFilter_HasNavigationInformation { public: CThingFilter_HasNavigationInformation() { } };
class CThingFilter_IsNot { public:
    CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing((CThing*)thing) { }
};
class CThingFilter_OverlappingBoundingBox { public:
    C3DVector Pos;
    float     Radius;
    CThingFilter_OverlappingBoundingBox(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) { }
};
template <class T1, class T2> class CFilter_Bind2 { public:
    T1* F1; T2* F2;
    CFilter_Bind2(const T1& f1, const T2& f2) : F1((T1*)&f1), F2((T2*)&f2) { }
};
template <class T1, class T2, class T3> class CFilter_Bind3 { public:
    T1* F1; T2* F2; T3* F3;
    CFilter_Bind3(const T1& f1, const T2& f2, const T3& f3) : F1((T1*)&f1), F2((T2*)&f2), F3((T3*)&f3) { }
};
class CThingFindNavigationLayerAt { public:
    C3DVector Pos;
    mutable long Layer;
    CThingFindNavigationLayerAt(const C3DVector& pos) : Pos(pos), Layer(0) { }
    float GetLayer() const { return (float)Layer; }
};
typedef CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> CNavFilter2;
typedef CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> CNavFilter3;
class CThingSearchTools { public:
    template <class T, class F, class P> bool ForEachInRoughArea(const C2DBoxI& box, const F& filter, const P& proc) const;
};
class CWorld { public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;           // +0x20
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;           // 0x013B86A0
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();   // slots 0-8 (not used here)
    virtual bool IsEditorActive() const;                     // slot 9 (+0x24), 0x004197B0 (FableWin.pdb UBE_NXZ)
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
};
extern bool g_bMapNavigationLayers;                // 0x0137571C
class CMap {
public:
    long GetNavigationLayerAt(const C3DVector& pos) const;   // 0x0081E920
};
class C2DCoordI;
class CWorldMap {
public:
    virtual ~CWorldMap();                                           // slot 0
    virtual void DeleteAllLevels();                                 // slot 1
    virtual void DeleteAllRegions();                                // slot 2
    virtual void LoadFromFile();                                    // slot 3
    virtual void SaveToFile();                                      // slot 4
    virtual void RemoveLevel(long map);                             // slot 5
    virtual void LoadTopologyFromFile();                            // slot 6
    virtual void PostLoadTopology(long map);                        // slot 7
    virtual void ForceLoad(long map);                               // slot 8
    virtual void ForceUnload(long map);                             // slot 9
    virtual bool IsMapLoaded(long map) const;                       // slot 10 (+0x28)
    virtual bool IsMapUsed(long map) const;                         // slot 11
    virtual long GetGranularity() const;                            // slot 12
    virtual const void* GetDimensions() const;                      // slot 13
    virtual CMap& GetMapAt(const C2DCoordI& pos);                   // slot 14
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;      // slot 15
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;        // slot 17 (+0x44)
    virtual long GetMapNumberAt(const C3DVector& pos) const;        // slot 16 (+0x40)
    virtual CMap& GetMap(long map);                                 // slot 18 (+0x48)
    virtual const CMap& PeekMap(long map) const;                    // slot 19 (+0x4c)
    long GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const;
    unsigned char _pad04[4];
    CWorld*       World;                                            // +0x08
};
long CWorldMap::GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const
{
    if (g_bMapNavigationLayers && ignore == 0 && !CMainGameComponent::Get()->IsEditorActive()) {
        long map = GetMapNumberAt(pos);
        if (map != 0 && IsMapLoaded(map))
        {
            const CMap& theMap = PeekMap(map);
            return theMap.GetNavigationLayerAt(pos);
        }
        return 0;
    }
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI box(x - 1, y - 1, x + 1, y + 1);
    CThingFindNavigationLayerAt finder(pos);
    if (ignore == 0) {
        CThingSearchTools* tools = World->ThingSearchTools;
        tools->ForEachInRoughArea<CThingPhysical>(box, CNavFilter2(CThingFilter_HasNavigationInformation(), CThingFilter_OverlappingBoundingBox(pos, 0.1f)), finder);
    }
    else
        World->ThingSearchTools->ForEachInRoughArea<CThingPhysical>(box, CNavFilter3(CThingFilter_IsNot(ignore), CThingFilter_HasNavigationInformation(), CThingFilter_OverlappingBoundingBox(pos, 0.1f)), finder);
    return finder.GetLayer();
}
