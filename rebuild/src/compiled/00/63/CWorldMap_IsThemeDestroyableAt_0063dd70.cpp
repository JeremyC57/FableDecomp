// CWorldMap::IsThemeDestroyableAt(const C3DVector& pos) const -- true when the map under the position is loaded and its strongest engine theme there has a destruction theme
// Leaf: ?IsThemeDestroyableAt@CWorldMap@@QBE_NABVC3DVector@@@Z
// Retail: Fable.exe @ 0x0063DD70, 119 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb ?IsThemeDestroyableAt@CWorldMap@@QBE_NABVC3DVector@@@Z (bool, const C3DVector&), the only
// CWorldMap predicate on a position that reads the theme def's DestructionThemeDef (PDB +0x9C, retail +0x98: the field
// DestroyThemeAt 0x0063EB20 paints); Ego_r.pdb places it among the theme queries after FindLastUnblockedThemePosition-
// AndReportCollision (0x0063DB30 here, the retail neighbour before it). Its caller 0x00661A80 (CThingPhysical slot,
// `GetWorldMap()->IsThemeDestroyableAt(pos)`) is in the CThingPhysical TU.
// Callees: GetMapNumberAt(const C3DVector&) slot 16 (+0x40), IsMapLoaded slot 10 (+0x28), GetStrongestEngineThemeAt
// 0x0063ADF0, PeekDefinitionManager 0x004FB310, CDefinitionManager::GetDef<CEngineThemeDef> 0x0063F3A0, the def's Destroy
// slot 1.
// Shape: the landed DestroyThemeAt model (same TU): the layer out-slot and the def pointer homed in the dead `pos` slot;
// the def's release has no null test and the flag is read before it (`setne bl`).
#include <new>
class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public: long X, Y; };
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
class CMap;
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
    long GetStrongestEngineThemeAt(const C3DVector& pos, long* layer) const;     // 0x0063ADF0
    const CDefinitionManager& PeekDefinitionManager() const;        // 0x004FB310
    bool IsThemeDestroyableAt(const C3DVector& pos) const;
};
bool CWorldMap::IsThemeDestroyableAt(const C3DVector& pos) const
{
    long map = GetMapNumberAt(pos);
    if (map != 0 && IsMapLoaded(map)) {
        long layer;
        long theme = GetStrongestEngineThemeAt(pos, &layer);
        CDefPointer<CEngineThemeDef> def;
        PeekDefinitionManager().GetDef(theme, def);
        return def->DestructionThemeDef != 0;
    }
    return false;
}