// _global::Copy_PairLongPairTextureDecal_00bee7b0(PairLongPairTextureDecal*, PairLongPairTextureDecal*, PairLongPairTextureDecal*, SRandomAccessIteratorTag, long*) -- SGI __copy over pair<pair<long,long>, CEngineTextureDecal>, returns the advanced result
// Leaf: Copy_PairLongPairTextureDecal_00bee7b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE7B0, length 119 bytes.
// Callees: vector<CCompositeBlendDesc>::operator= @ 0x006CADE0.
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in ecx,
// last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`
// with the signed count computed once; the advanced result is returned.
// Element (the manifest label is right): std::pair<std::pair<long,long>, CEngineTextureDecal> = 28 bytes in retail
// (`/ 28`): Ego_r.pdb CEngineTextureDecal = { long OriginalTextureIndex; long DecalID; vector<CCompositeBlendDesc>
// OverlaidBlends } is 24 bytes in the PDB but 20 in retail (STLport vectors are 12 bytes, not the PDB's 16). The
// implicit operator= moves the key pair and the two longs and calls the out-of-line vector operator= @ 0x006CADE0;
// the count lives in the dead `result` argument slot, src/dst and src+0x10/dst+0x10 are the two pointer pairs.
// Label correction: catalogued as `_Move_backward`, but the body walks forward (`add ebx,1Ch`): it is SGI __copy, the
// forward twin of 0x00BEE4E0.
class CCompositeBlendDesc;

template <class T>
struct CRetailVector {                           // STLport vector<T>: 12 bytes
    T* _M_start;                                 // +0
    T* _M_finish;                                // +4
    T* _M_end_of_storage;                        // +8
    CRetailVector& operator=(const CRetailVector& other);   // 0x006CADE0 for CCompositeBlendDesc
};

class CEngineTextureDecal { public:                     // Ego_r.pdb names; 20 bytes in retail
    long                                OriginalTextureIndex;   // +0x00
    long                                DecalID;                // +0x04
    CRetailVector<CCompositeBlendDesc>  OverlaidBlends;         // +0x08
};

struct SLongPair {                               // std::pair<long, long>
    long first;                                  // +0x00
    long second;                                 // +0x04
};

struct PairLongPairTextureDecal {                // std::pair<std::pair<long,long>, CEngineTextureDecal>, 28 bytes
    SLongPair           first;                   // +0x00
    CEngineTextureDecal second;                  // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairLongPairTextureDecal* __fastcall Copy_PairLongPairTextureDecal_00bee7b0(PairLongPairTextureDecal* first, PairLongPairTextureDecal* last, PairLongPairTextureDecal* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}