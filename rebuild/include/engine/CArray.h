#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H
// CArray<T> (Codex pass 11 consolidation, one model): Lionhead's array is a public STLport 4.6 vector<T> (12 bytes in retail;
// the donor PDB's later vector base is 16) adding no fields. Ego_r CArray<C3DVector> / CArray<long>: unsigned-int index
// operators returning T& and const T&, the (n) and (n, value) constructors, GetAsCArray over a private zero-size array.
#include "stlport/fable_containers.h"
template<class T> class CArray : public std::vector<T> {
public:
    CArray() : std::vector<T>() { }
    explicit CArray(unsigned int n) : std::vector<T>(n,T()) { }
    CArray(unsigned int n,const T& x) : std::vector<T>(n,x) { }
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
    T* GetAsCArray() { return this->size()>0 ? &this->front() : GetZeroSizeArray(); }
    const T* GetAsCArray() const { return this->size()>0 ? this->_M_start : (const T*)this; }
private:
    T* GetZeroSizeArray() { return (T*)this; }
};
#endif
