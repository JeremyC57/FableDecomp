// CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader(long) -- the vertex shader for n lights, clamped to the last one in the set
// Leaf: ?GetForegroundSpotLightVertexShader@CEngineLandscapeRenderer@@QAEAAVCVertexShader@@J@Z
// Retail: Fable.exe @ 0x00BF5C60, 52 bytes, `ret 4`; Ego_r.pdb same symbol (0x00A3A580). Catalog label is right.
// Returns &ForegroundSpotLightVertexShaders[min(n, size - 1)] with an unsigned compare, re-reading the array bounds in the clamp arm and
// _M_start for the result.
// Layout: retail CEngineLandscapeRenderer -- embedded CArray<CVertexShader> (12-byte STLport vectors of 8-byte shaders):
// ForegroundVertexShaders +0x4dc, ForegroundSpotLightVertexShaders +0x4e8 (Ego_r.pdb +0xd3c / +0xd4c; counted back
// from the pointer members at +0x544, as modelled for 0x00BF5CA0, over the 12-byte retail vectors); no engine/ header.
// Shape: /O2; the clamp written inline (as in the pointer-member siblings 0x00BF5CA0..0x00BF5D00).
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
    CVertexShader& GetForegroundSpotLightVertexShader(long n);
    unsigned char           _pad00[0x4e8];
    CArray<CVertexShader>   ForegroundSpotLightVertexShaders; // +0x4e8
};
CVertexShader& CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader(long n)
{
    return ForegroundSpotLightVertexShaders[(unsigned int)n < ForegroundSpotLightVertexShaders.size() - 1 ? n : ForegroundSpotLightVertexShaders.size() - 1];
}