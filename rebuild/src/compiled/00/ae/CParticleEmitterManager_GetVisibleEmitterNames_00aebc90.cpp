// CParticleEmitterManager::GetVisibleEmitterNames(vector<CCharString>& names) -- collect the names of the visible particle emitters, counting duplicates, then append " (count)" to each name; returns the number of visible emitters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEBC90, length 383 bytes (+ int3 padding), `ret 4`.
// Callees: CParticleEmitter::IsVisible 0x00AED010, the SGI __find for CCharString ranges 0x004B8E40 (`__fastcall`, as the
// landed CQuestManager::SetCreatureGenerationEnabledDuringScript 0x004B3810), vector<CCharString>::push_back
// 0x0044BFF0, vector<long>::_M_insert_overflow 0x00440990, IntToCharString(long) 0x0099F830, operator+(const char*,
// const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600, CCharString::operator+=
// 0x0099F0A0, ~CCharString 0x0099EAE0, free 0x00BFEA14 (the local vector's storage).
// Strings: " (" 0x01244E6C, ")" 0x0122E020.
// Layout: CParticleEmitterManager's first member is the STLport list of emitters (the sentinel node pointer; nodes are
// {next, prev, CParticleEmitter*}); CParticleEmitter ushort bitfield group +0x40 (bit 5 Dead, PDB names; PDB offsets are +0xC: Ego_r's vector is 16 bytes), Name +0x140.
// Shape: a local STLport vector<long> of counts (three zeroed words, freed at the end); the find runs from the names'
// start to a cached finish; the count push_back is the inline STLport body (null-tested construct, overflow with
// `__true_type`, 1, true; the value and the tag homed in the dead `names` argument slot); the suffix is
// `" (" + IntToCharString(count) + ")"` appended with `+=`, the count load shared with the running total.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 (MSVCR71)

class CCharString { public:
    void* PStringData;
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long);                              // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
bool __fastcall StringsEqual(const CCharString& a, const CCharString& b);   // CCharString operator==
// Same-TU body of the SGI __find<CCharString*> instantiation 0x004B8E40 (retail instantiated it in this TU), tag unused.
template <class It, class T, class Tag> __declspec(noinline) It __fastcall FindString(It first, It last, const T& value, const Tag&)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (StringsEqual(*first, value)) return first;
        ++first;
        if (StringsEqual(*first, value)) return first;
        ++first;
        if (StringsEqual(*first, value)) return first;
        ++first;
        if (StringsEqual(*first, value)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (StringsEqual(*first, value)) return first;
        ++first;
    case 2:
        if (StringsEqual(*first, value)) return first;
        ++first;
    case 1:
        if (StringsEqual(*first, value)) return first;
    case 0:
    default:
        break;
    }
    return last;
}

struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCharString* begin() { return _M_start; }
    CCharString& operator[](unsigned long n) { return *(begin() + n); }
    void push_back(const CCharString& value);           // 0x0044BFF0
};

struct __true_type { __true_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CLongVector {                                    // STLport vector<long> (12 bytes, malloc storage)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CLongVector() { if (_M_start) free(_M_start); }
    long& operator[](unsigned long n) { return _M_start[n]; }
    template <class Tag> void _M_insert_overflow(long* position, const long& x, const Tag&, unsigned long fill_len, bool atend);   // 0x00440990
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) long(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);
// Same-TU body of vector<long>::_M_insert_overflow 0x00440990 (retail instantiated it in this TU), tag unused.
template <class Tag> __declspec(noinline) void CLongVector::_M_insert_overflow(long* position, const long& x, const Tag&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
    long* newStart = len ? (long*)malloc(len * sizeof(long)) : 0;
    long* newFinish = (long*)((char*)memmove(newStart, _M_start, (char*)position - (char*)_M_start) + ((char*)position - (char*)_M_start));
    for (unsigned long i = fill_len; i != 0; --i)
        *newFinish++ = x;
    if (!atend) {
        memmove(newFinish, position, (char*)_M_finish - (char*)position);
        newFinish += _M_finish - position;
    }
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}

namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad000[0x40];
    unsigned short Priority : 3;                        // +0x40 (PDB ushort bitfield group; PDB +0x4C, retail vector is 12 bytes)
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;                            // +0x40 bit 5
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short _flags8 : 8;
    bool IsDead() const { return Dead; }
    unsigned char _pad042[0x140 - 0x42];
    CCharString   Name;                                 // +0x140
    bool IsVisible();                                   // 0x00AED010
};
}

struct CEmitterListNode {                               // STLport list node
    CEmitterListNode*                  _M_next;
    CEmitterListNode*                  _M_prev;
    NParticleEngine::CParticleEmitter* _M_data;
};

struct CEmitterListIterator {                           // STLport _List_iterator
    CEmitterListNode* _M_node;
    CEmitterListIterator(CEmitterListNode* node) : _M_node(node) {}
    NParticleEngine::CParticleEmitter*& operator*() const { return _M_node->_M_data; }
    CEmitterListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CEmitterListIterator& other) const { return _M_node != other._M_node; }
};
struct CEmitterList {                                   // STLport list<CParticleEmitter*> (the sentinel pointer)
    CEmitterListNode* _M_node;
    CEmitterListIterator begin() { return CEmitterListIterator(_M_node->_M_next); }
    CEmitterListIterator end() { return CEmitterListIterator(_M_node); }
};

struct CParticleEmitterManager {
    CEmitterList Emitters;                              // +0x00
    long GetVisibleEmitterNames(CCharStringVector& names);
};

long CParticleEmitterManager::GetVisibleEmitterNames(CCharStringVector& names)
{
    CLongVector counts;
    for (CEmitterListIterator it = Emitters.begin(); it != Emitters.end(); ++it) {
        if (*it && !(*it)->IsDead() && (*it)->IsVisible()) {
            CCharString* last = names._M_finish;
            CCharString* first = names._M_start;
            const CCharString& name = (*it)->Name;
            CCharString* found = FindString(first, last, name, SRandomAccessIteratorTag());
            if (found == last) {
                names.push_back(name);
                counts.push_back(1);
            } else
                counts[found - first]++;
        }
    }
    long total = 0;
    for (int i = 0; i < names.size(); ++i) {
        total += counts[i];
        names[i] += " (" + GFIntToCharString(counts[i]) + ")";
    }
    return total;
}