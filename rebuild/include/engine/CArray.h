#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// CArray<T> (Codex pass 11 consolidation, one model): Lionhead's array is a public STLport 4.6 vector<T> (12 bytes in retail;
// the donor PDB's later vector base is 16) adding no fields. Ego_r CArray<C3DVector> / CArray<long>: unsigned-int index
// operators returning T& and const T&, the (n) and (n, value) constructors, GetAsCArray over a private zero-size array.
#include "stlport/fable_containers.h"
template<class T> class CArray : public std::vector<T> {
public:
    CArray() : std::vector<T>() { }
#ifdef FABLE_RETAIL_CARRAY
    // retry11a (row 0x00520670 CArray<unsigned char>(size)): the one-argument ctor forwards to STLport's vector(n) -- the
    // vector(n, T()) form changes LoadHeroStateBinary's register allocation (1 instruction).
    explicit CArray(unsigned int n) : std::vector<T>(n) { }
#else
    explicit CArray(unsigned int n) : std::vector<T>(n,T()) { }
#endif
    CArray(unsigned int n,const T& x) : std::vector<T>(n,x) { }
#ifdef FABLE_RETAIL_CARRAY
    // Retail opt-in (chunk 723, row 0x00BC14E0): the index operators go through begin() like STLport's vector::operator[]; the
    // element offset then folds into the addressing mode of the inlined CVertexShader::operator= (`[esi+0Ch]`), where the
    // `_M_start[i]` form materialises the element address first (`add esi,8`).
    T& operator[](unsigned int i) { return *(this->begin() + i); }
    const T& operator[](unsigned int i) const { return *(this->begin() + i); }
#else
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
#endif
#ifdef FABLE_RETAIL_CARRAY_GETASCARRAY_START
    // Nested opt-in (chunk 762, row 0x00B847D0 RadixSort): the non-const form reads _M_start like the const one (the landed
    // 0x00BB1EF0 CEngineClothRenderer::FrameStart model); `&front()` makes VC7.1 select into a register instead of
    // storing both arms of the conditional to the spill slot (`mov [esp+10h],eax ; ja ; mov [esp+10h],edi`).
    T* GetAsCArray() { return this->size()>0 ? this->_M_start : GetZeroSizeArray(); }
#else
    T* GetAsCArray() { return this->size()>0 ? &this->front() : GetZeroSizeArray(); }
#endif
    const T* GetAsCArray() const { return this->size()>0 ? this->_M_start : (const T*)this; }
private:
    T* GetZeroSizeArray() { return (T*)this; }
};
#endif
