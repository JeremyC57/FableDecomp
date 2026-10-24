// _global::UninitCopy_ParamDescPair_00b0f670(const CParamDescPair* first, const CParamDescPair* last, CParamDescPair* result, const __false_type&) -- STLport __uninitialized_copy over pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_ParamDescPair_00b0f670
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0F670, length 64 bytes, `ret 8`.
// Callers: 0x00B0FA45 / 0x00B0FA9F in the vector reallocation 0x00B0F9F0 -- the same caller as the landed fill twin
//   `__uninitialized_fill_n` 0x00B0F6B0 (the next function in this TU), so the same 16-byte element.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. Vtable
//   0x0125FEE4 is CParticleUpdateParamDesc (rtti-vtables.tsv); Ego_r.pdb NParticleEngine::CParticleUpdateParamDesc
//   <unsigned long, unsigned long> is 12 bytes (vptr, Callback +4, Index +8) and is the mapped type of the CVectorMap
//   <unsigned long, CParticleUpdateParamDesc, ...>, whose storage is a vector of pair<unsigned long, desc>.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack). The pair's memberwise copy constructor over a member with its own
//   memberwise user copy constructor keeps second induction registers at +0x0C (`lea esi,[eax+0Ch]` / `lea edi,[ecx+0Ch]`),
//   the desc's copy stores its vtable (`mov [esi-8],imm`) between the key and its members (as in 0x00B0F6B0);
//   placement-new null test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CParticleUpdateParamCallback;            // CParticleUpdateParamCallback<unsigned long, unsigned long>

struct CParticleUpdateParamDesc {               // NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long>, vtable 0x0125FEE4
    CParticleUpdateParamCallback* Callback;     // +0x04
    unsigned long                 Index;        // +0x08
    CParticleUpdateParamDesc(const CParticleUpdateParamDesc& o) : Callback(o.Callback), Index(o.Index) { }
    virtual void v00();
};

struct CParamDescPair {                         // _STL::pair<unsigned long, CParticleUpdateParamDesc>, 16 bytes
    unsigned long            first;             // +0x00
    CParticleUpdateParamDesc second;            // +0x04
    CParamDescPair(const CParamDescPair& o) : first(o.first), second(o.second) { }
};

CParamDescPair* __fastcall UninitCopy_ParamDescPair_00b0f670(const CParamDescPair* first, const CParamDescPair* last, CParamDescPair* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CParamDescPair(*first);
    return result;
}