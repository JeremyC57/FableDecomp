// C2DBoxI::GetDistance(const C2DCoordI& pos) -- distance from a point to the box: clamp the point into the box, then the 2D distance from the point to that clamped point, truncated to long
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3A180, length 87 bytes.
// Leaf: GetDistance
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// Ego_r.pdb lists ?GetDistance@C2DBoxI@@QBEJABVC2DCoordI@@@Z (Ego_r 0x00B93D80); C2DBoxI = {TLX, TLY, BRX, BRY},
// C2DCoordI = {X, Y}. Caller: CWorldMap::GetMinPlayerDistanceFromMap 0x004FCB82, on `Maps[i].Area` (CMapInfo +0,
// 0x48-byte records) with the player position truncated to a C2DCoordI.
// Callees: GFGet2DDistance(const C2DCoordI&, const C2DCoordI&) @ 0x009E2430 (__fastcall: pos in ecx, nearest in edx),
//          __ftol2 @ 0x00BFEA70.
// Shape: the clamp tests the LOCAL copy (`nearest.X < TLX`), so pos.X / pos.Y stay in edx / edi after the copy; testing
// `pos.X` re-reads the reference after each store and duplicates the call tail (108 bytes).
class C2DCoordI {
public:
    long X;                                       // +0x0
    long Y;                                       // +0x4
};
float __fastcall GFGet2DDistance(const C2DCoordI& a, const C2DCoordI& b);   // 0x009E2430
class C2DBoxI {
public:
    long TLX;                                     // +0x0
    long TLY;                                     // +0x4
    long BRX;                                     // +0x8
    long BRY;                                     // +0xc
    long GetDistance(const C2DCoordI& pos) const;
};
long C2DBoxI::GetDistance(const C2DCoordI& pos) const
{
    C2DCoordI nearest = pos;
    if (nearest.X < TLX)
        nearest.X = TLX;
    else if (nearest.X > BRX)
        nearest.X = BRX;
    if (nearest.Y < TLY)
        nearest.Y = TLY;
    else if (nearest.Y > BRY)
        nearest.Y = BRY;
    return (long)GFGet2DDistance(pos, nearest);
}