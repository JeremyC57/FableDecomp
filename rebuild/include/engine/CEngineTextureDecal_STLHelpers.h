#ifndef FABLE_CENGINETEXTUREDECAL_STLHELPERS_H
#define FABLE_CENGINETEXTUREDECAL_STLHELPERS_H
#include "engine/CEngineTextureDecal.h"
// Fidelity: scaffolding, not retail. Preserve the genuine library bodies in their native owner scopes.
// No row-head pragma may precede this header; compiler defaults are restored below.
#pragma optimize("s",on)
namespace std {
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
CCompositeBlendDesc* __fastcall __uninitialized_copy(const CCompositeBlendDesc* first,const CCompositeBlendDesc* last,CCompositeBlendDesc* result,const __false_type&) {
 CCompositeBlendDesc* cur=result;
 for(;first!=last;++first,++cur)_Construct(&*cur,*first);
 return cur;
}
#if defined(FABLE_RETAIL_STLP_BLEND_VECTOR_COPY_OWNERS_PASS26) && defined(_MSC_VER)
// Fidelity: scaffolding, not retail. The storage owner 005BAA6B uses size mode;
// the vector copy at 005BB346 also preserves a frame pointer. The allocator's
// own copy retains command-line optimization. Place any row-head pragma after
// this include, which restores optimize("", on).
#pragma optimize("", on)
template<> FABLE_FORCEINLINE CCompositeBlendDesc* allocator<CCompositeBlendDesc>::allocate(size_t n, const void*)
{
    return n ? static_cast<CCompositeBlendDesc*>(malloc(n * sizeof(CCompositeBlendDesc))) : 0;
}

#pragma optimize("s", on)
template<>
inline _Vector_base<CCompositeBlendDesc, allocator<CCompositeBlendDesc> >::_Vector_base(size_t n, const allocator<CCompositeBlendDesc>& a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
{
    _M_start = _M_end_of_storage.allocate(n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + n;
}

#pragma optimize("y", off)
template<>
inline vector<CCompositeBlendDesc, allocator<CCompositeBlendDesc> >::vector(const vector& other)
    : _Vector_base<CCompositeBlendDesc, allocator<CCompositeBlendDesc> >(other.size(), other.get_allocator())
{
    this->_M_finish = __uninitialized_copy(static_cast<const CCompositeBlendDesc*>(other._M_start),
        static_cast<const CCompositeBlendDesc*>(other._M_finish), this->_M_start, __type_traits<CCompositeBlendDesc>::is_POD_type());
}
#endif // FABLE_RETAIL_STLP_BLEND_VECTOR_COPY_OWNERS_PASS26
}
#pragma optimize("",on)
#ifdef FABLE_RETAIL_CENGINETEXTUREDECAL_INLINE_GENERATED_COPY_PASS26
// Fidelity scaffolding. Spell the real memberwise copy to preserve the native expansion.
FABLE_FORCEINLINE CEngineTextureDecal::CEngineTextureDecal(const CEngineTextureDecal& other)
    : OriginalTextureIndex(other.OriginalTextureIndex), DecalID(other.DecalID), OverlaidBlends(other.OverlaidBlends)
{
}
#endif // FABLE_RETAIL_CENGINETEXTUREDECAL_INLINE_GENERATED_COPY_PASS26
#if defined(FABLE_RETAIL_STLP_DECAL_PAIR_INLINE_PASS26) && defined(_MSC_VER)
namespace std {
// Inline-budget: 00BECDE0 and its native range helpers expand the genuine pair's
// two member constructions in both insertion paths.
typedef pair<pair<long, long>, CEngineTextureDecal> TextureDecalEntry;


#ifdef FABLE_RETAIL_STLP_DECAL_CLEAR_EXPANSION_PASS26
// Inline-budget: the out-of-line native clear expands the original non-POD destruction loop.
template<> FABLE_FORCEINLINE void __destroy_aux(TextureDecalEntry* first, TextureDecalEntry* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
#endif // FABLE_RETAIL_STLP_DECAL_CLEAR_EXPANSION_PASS26

template<>
FABLE_FORCEINLINE TextureDecalEntry::pair(const pair<long, long>& key, const CEngineTextureDecal& value)
    : first(key), second(value)
{
}

template<>
FABLE_FORCEINLINE TextureDecalEntry::pair(const pair& other)
    : first(other.first), second(other.second)
{
}
#ifdef FABLE_RETAIL_STLP_DECAL_CONSTRUCT_INLINE_PASS26
// Inline-budget: native placement expands _Construct and the pair/decal copies.
template<> FABLE_FORCEINLINE void FABLE_FASTCALL _Construct(TextureDecalEntry* position, const TextureDecalEntry& value)
{
    new (static_cast<void*>(position)) TextureDecalEntry(value);
}
#endif // FABLE_RETAIL_STLP_DECAL_CONSTRUCT_INLINE_PASS26
#ifdef FABLE_RETAIL_STLP_DECAL_PUSHBACK_INLINE_PASS26
// Inline-budget: the retail Insert expands vector::push_back and its placement path.
template<>
FABLE_FORCEINLINE void vector<TextureDecalEntry>::push_back(const TextureDecalEntry& value)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, value);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish, value, _IsPODType(), 1, true);
    }
}
#endif // FABLE_RETAIL_STLP_DECAL_PUSHBACK_INLINE_PASS26
}
#endif // FABLE_RETAIL_STLP_DECAL_PAIR_INLINE_PASS26
#endif
