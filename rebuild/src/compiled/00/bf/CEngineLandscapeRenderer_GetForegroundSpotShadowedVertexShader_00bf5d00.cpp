// CEngineLandscapeRenderer::GetForegroundSpotShadowedVertexShader(long) -- the vertex shader for n lights, clamped to the last one in the active set
// Leaf: ?GetForegroundSpotShadowedVertexShader@CEngineLandscapeRenderer@@QAEAAVCVertexShader@@J@Z
// Retail: Fable.exe @ 0x00BF5D00, 42 bytes; Ego_r.pdb same symbol (0x00A3A610); label says Bump, but this is the third (Spot) getter. Reads the CArray<CVertexShader>* ForegroundSpotShadowedVertexShaders
// (0x54c) and returns &(*p)[min(n, size - 1)] with an unsigned compare, re-reading the pointer and the size in each arm.
// The three pointer members sit at +0x544 / +0x548 / +0x54c (Shadowed / BumpShadowed / SpotShadowed, the Ego_r.pdb
// order at +0xdc4..+0xdcc); the sibling getters 0x00BF5CA0 / 0x00BF5CD0 / 0x00BF5D00 read them in that order, 0x30 apart
// as Ego_r.pdb's 0x00A3A5B0 / 0x00A3A5E0 / 0x00A3A610.
// Layout: CVertexShader 8 bytes, CArray<T> a 12-byte STLport vector; the members before +0x544 are not modelled.
// Shape: /O2; the clamp written inline through the member pointer (a local reference, a min() helper or an if lets VC7.1
// keep the size in a register).
class CVertexShader { public: void* Handle; long Flags; };
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
template <class T> class CArray : public vector<T> {};
class CEngineLandscapeRenderer {
public:
    CVertexShader& GetForegroundSpotShadowedVertexShader(long n);
    unsigned char           _pad00[0x54c];
    CArray<CVertexShader>*  ForegroundSpotShadowedVertexShaders;      // +0x54c
};
CVertexShader& CEngineLandscapeRenderer::GetForegroundSpotShadowedVertexShader(long n)
{
    return (*ForegroundSpotShadowedVertexShaders)[(unsigned int)n < ForegroundSpotShadowedVertexShaders->size() - 1 ? n : ForegroundSpotShadowedVertexShaders->size() - 1];
}