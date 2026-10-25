// _STL::vector::resize(unsigned int new_size) -- STLport one-argument resize of the file installer's buffer-chunk vector: resize(n, CBufferChunk()) with the empty chunk destroyed on both exits
// Leaf: ?resize@?$vector@VCBufferChunk@CFileInstaller@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C7850, 103 bytes (+ int3 padding), `__fastcall` (this stays in ecx), `ret 4`.
// Identity: element CFileInstaller::CBufferChunk (Ego_r.pdb { uchar* Mem; bool Used; bool IsInput; }, 8 bytes; the
//   BufferChunks member is CArray<CFileInstaller::CBufferChunk>, whose resize is the inherited vector one); the one caller
//   0x009C532D is the landed CFileInstaller::InitBuffers 0x009C5310, the erase callee the landed
//   vector<CFileInstaller::CBufferChunk>::erase 0x009C7150, the temporary's destructor the landed ~CBufferChunk 0x009C6020.
//   The default constructor zeroes Mem and Used only (`mov [t],eax ; mov byte [t+4],al`). The catalog's
//   "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize" is propagated.
// Callees: vector::erase(first, last) 0x009C7150, vector::_M_fill_insert(pos, n, x) 0x009C75A0, ~CBufferChunk 0x009C6020.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`: the default element in the 8-byte frame (Mem and Used zeroed between the size loads), its address pushed
//   to the fill-insert, and its destructor on both exits.
class CFileInstaller {
public:
    class CBufferChunk {                                // Ego_r.pdb, 8 bytes
    public:
        unsigned char* Mem;                             // +0x00
        bool           Used;                            // +0x04
        bool           IsInput;                         // +0x05
        CBufferChunk() : Mem(0), Used(false) { }
        ~CBufferChunk();                                // 0x009C6020
    };
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
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x009C7150
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x009C75A0
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

template void vector<CFileInstaller::CBufferChunk>::resize(unsigned int);
}