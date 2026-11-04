// C3DAnimationBuilder::C3DAnimationBuilder() -- CBaseClassNonCopyable base, this class's vtable, an empty AddedBonePrefixes string vector (+4), then the two default bone prefixes "x" and "sub" pushed into it
// Leaf: ??0C3DAnimationBuilder
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB1660, length 219 bytes (+5 int3), plain `ret`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString(const char*, long) @ 0x0099EBF0,
//   CCharString(const CCharString&) @ 0x0099EC30 (the placement copy), vector<CCharString>::_M_insert_overflow
//   @ 0x00433530, ~CCharString @ 0x0099EAE0; literals "x" @ 0x0123089C and "sub" @ 0x0129E9C0.
// Identity: vtable 0x0129E9C8 = C3DAnimationBuilder (rtti-vtables.tsv; slot 0 = the landed deleting destructor
//   0x00AB3550); Ego_r.pdb `??0C3DAnimationBuilder@@QAE@XZ` and the member AddedBonePrefixes (CArray<CCharString>).
//   The manifest's "CParticleMorphs_InitializeEntry" is propagated and wrong.
// Shape: the vtable store precedes the vector's three zero stores (STLport `_Vector_base(allocator_type())` member
//   constructor); each prefix is an implicit CCharString conversion temporary (re-addressed with `lea` for the
//   placement copy and the overflow call) handed to the inlined STLport push_back; the empty __false_type tag gets the
//   high byte of the second frame word; VC7.1 tail-duplicates the last temporary's destructor and the return.

class CCharString {
public:
    void* PStringData;                             // +0x00
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};

class CBaseClassNonCopyable {
protected:                                         // ctor and virtual dtor are protected (IAE / MAE)
    CBaseClassNonCopyable();                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

#include "engine/CArray.h"


class C3DAnimationBuilder : public CBaseClassNonCopyable {     // vtable 0x0129E9C8
public:
    CArray<CCharString> AddedBonePrefixes;         // +0x04
    C3DAnimationBuilder();
    virtual ~C3DAnimationBuilder();
};

C3DAnimationBuilder::C3DAnimationBuilder()
{
    AddedBonePrefixes.push_back("x");
    AddedBonePrefixes.push_back("sub");
}