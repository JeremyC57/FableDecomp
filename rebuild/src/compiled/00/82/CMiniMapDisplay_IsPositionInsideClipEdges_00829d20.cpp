// CMiniMapDisplay::IsPositionInsideClipEdges(const C3DVector& pos) const -- false when an enabled edge (four flags at +0x40..+0x43) excludes the position's Z (+8) or Y (+4) against the mini-map's two edge limits, true otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00829D20, length 105 bytes (padded to 112).
// Label correction: the catalog "CGameTimeManager::IsDaybreak" is propagated over the byte-identical twin shape; the
// function sits among CMiniMapDisplay methods (0x00829CE0 GetRelativePosOnMap, 0x00829D90 InitialiseMiniMapFileLoading_Region).
// The method name is descriptive (twin of the CTCInventoryMap test @ 0x005FABA0).
// Globals: the Z edge @ 0x01272580, the Y edge @ 0x01272584 (floats).
// Shape: four `if (flag && compare) return false;` tests; `>` gives `test ah,41h ; je`, `<=` gives `test ah,41h ; jnp`.
// Leaf: ?IsPositionInsideClipEdges@CMiniMapDisplay@@
struct C3DVector {
    float X, Y, Z;
};
extern const float g_miniMapEdgeZ;                 // 0x01272580
extern const float g_miniMapEdgeY;                 // 0x01272584
struct CMiniMapDisplay {
    unsigned char _pad00[0x40];
    bool          m_clipAboveZ;                    // +0x40
    bool          m_clipBelowZ;                    // +0x41
    bool          m_clipBelowY;                    // +0x42
    bool          m_clipAboveY;                    // +0x43
    bool IsPositionInsideClipEdges(const C3DVector& pos) const;
};

bool CMiniMapDisplay::IsPositionInsideClipEdges(const C3DVector& pos) const
{
    if (m_clipAboveZ && pos.Z > g_miniMapEdgeZ)
        return false;
    if (m_clipBelowZ && pos.Z <= g_miniMapEdgeZ)
        return false;
    if (m_clipBelowY && pos.Y <= g_miniMapEdgeY)
        return false;
    if (m_clipAboveY && pos.Y > g_miniMapEdgeY)
        return false;
    return true;
}