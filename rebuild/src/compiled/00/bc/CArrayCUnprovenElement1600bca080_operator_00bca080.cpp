// CArray<CUnprovenElement16_00bca080>::operator[](unsigned int i) -- address of element i of a CArray (STLport vector) of 16-byte elements
// Leaf: ??A?$CArray@UCUnprovenElement16_00bca080@@@@QAEAAUCUnprovenElement16_00bca080@@I@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA080, 14 bytes, thiscall, `ret 4`: `_M_start + i*16`.
// Identity (descriptive): an out-of-line CArray<T>::operator[] COMDAT for a 16-byte element type; no direct call reaches it
//   (the xref hits are bytes inside other instructions), so the element type is not recoverable from a caller. The class is
//   named by its address; the 0x00BCA090 neighbour is the same accessor shape for another instantiation.
struct CUnprovenElement16_00bca080 {           // 16 bytes (the `shl eax,4` stride)
    unsigned long Word[4];
};

#include "engine/CArray.h"

template CUnprovenElement16_00bca080& CArray<CUnprovenElement16_00bca080>::operator[](unsigned int);