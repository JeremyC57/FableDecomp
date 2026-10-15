// CTCInventoryMap::IsPositionInsideClipEdges(const C3DVector& pos) const -- false when an enabled edge (four flags at +0x184..+0x187) excludes the position's Z (+8) or Y (+4) against the two map-edge limits, true otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FABA0, length 117 bytes (padded to 128).
// Label correction: the catalog "CGameTimeManager::IsDaybreak" is propagated over the byte-identical twin shape; the
// function sits among CTCInventoryMap methods (0x005FAA60 IsTeleportValid, 0x005FAC20 GetMenuEntryFromCategory) and is
// called only from CTCInventoryMap code (0x005FEA31 ..). The method name is descriptive.
// Globals: the Z edge @ 0x01253D40, the Y edge @ 0x01257088 (floats).
// Shape: four `if (flag && compare) return false;` tests; `>` gives `test ah,41h ; je`, `<=` gives `test ah,41h ; jnp`;
// the shared `xor al,al` exit sits after the fourth test, `mov al,1` last.
// Leaf: ?IsPositionInsideClipEdges@CTCInventoryMap@@
struct C3DVector {
    float X, Y, Z;
};
extern const float g_mapEdgeZ;                     // 0x01253D40
extern const float g_mapEdgeY;                     // 0x01257088
struct CTCInventoryMap {
    unsigned char _pad000[0x184];
    bool          m_clipAboveZ;                    // +0x184
    bool          m_clipBelowZ;                    // +0x185
    bool          m_clipBelowY;                    // +0x186
    bool          m_clipAboveY;                    // +0x187
    bool IsPositionInsideClipEdges(const C3DVector& pos) const;
};

bool CTCInventoryMap::IsPositionInsideClipEdges(const C3DVector& pos) const
{
    if (m_clipAboveZ && pos.Z > g_mapEdgeZ)
        return false;
    if (m_clipBelowZ && pos.Z <= g_mapEdgeZ)
        return false;
    if (m_clipBelowY && pos.Y <= g_mapEdgeY)
        return false;
    if (m_clipAboveY && pos.Y > g_mapEdgeY)
        return false;
    return true;
}
