// CWorld::GetEnvironmentFishWeightMult(const C3DVector&) -- the fishing weight multiplier at a position: the blend-weighted sum of the cell's three day-set themes' FishWeightMult, any unblended remainder counting 1.0; 1.0 off the game map
// Leaf: GetEnvironmentFishWeightMult
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049F2D0, 283 bytes, __fastcall `ret 4`.
// Callees: CWorld::PeekWorldMap (slot 13), CWorldMap::GetPGameMapCellAt @ 0x0063D8A0, CEnvironment::
// GetGlobalEnvironmentThemeIndexFromLocal @ 0x006B2C60, CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDef<
// CEnvironmentThemeDaySetDef> @ 0x004ABA00, CGameMapCell::GetAtmosBlend @ 0x006B74E0, the def's virtual slot 1.
// Shape: GetDef is a same-TU noinline stand-in body that fills the raw, null-initialised def pointer through `T*&`
// (writes only the out slot), so the def stays in edi and the dead out slot's word is reused for the fild temporaries
// (0x14 frame); the def is released by hand at the end of each iteration.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CDefBase : CDefVirtual {
    long m_count;                                  // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
class CEnvironmentThemeDaySetDef : public CDefBase { public:
    unsigned char _pad08[0x68 - 0x08];
    float FishWeightMult;                          // +0x68 (Ego_r.pdb +0x70)
};
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, T*& out);   // 0x004ABA00 (CEnvironmentThemeDaySetDef instantiation)
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, T*& out)
{
    void* def = Lookup_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CGameMapCell { public:
    unsigned char AtmosThemes[3];                  // +0x00
    unsigned char AtmosBlends[2];                  // +0x03
    signed char   SoundTheme;                      // +0x05
    unsigned char Music;                           // +0x06
    unsigned char GetAtmosBlend(long index) const; // 0x006B74E0
};
class CWorldMap { public:
    const CGameMapCell* GetPGameMapCellAt(const C3DVector& pos) const;   // 0x0063D8A0
};
class CEnvironment { public:
    void* __vftable;
    void* DefinitionManager;                       // +0x04
    unsigned char _pad08[8];
    long  RegionTheme;                             // +0x10
    long GetGlobalEnvironmentThemeIndexFromLocal(long localIndex);   // 0x006B2C60
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap* PeekWorldMap() const;   // slot 13 (+0x34)
    unsigned char _pad04[0x14];
    CEnvironment* PEnvironment;                    // +0x18 (scoped_ptr)
    float GetEnvironmentFishWeightMult(const C3DVector& pos);
};
float CWorld::GetEnvironmentFishWeightMult(const C3DVector& pos)
{
    const CWorldMap* map = PeekWorldMap();
    const CGameMapCell* cell = map->GetPGameMapCellAt(C3DVector(pos.X, pos.Y, 0.0f));
    if (!cell)
        return 1.0f;
    float mult = 0.0f;
    long totalBlend = 0;
    for (long i = 0; i != 3; ++i) {
        long theme = cell->AtmosThemes[i];
        long globalTheme = theme == 0 ? PEnvironment->RegionTheme : PEnvironment->GetGlobalEnvironmentThemeIndexFromLocal(theme);
        CEnvironmentThemeDaySetDef* def = 0;
        if (CGameDefinitionManager::Get().GetDef(globalTheme, def)) {
            totalBlend += cell->GetAtmosBlend(i);
            mult += (float)cell->GetAtmosBlend(i) * def->FishWeightMult * (1.0f / 255.0f);
        }
        if (def)
            def->Release();
    }
    if (totalBlend < 255)
        mult += (float)(255 - totalBlend) * (1.0f / 255.0f);
    return mult;
}