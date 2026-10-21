// CEngineLandscapeRenderer::GetForegroundShadowedVertexShader(long) -- the vertex shader for n lights, clamped to the last one in the active set
// Leaf: ?GetForegroundShadowedVertexShader@CEngineLandscapeRenderer@@QAEAAVCVertexShader@@J@Z
// Retail: Fable.exe @ 0x00BF5CA0, 42 bytes; Ego_r.pdb same symbol (0x00A3A5B0). Catalog label CEngineLandscapeRenderer::GetForegroundBumpShadowedVertexShader (off by one: this is the plain shadowed getter) is wrong;
// Reads the CArray<CVertexShader>* ForegroundShadowedVertexShaders
// (0x544) and returns &(*p)[min(n, size - 1)] with an unsigned compare, re-reading the pointer and the size in each arm.
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
    CVertexShader& GetForegroundShadowedVertexShader(long n);
    unsigned char           _pad00[0x544];
    CArray<CVertexShader>*  ForegroundShadowedVertexShaders;      // +0x544
};
CVertexShader& CEngineLandscapeRenderer::GetForegroundShadowedVertexShader(long n)
{
    return (*ForegroundShadowedVertexShaders)[(unsigned int)n < ForegroundShadowedVertexShaders->size() - 1 ? n : ForegroundShadowedVertexShaders->size() - 1];
}