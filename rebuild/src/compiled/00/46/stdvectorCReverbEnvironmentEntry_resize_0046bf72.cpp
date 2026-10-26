// std::vector<CReverbEnvironmentEntry>::resize(unsigned int newSize) -- STLport one-argument resize: forward to resize(newSize, T()) with a zero-filled reverb environment
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046BF72, 38 bytes, `ret 4` (__fastcall: the vector in ecx, newSize on the stack).
// Identity: the catalog label "$E1" is a stray; the only caller is GFSerialiseVectorBinaryIn<CReverbEnvironmentEntry>
//   @ 0x0046BE89 (`out.resize(count)`), and the callee 0x0046BF98 is the two-argument resize(n, const T&) (landed under a
//   propagated "CArray<pair>::push_back" label: `n < size() ? erase(begin() + n, end()) : insert(end(), n - size(), x)`).
// Callees: vector<CReverbEnvironmentEntry>::resize(unsigned int, const CReverbEnvironmentEntry&) @ 0x0046BF98.
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }`: `_Tp()` value-initialises the 21-float POD in the
//   frame (`push 15h ; pop ecx ; xor eax,eax ; rep stosd`, `this` parked in edx across the fill). Size TU with a frame
//   pointer: `optimize("y", off)` + `("s", on)`.
// Model: STLport's three vector words; CReverbEnvironmentEntry (0x54 bytes, PDB: float Params[21]) is local (no engine header).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CReverbEnvironmentEntry { public:
    float Params[21];                                       // +0x00
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize, const T& x);         // 0x0046BF98
    void resize(unsigned int newSize);
};
template <class T> void vector<T>::resize(unsigned int newSize)
{
    resize(newSize, T());
}
template void vector<CReverbEnvironmentEntry>::resize(unsigned int newSize);
}