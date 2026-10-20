// CWorldMap::GetImpassabilityFactorAround(const C3DVector&, vector<float>&, bool) const -- fill nine impassability factors (0 passable, 1 blocked) for the world-map block under a position and its eight neighbours
// Leaf: GetImpassabilityFactorAround
// Retail: Fable.exe @ 0x0063E3C0, 177 bytes, `ret 0Ch`. PDB ?GetImpassabilityFactorAround@CWorldMap@@QBEXABVC3DVector@@AAV?$vector@MV?$allocator@M@std@@@std@@_N@Z
// Callees: CWorldMap::PeekDefinitionManager() const 0x004FB310 (twice, results unused; it forwards to the component's
// CGameDefinitionManager::Get 0x0044C6B0), memmove 0x00BFEAE6 (the inlined vector clear), _ftol2 0x00BFEA70,
// CWorldMap::IsPosPassable(const C2DCoordI&) const 0x004FCBD0.
// Data: the nine block offsets {-1,-1} {0,-1} {1,-1} {1,0} {1,1} {0,1} {-1,1} {-1,0} {0,0} at 0x01385978 (C2DCoordI[9]).
// Shape: the vector is cleared with the inline erase(begin(), end()) (early-out copy-down) and then written by index
// (retail relies on its capacity); every slot is zeroed before the passability test; with ebx/ebp/esi/edi taken by the
// vector, centre.X, the offset pointer and the index, `this` and the C2DCoordI centre local live in the frame (only
// its Y is ever stored); the bool argument is unused.
#include <string.h>

struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
extern const C2DCoordI g_BlockNeighbourOffsets[9];          // 0x01385978

struct CFloatVector {                                        // STLport vector<float> (12 bytes)
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    float* begin() { return _M_start; }
    float* end() { return _M_finish; }
    float& operator[](unsigned long n) { return *(begin() + n); }
    static float* CopyDown(float* src, float* srcEnd, float* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long count = (char*)srcEnd - (char*)src;
        return (float*)((char*)memmove(dest, src, count) + count);
    }
    void erase(float* first, float* last) { _M_finish = CopyDown(last, _M_finish, first); }
    void clear() { erase(begin(), end()); }
};

struct CGameDefinitionManager { };
struct CWorldMap {
    const CGameDefinitionManager& PeekDefinitionManager() const;      // 0x004FB310
    bool IsPosPassable(const C2DCoordI& pos) const;                   // 0x004FCBD0
    void GetImpassabilityFactorAround(const C3DVector& pos, CFloatVector& factors, bool unused) const;
};

void CWorldMap::GetImpassabilityFactorAround(const C3DVector& pos, CFloatVector& factors, bool unused) const
{
    PeekDefinitionManager();
    PeekDefinitionManager();
    factors.clear();
    C2DCoordI centre((long)pos.X, (long)pos.Y);
    for (long i = 0; i < 9; ++i)
    {
        factors[i] = 0.0f;
        if (!IsPosPassable(C2DCoordI(g_BlockNeighbourOffsets[i].X + centre.X, g_BlockNeighbourOffsets[i].Y + centre.Y)))
            factors[i] = 1.0f;
    }
}