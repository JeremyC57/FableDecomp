// _STL::vector<CTexture>::resize(unsigned int new_size) -- STLport one-argument resize of the progress display's texture vector: resize(n, CTexture()) with the empty default texture destroyed on both exits
// Leaf: ?resize@?$vector@VCTexture@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049B7F0, 103 bytes (+ int3 padding), `__fastcall` (this stays in ecx), `ret 4`.
// Identity: Ego_r.pdb `?resize@?$vector@VCTexture@@...@@QAEXI@Z`; the erase callee is the landed vector<CTexture>::erase
//   0x0049B3B0 (FableWin.pdb names) and the temporary's destructor is ~CTexture 0x009F9F70 (byte twin of
//   CTexture::Uninitialise). Callers: CProgressDisplay::InitialiseTextures (0x0049A119, 0x0049A225, 0x0049A22F) and
//   0x00B3C665. The 8-byte element is Ego_r.pdb CTexture { IDirect3DTexture9* PD3DTexture; ulong ByteLength:28,
//   AllocationSource:4; } (engine/CTexture.h flattens the bitfields into byte arrays, so a local model as in the landed
//   CTexture users). The catalog's "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize" is propagated.
// Callees: vector::erase(first, last) 0x0049B3B0, vector::_M_fill_insert(pos, n, x) 0x0049B5A0 (catalog
//   "Std_Vector_Insert_Range_CTexture"), ~CTexture 0x009F9F70.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`: the default texture (two zero words) in the 8-byte frame, its address
//   pushed to the fill-insert, and its destructor on both exits.
struct IDirect3DTexture9;

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                     // +0x00
    unsigned long      ByteLength : 28;                 // +0x04 bits 0..27
    unsigned long      AllocationSource : 4;            // +0x04 bits 28..31
    CTexture() : PD3DTexture(0) { ByteLength = 0; AllocationSource = 0; }
    ~CTexture();                                        // 0x009F9F70
};

namespace _STL {
template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x0049B3B0
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x0049B5A0
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CTexture>::resize(unsigned int);
}