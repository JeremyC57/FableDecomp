// C3DMeshFileXAnimatedPrimitiveBlockChunk::AddVertex(unsigned long vertex, const CArray<C3DIndexedBoneWeight>& weights) -- append a vertex index to the block's vertex list and each of its bone weights to the bone-weight array
// Leaf: ?AddVertex@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA65A0, 152 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb `?AddVertex@C3DMeshFileXAnimatedPrimitiveBlockChunk@@QAEXKABV?$CArray@VC3DIndexedBoneWeight@@@@@Z`;
//   no direct caller in retail (an unreferenced out-of-line copy). The targets are VertexList +0x48 (4-byte elements) and
//   BoneWeights +0x54 (8-byte C3DIndexedBoneWeight {BoneIndex, Weight}), the retail offsets the landed destructor
//   0x00AA7480 frees. The catalog's "ParticleEffectParameter_InsertData" is a guess.
// Callees: vector<unsigned long>::_M_insert_overflow 0x00464ACE and vector<C3DIndexedBoneWeight>::_M_insert_overflow
//   0x00A96C20 (pos, x, tag, 1, true each).
// Layout (retail, STLport 12-byte arrays): CChunkedFileSuperChunk base 0x2C bytes, GroupBlock +0x2C (0x18),
//   BonesPerVertex +0x44, VertexList +0x48, BoneWeights +0x54 -- the engine header is the Ego_r layout, so modelled locally.
// Shape: STLport `VertexList.push_back(vertex)` inline (the value store under the null test, `add [ecx+4],4`; the overflow
//   takes &vertex and homes the empty tag in that same slot), then `for (unsigned long i = 0; i < weights.Size(); ++i)
//   BoneWeights.push_back(weights[i]);` with the member pointer hoisted (`add esi,54h`), Size() re-read after every push
//   (`sar ecx,3`, unsigned `jb`) and &weights[i] formed before the capacity test.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // stand-in body only
extern "C" void __cdecl free(void* block);              // stand-in body only

class C3DIndexedBoneWeight {
public:
    unsigned long BoneIndex;                            // +0x00
    unsigned long Weight;                               // +0x04
};

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> struct _Is_POD { typedef __false_type _Ret; };
template <> struct _Is_POD<unsigned long> { typedef __true_type _Ret; };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef typename _Is_POD<_Tp>::_Ret _IsPODType;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const _IsPODType&, unsigned int __fill_len, bool __atend);   // 0x00464ACE / 0x00A96C20 (same-TU body below)
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    const _Tp& operator[](unsigned int __n) const { return this->_M_start[__n]; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};

// Same-TU stand-in for the growth paths 0x00464ACE / 0x00A96C20: retail homes the empty tag in the LIVE `vertex` argument
// slot (the same address it passes as `x`), which VC7.1 only does when the callee's body is visible and never touches
// the tag; with a declaration only it gives the tag its own frame byte (`push ecx`).
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const _IsPODType&, unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = this->_M_finish - this->_M_start;
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = this->_M_start; __p != __position; ++__p, ++__new_finish)
        new (__new_finish) _Tp(*__p);
    for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish)
        new (__new_finish) _Tp(__x);
    if (!__atend)
        for (_Tp* __q = __position; __q != this->_M_finish; ++__q, ++__new_finish)
            new (__new_finish) _Tp(*__q);
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)
}

template <class T> class CArray : public std::vector<T> {     // retail CArray<T>: the 12-byte STLport vector
public:
    unsigned long Size() const { return (unsigned long)this->size(); }
};

class CChunkedFileSuperChunk {                          // retail 0x2C bytes
public:
    virtual ~CChunkedFileSuperChunk();
    unsigned char _body[0x28];                          // +0x04 chunk header, SubChunks, RegisteredSubChunkTypes, CurrentChunkIndex
};

class CVertexGroupBlock {                               // retail 0x18 bytes
public:
    unsigned char _body[0x18];
};

class C3DMeshFileXAnimatedPrimitiveBlockChunk : public CChunkedFileSuperChunk {
public:
    CVertexGroupBlock               GroupBlock;         // +0x2c
    unsigned long                   BonesPerVertex;     // +0x44
    CArray<unsigned long>           VertexList;         // +0x48
    CArray<C3DIndexedBoneWeight>    BoneWeights;        // +0x54
    void AddVertex(unsigned long vertex, const CArray<C3DIndexedBoneWeight>& weights);
};

void C3DMeshFileXAnimatedPrimitiveBlockChunk::AddVertex(unsigned long vertex, const CArray<C3DIndexedBoneWeight>& weights)
{
    VertexList.push_back(vertex);
    for (unsigned long i = 0; i < weights.Size(); ++i)
        BoneWeights.push_back(weights[i]);
}