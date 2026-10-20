// Grade: DIFFER(282v283) -- same instructions; register permutation (retail ebp=this ebx=cell esi=i edi=def, ours ebx/ebp/edi/esi) and retail reuses the dead out slot for the fild temporaries (0x14 frame, ours 0x18)
// CWorld::GetEnvironmentFishWeightMult(const C3DVector&) -- the fishing weight multiplier at a position: the blend-weighted sum of the cell's three day-set themes' FishWeightMult, any unblended remainder counting 1.0; 1.0 off the game map
// Leaf: GetEnvironmentFishWeightMult
struct C3DVector {
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
struct CEnvironmentThemeDaySetDef : CDefBase {
    unsigned char _pad08[0x68 - 0x08];
    float FishWeightMult;                          // +0x68 (Ego_r.pdb +0x70)
};
template <class T> struct CDefPointer {          // the out slot GetDef fills (zeroed by its ctor)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> struct CDefHolder {           // the def taken once from the out slot; releases it
    T* m_p;
    CDefHolder(T* p) : m_p(p) { }
    ~CDefHolder()
    {
        if (m_p)
            m_p->Release();
    }
    T* operator->() const { return m_p; }
};
struct CDefinitionManager {
    template <class T> bool GetDef(long index, CDefPointer<T>& out);   // 0x004ABA00 (CEnvironmentThemeDaySetDef instantiation)
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CGameMapCell {
    unsigned char AtmosThemes[3];                  // +0x00
    unsigned char AtmosBlends[2];                  // +0x03
    signed char   SoundTheme;                      // +0x05
    unsigned char Music;                           // +0x06
    unsigned char GetAtmosBlend(long index) const; // 0x006B74E0
};
struct CWorldMap {
    const CGameMapCell* GetPGameMapCellAt(const C3DVector& pos) const;   // 0x0063D8A0
};
struct CEnvironment {
    void* __vftable;
    void* DefinitionManager;                       // +0x04
    unsigned char _pad08[8];
    long  RegionTheme;                             // +0x10
    long GetGlobalEnvironmentThemeIndexFromLocal(long localIndex);   // 0x006B2C60
};
struct CWorld {
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
        CDefPointer<CEnvironmentThemeDaySetDef> out;
        bool found = CGameDefinitionManager::Get().GetDef(globalTheme, out);
        CDefHolder<CEnvironmentThemeDaySetDef> def(out.m_p);
        if (found) {
            totalBlend += cell->GetAtmosBlend(i);
            mult += (float)cell->GetAtmosBlend(i) * def->FishWeightMult * (1.0f / 255.0f);
        }
    }
    if (totalBlend < 255)
        mult += (float)(255 - totalBlend) * (1.0f / 255.0f);
    return mult;
}
