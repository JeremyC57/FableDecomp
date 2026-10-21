// Grade: DIFFER(375v385) -- frame/temporaries: retail homes the push_back value and the overflow tag in the dead `names` argument slot (ours in two extra frame words), does not reload _M_finish after the placement store, tests the hidden bit as `mov al ; shr al,5 ; test al,1` (ours `test byte ptr`), and uses load/inc/store for the count bump; loop/iterator/find structure matches
// CParticleEmitterManager::GetVisibleEmitterNames(vector<CCharString>& names) -- collect the names of the visible particle emitters, counting duplicates, then append " (count)" to each name; returns the number of visible emitters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEBC90, length 383 bytes (+ int3 padding), `ret 4`.
// Callees: CParticleEmitter::IsVisible 0x00AED010, the SGI __find for CCharString ranges 0x004B8E40 (`__fastcall`, as the
// landed CQuestManager::SetCreatureGenerationEnabledDuringScript 0x004B3810), vector<CCharString>::push_back
// 0x0044BFF0, vector<long>::_M_insert_overflow 0x00440990, IntToCharString(long) 0x0099F830, operator+(const char*,
// const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600, CCharString::operator+=
// 0x0099F0A0, ~CCharString 0x0099EAE0, free 0x00BFEA14 (the local vector's storage).
// Strings: " (" 0x01244E6C, ")" 0x0122E020.
// Layout: CParticleEmitterManager's first member is the STLport list of emitters (the sentinel node pointer; nodes are
// {next, prev, CParticleEmitter*}); CParticleEmitter flag byte +0x40 (bit 5: hidden, descriptive), Name +0x140.
// Shape: a local STLport vector<long> of counts (three zeroed words, freed at the end); the find runs from the names'
// start to a cached finish; the count push_back is the inline STLport body (null-tested construct, overflow with
// `__true_type`, 1, true; the value and the tag homed in the dead `names` argument slot); the suffix is
// `" (" + IntToCharString(count) + ")"` appended with `+=`, the count load shared with the running total.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 (MSVCR71)

struct CCharString {
    void* PStringData;
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall IntToCharString(long value);                              // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
CCharString* __fastcall FindString(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag);   // 0x004B8E40

struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCharString& operator[](unsigned long n) { return _M_start[n]; }
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
    void _M_insert_overflow(long* position, const long& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00440990
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) long(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char _pad000[0x40];
    unsigned char _flags0 : 5;
    unsigned char Hidden : 1;                           // +0x40 bit 5 (descriptive)
    unsigned char _flags6 : 2;
    unsigned char IsHidden() const { return Hidden; }
    unsigned char _pad041[0x140 - 0x41];
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
        if (*it && !(*it)->IsHidden() && (*it)->IsVisible()) {
            CCharString* first = names._M_start;
            CCharString* last = names._M_finish;
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
    for (unsigned long i = 0; i < names.size(); ++i) {
        total += counts[i];
        names[i] += " (" + IntToCharString(counts[i]) + ")";
    }
    return total;
}
