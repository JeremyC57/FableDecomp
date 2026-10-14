// CMap::GetThemeMinCameraHeightAtPosition(const C2DVector& pos) -- the theme's minimum camera-size Z offset at the position's cell plus the interpolated terrain height there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008200B0, length 80 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (x2), CMap::GetThemeMinCameraSizeZOffsetAt(C2DCoordI*) @ 0x0081FA20, CMap::
// InterpolateHeightAtPosition(const C2DVector&) @ 0x0081EA50 (manifest "Terrain_InterpolateHeightAtPosition").
// Class attribution: manifest label "CGameScriptInterface::MsgIsActionModeButtonPressed" is propagated; `this` is handed
// unchanged to two CMap methods, so this is a CMap method.
// Shape: the C2DCoordI cell is built in the 8-byte frame from two __ftol2 conversions, the offset is a named float local
// parked in the dead argument slot (`fstp [esp+14h]`) across the second call and added last (`fadd [esp+14h]`).
struct C2DVector {
    float X;
    float Y;
};

struct C2DCoordI {
    long X;
    long Y;
};

struct CMap {
    float GetThemeMinCameraSizeZOffsetAt(C2DCoordI* cell);          // 0x0081FA20
    float InterpolateHeightAtPosition(const C2DVector& pos);        // 0x0081EA50
    float GetThemeMinCameraHeightAtPosition(const C2DVector& pos);
};

float CMap::GetThemeMinCameraHeightAtPosition(const C2DVector& pos)
{
    C2DCoordI cell;
    cell.X = (long)pos.X;
    cell.Y = (long)pos.Y;
    float offset = GetThemeMinCameraSizeZOffsetAt(&cell);
    return offset + InterpolateHeightAtPosition(pos);
}
