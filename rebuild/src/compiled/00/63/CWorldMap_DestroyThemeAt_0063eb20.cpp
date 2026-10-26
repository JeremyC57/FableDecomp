// CWorldMap::DestroyThemeAt(const C3DVector& pos) -- replaces the strongest engine theme at a world position with its
// def's destruction theme: paints the destruction theme on layer 0 of the block, records the block in
// DestroyedThemePositions, clears layers 1 and 2, sets the block's blend to 0xff and, when the block's map is loaded
// and changeable, resets the map's engine blend at the local block
// Leaf: DestroyThemeAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063EB20, 350 bytes, `ret 4`, 8-aligned frame. PDB
//   ?DestroyThemeAt@CWorldMap@@QAEXABVC3DVector@@@Z.
// Callees: IsPosInMap(const C3DVector&) 0x004FC600, GetStrongestEngineThemeAt 0x0063ADF0, PeekDefinitionManager
//   0x004FB310 (labelled _Cons_val<...CParticleMorphs...>), CDefinitionManager::GetDef<T> 0x0063F3A0, __ftol2 0x00BFEA70,
//   SetThemeAtBlock 0x0063A970, vector<C2DCoordI>::_M_insert_overflow 0x00618550, SetThemeBlendAtBlock 0x0063A9E0
//   (labelled SetThemeAtBlock), GetMapNumberAt(const C2DCoordI&) slot 17 (+0x44), IsPosChangeable slot 25 (+0x64),
//   ConvertWorldMapCoordsToLocalCoords(long, const C2DCoordI&) 0x004FC3C0, GetMap slot 18 (+0x48),
//   CMap::SetEngineBlendAt(long, long, unsigned char, unsigned long, EThemePaintType) 0x0081EFC0, def Destroy slot 1.
// Layout: retail CWorldMap::DestroyedThemePositions +0xb0 (12-byte STLport vector of C2DCoordI; PDB +0xd0);
//   CEngineThemeDef::DestructionThemeDef retail +0x98 (PDB +0x9c, def layouts sit 4 below the PDB); the def's count at +4.
#include <new>
struct __false_type { __false_type() { } };
class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public:
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(long x, long y) : X(x), Y(y) { }
    C2DCoordI(const C2DCoordI& o) : X(o.X), Y(o.Y) { }
};
struct CCoordVector {                              // STLport vector<C2DCoordI> (12 bytes)
    C2DCoordI* _M_start;
    C2DCoordI* _M_finish;
    C2DCoordI* _M_end_of_storage;
    void _M_insert_overflow(C2DCoordI* position, const C2DCoordI& x, const __false_type& tag, unsigned long n, bool atEnd);   // 0x00618550
    void push_back(const C2DCoordI& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C2DCoordI(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
};
class CEngineThemeDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x98 - 0x08];
    long          DestructionThemeDef;             // +0x98
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_refCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CEngineThemeDef> GetPDefFromGlobalIndex(long index) const;      // 0x009AD9E0
    bool GetDef(long globalIndex, CDefPointer<CEngineThemeDef>& out) const;   // 0x0063F3A0
};
// Same-TU stand-in for GetDef<T> 0x0063F3A0 (the landed body): retail's LTCG knowledge that the def pointer's address
// does not escape the lookup.
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CEngineThemeDef>& out) const
{
    if (globalIndex <= 0)
        return false;
    CDefPointer<CEngineThemeDef> def = GetPDefFromGlobalIndex(globalIndex);
    if (def.Object) {
        out = def;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
enum EThemePaintType { THEME_PAINT_NORMAL };
class CMap {
public:
    void SetEngineBlendAt(long x, long y, unsigned char blend, unsigned long layer, EThemePaintType type);   // 0x0081EFC0
};
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
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual bool IsPosChangeable(const C2DCoordI& pos) const;       // slot 25 (+0x64)
    bool IsPosInMap(const C3DVector& pos) const;                    // 0x004FC600
    long GetStrongestEngineThemeAt(const C3DVector& pos, long* layer) const;     // 0x0063ADF0
    const CDefinitionManager& PeekDefinitionManager() const;        // 0x004FB310
    void SetThemeAtBlock(const C2DCoordI& pos, unsigned long theme, unsigned long layer, EThemePaintType type);   // 0x0063A970
    void SetThemeBlendAtBlock(const C2DCoordI& pos, unsigned char blend, unsigned long layer, EThemePaintType type);   // 0x0063A9E0
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const;   // 0x004FC3C0
    void DestroyThemeAt(const C3DVector& pos);
    unsigned char _pad04[0xb0 - 0x04];
    CCoordVector  DestroyedThemePositions;                          // +0xb0
};
void CWorldMap::DestroyThemeAt(const C3DVector& pos)
{
    if (!IsPosInMap(pos))
        return;
    long layer;
    long theme = GetStrongestEngineThemeAt(pos, &layer);
    CDefPointer<CEngineThemeDef> def;
    PeekDefinitionManager().GetDef(theme, def);
    unsigned long destructionTheme = def->DestructionThemeDef;
    if (destructionTheme != 0) {
        C2DCoordI block((long)pos.X, (long)pos.Y);
        SetThemeAtBlock(block, destructionTheme, 0, THEME_PAINT_NORMAL);
        DestroyedThemePositions.push_back(block);
        for (long i = 1; i < 3; ++i)
            SetThemeAtBlock(block, 0, i, THEME_PAINT_NORMAL);
        SetThemeBlendAtBlock(block, 0xff, 0, THEME_PAINT_NORMAL);
        long map = GetMapNumberAt(block);
        if (map != 0 && IsPosChangeable(block)) {
            C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, block);
            GetMap(map).SetEngineBlendAt(local.X, local.Y, 0, 1, THEME_PAINT_NORMAL);
        }
    }
}