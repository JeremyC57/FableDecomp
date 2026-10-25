// CHeightSource::Evaluate(const SPosition2D& position) -- when a provider is attached (+0xD0) and the source is not disabled (+0xCC), return the provider's value at the position (its virtual slot 0, the position passed by value), else 0.0
// Leaf: Evaluate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A390, length 64 bytes (55 + int3 padding), __fastcall, `ret 4`, float result.
// Callers: 0x00C36F4A (adds the result to a +0x0C float) and 0x00C61ECF (stores it); both pass the address of a two-word local.
// Callee: the provider's virtual slot 0 (`call [edx]`) taking the two words by value (pushed +4 then +0) and returning a float.
// Name: no PDB name covers this object; the class, member and type names are descriptive (the catalog's
//   `Thiscall_GraphicsCallbackEvaluate` is descriptive too).
// Shape: the provider pointer is tested, then the disabled byte; the by-value 8-byte position is copied through esi/eax
//   (pushed high word first); the fallback is `fld [0.0f]`.
struct SPosition2D {                        // 8 bytes, plain data
    float x;
    float y;
};

struct CHeightProvider {
    virtual float Evaluate(SPosition2D position);   // slot 0
};

struct CHeightSource {
    unsigned char    _pad_0x00[0xcc];
    bool             m_bDisabled;           // +0xcc
    unsigned char    _pad_0xcd[3];
    CHeightProvider* m_pProvider;           // +0xd0
    float Evaluate(const SPosition2D& position);
};

float CHeightSource::Evaluate(const SPosition2D& position)
{
    if (m_pProvider && !m_bDisabled)
        return m_pProvider->Evaluate(position);
    return 0.0f;
}