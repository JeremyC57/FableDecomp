// NParticleEngine::CParticleEmitterManager::GetActiveEmitterNames(vector<CCharString>& names) -- collect the distinct names of the live emitters with a count each, append " (n)"-style counts to the names, and return the total number of live emitters
// Leaf: ?GetActiveEmitterNames@CParticleEmitterManager@NParticleEngine@@QBEJAAV?$CArray@VCCharString@@@@@Z
// Naming wave 27 (family vl2, DECISIONS_W26 D6): GetActiveEmitterNames is const and takes a CArray<CCharString>& (Ego_r.pdb /
//   FableWin.pdb), CArray<T> over the STLport vector template whose __false_type grow-insert 0x00433530 takes the established
//   spelling. Was a non-const member over a CCharStringVector stand-in (?_M_insert_overflow@CCharStringVector@@..K_N).
// Naming wave 26 (family vlong): the vector is STLport 4.6 std::vector<long, allocator<long> >; its grow-insert 0x00440990 is the
//   protected ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the
//   own row's spelling, c689 queue). Was CLongVector::InsertGrow(..., const SScalarTag&, unsigned long, bool).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEBAF0, 0x192 bytes (padded to 416), `ret 4`.
// Callees: SGI __find for CCharString ranges 0x004B8E40 (__fastcall, random-access tag), CCharString copy 0x0099EC30,
//   vector<CCharString>::_M_insert_overflow 0x00433530, vector<long> insert-grow 0x00440990, GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600 (all
//   __fastcall with the hidden result in ecx), CCharString::operator+= 0x0099F0A0, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Layout: local model: the manager's emitter list (STLport list<CParticleEmitter*>, node pointer at +0); the emitter's
//   dead bit is bit 5 of the byte at +0x40 and its name the CCharString at +0x140.
// Shape: STLport inline push_back on both vectors (the landed CVTalentlessBardScript::ProcessSongsMarriage 0x00EDABA0
//   CCharString body and the PushBackConstant 0x00BBAC20 long body); the find tag and the pushed count live in the dead
//   argument slot. The three template callees (__find, both vector grow paths) are instantiations of this TU, so they are
//   defined here noinline: the find body is a counted loop that, like the real SGI __find, clobbers edx (the caller then
//   keeps `last` in esi), and the visible grow bodies give retail's 0x14 frame (this and the list iterator spilled).
//   find/end/begin are read straight from the vector (no first/last locals) and names[i] is STLport's *(begin() + i).
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);                           // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag);   // 0x004B8E40
// Same-TU instantiation (out of line in retail): a read-only counted scan, so VC7.1 knows the call writes no memory.
#pragma auto_inline(off)
__declspec(noinline) CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag)
{
    for (long n = last - first; n > 0; --n, ++first)
        if (first->m_pData == value.m_pData)
            return first;
    return last;
}
#pragma auto_inline(on)
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type is_POD_type; };   // STLport type_traits.h
template <> struct __type_traits<long> { typedef __true_type is_POD_type; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // STLport 4.6 vector (vector<long>, vector<CCharString>)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x00440990 (long)
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __false_type&, unsigned int fill_len, bool atend);     // 0x00433530 (CCharString)
public:
    void push_back(const _Tp& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(value);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, value, typename __type_traits<_Tp>::is_POD_type(), 1, true);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](unsigned long i) { return *(begin() + i); }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { public: };   // Fable's CArray over the STLport vector

extern "C" void* __cdecl malloc(unsigned int size);
// Same-TU instantiations of the two STLport grow paths (out of line in retail).
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* where, const _Tp& value, const std::__true_type&, unsigned int count, bool noTail)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize > count ? oldSize : count);
    long* start = (long*)malloc(len * sizeof(long));
    long* finish = start;
    for (long* p = _M_start; p != where; ++p)
        *finish++ = *p;
    for (unsigned long i = 0; i < count; ++i)
        *finish++ = value;
    if (!noTail)
        for (long* p = where; p != _M_finish; ++p)
            *finish++ = *p;
    if (_M_start)
        free(_M_start);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage = start + len;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* position, const _Tp& x, const std::__false_type&, unsigned int fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize > fill_len ? oldSize : fill_len);
    _Tp* start = (_Tp*)malloc(len * sizeof(_Tp));
    _Tp* finish = start;
    for (_Tp* p = _M_start; p != position; ++p, ++finish)
        new (finish) _Tp(*p);
    for (unsigned long i = 0; i < fill_len; ++i, ++finish)
        new (finish) _Tp(x);
    if (!atend)
        for (_Tp* p = position; p != _M_finish; ++p, ++finish)
            new (finish) _Tp(*p);
    for (_Tp* p = _M_start; p != _M_finish; ++p)
        p->~_Tp();
    if (_M_start)
        free(_M_start);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage = start + len;
}
#pragma auto_inline(on)
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad00[0x40];
    unsigned short Priority : 3;                        // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;                            // +0x40 bit 5
    unsigned short _bits6 : 10;
    unsigned char _pad42[0x140 - 0x42];
    CCharString   Name;                                 // +0x140
};
struct SEmitterNode {
    SEmitterNode*     _M_next;
    SEmitterNode*     _M_prev;
    CParticleEmitter* _M_data;
};
class CParticleEmitterManager { public:
    SEmitterNode* Emitters;                             // +0x00 list<CParticleEmitter*> node
    long GetActiveEmitterNames(CArray<CCharString>& names) const;   // const, CArray<CCharString> (Ego_r.pdb / FableWin.pdb)
};
long CParticleEmitterManager::GetActiveEmitterNames(CArray<CCharString>& names) const
{
    std::vector<long> counts;
    for (SEmitterNode* it = Emitters->_M_next; it != Emitters; it = it->_M_next) {
        CParticleEmitter* emitter = it->_M_data;
        if (emitter && !(bool)emitter->Dead) {
            CCharString* found = FindString_004b8e40(names._M_start, names._M_finish, emitter->Name, SRandomAccessIteratorTag());
            if (found == names._M_finish) {
                names.push_back(emitter->Name);
                counts.push_back(1);
            } else
                ++counts[found - names._M_start];
        }
    }
    long total = 0;
    for (unsigned long i = 0; i < names.size(); ++i) {
        total += counts[i];
        names[i] += " (" + GFIntToCharString(counts[i]) + ")";
    }
    return total;
}
}