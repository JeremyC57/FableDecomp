// Grade: DIFFER(354v345) -- frame: retail shares one slot between GetThemeIndexAt's out word and the theme CDefPointer (0x2C frame, ours 0x34), keeps x in edi with the area reference re-read from [ebp+8] each step, and releases the def without re-reading it; the passable/theme logic, cell addressing and the 0.7 normal test already match in shape
// CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI&) -- when the map still needs it, mark each height-field cell of the area passable unless its theme forbids it or the ground there is steeper than a 0.7 normal
// Leaf: InitPassabilityFromThemesIfNecessary
// Callees: CMap theme lookup 0x0081F910 (labelled CopyPos), CGameDefinitionManager::Get 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<T> 0x0063F3A0, CMap::GetGroundFaceNormalAt 0x0081F6C0; constants 0.0f @ 0x0122DEDC, 0.7f @ 0x01255318.
// Layout: no CMap engine header; local model (height field +0x1A7C: cells +0x10 of 12 bytes, width +0x204, height +0x208; passability flag +0x1D38; theme def flags +0xA0 bit 0, passability +0x80).
struct C2DCoordI {
    long X, Y;
};
struct C2DBoxI {
    C2DCoordI Min;
    C2DCoordI Max;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CHeightFieldCell {
    unsigned char _pad00[9];
    unsigned char Reserved : 1;
    unsigned char Passable : 1;                 // +0x09 bit 1
    unsigned char _rest : 6;
    unsigned char _pad0a[2];
};
struct CHeightField {
    unsigned char     _pad00[0x10];
    CHeightFieldCell* Cells;                    // +0x10
    unsigned char     _pad14[0x1f0];
    long              Width;                    // +0x204
    long              Height;                   // +0x208
    CHeightFieldCell* GetCell(long x, long y) { return &Cells[(Height - y) * (Width + 1) + x]; }
};
struct CThemeDef {
    virtual void v00();
    virtual void Delete();                      // slot 1
    long          m_refCount;                   // +0x04
    unsigned char _pad08[0x78];
    float         Passability;                  // +0x80
    unsigned char _pad84[0x1c];
    unsigned char Flags;                        // +0xa0 bit 0: passable
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (--Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThemeDef>* out);   // 0x0063F3A0
};
struct CGameDefinitionManager {
    static CDefinitionManager* __fastcall Get();            // 0x0044C6B0
};
struct CMap {
    virtual void v00();
    unsigned char _pad04[0x1a78];
    CHeightField* HeightField;                  // +0x1a7c
    unsigned char _pad1a80[0x2b8];
    bool          PassabilityNeedsInit;         // +0x1d38
    long GetThemeIndexAt(const C2DCoordI& pos, long* weight);         // 0x0081F910
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos);            // 0x0081F6C0
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area);
};

void CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI& area)
{
    if (!PassabilityNeedsInit)
        return;
    CHeightField* heightField = HeightField;
    C2DCoordI pos;
    for (pos.Y = area.Min.Y; pos.Y < area.Max.Y; ++pos.Y) {
        CHeightFieldCell* cell = heightField->GetCell(area.Min.X, pos.Y);
        for (pos.X = area.Min.X; pos.X < area.Max.X; ++pos.X, ++cell) {
            bool passable = true;
            long theme;
            {
                long weight;
                theme = GetThemeIndexAt(pos, &weight);
            }
            if (theme > 0) {
                CDefPointer<CThemeDef> def;
                CGameDefinitionManager::Get()->GetDef(theme, &def);
                passable = (def->Flags & 1) && def->Passability > 0.0f;
            }
            if (passable) {
                C3DVector normal = GetGroundFaceNormalAt(C3DVector((float)pos.X, (float)pos.Y, 0.0f));
                if (normal.Z < 0.7f)
                    passable = false;
            }
            cell->Passable = passable;
        }
    }
}
