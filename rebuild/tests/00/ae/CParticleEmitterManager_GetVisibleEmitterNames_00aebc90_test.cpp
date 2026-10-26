#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
extern "C" int __cdecl printf(const char* format, ...);
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
CCharString __fastcall IntToCharString(long value);                              // 0x0099F830
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
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* where) { return where; }
#else
inline void* operator new(__SIZE_TYPE__, void* where) { return where; }
#endif
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
        names[i] += " (" + IntToCharString(counts[i]) + ")";
    }
    return total;
}

// ---- behaviour-test stubs (strings live in an arena; destructors only count, so /Od copies are safe) ----
static char g_arena[1 << 16];
static unsigned int g_used;
static int g_dtors;
static char* NewText(unsigned int n) { char* p = g_arena + g_used; g_used += (n + 8) & ~7u; return p; }
static unsigned int Len(const char* s) { unsigned int n = 0; while (s[n]) ++n; return n; }
static const char* Txt(const CCharString& s) { return (const char*)s.PStringData; }
static CCharString Make(const char* a, const char* b)
{
    unsigned int la = Len(a), lb = Len(b), i;
    char* p = NewText(la + lb + 1);
    for (i = 0; i < la; ++i) p[i] = a[i];
    for (i = 0; i < lb; ++i) p[la + i] = b[i];
    p[la + lb] = 0;
    CCharString r; r.PStringData = p; return r;
}
CCharString& CCharString::operator+=(const CCharString& other) { CCharString t = Make(Txt(*this), Txt(other)); PStringData = t.PStringData; return *this; }
CCharString::~CCharString() { ++g_dtors; }
CCharString __fastcall IntToCharString(long value)
{
    char buf[16]; int n = 0; char tmp[16]; int k = 0;
    if (value == 0) tmp[k++] = '0';
    while (value > 0) { tmp[k++] = (char)('0' + value % 10); value /= 10; }
    while (k) buf[n++] = tmp[--k];
    buf[n] = 0;
    return Make(buf, "");
}
CCharString __fastcall operator+(const char* left, const CCharString& right) { return Make(left, Txt(right)); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return Make(Txt(left), right); }
bool __fastcall StringsEqual(const CCharString& a, const CCharString& b)
{
    const char* x = Txt(a); const char* y = Txt(b);
    while (*x && *x == *y) { ++x; ++y; }
    return *x == *y;
}
void CCharStringVector::push_back(const CCharString& value)
{
    if (_M_finish == _M_end_of_storage) {
        unsigned long n = size(), cap = n ? n * 2 : 1, i;
        CCharString* p = (CCharString*)malloc(cap * sizeof(CCharString));
        for (i = 0; i < n; ++i) p[i].PStringData = _M_start[i].PStringData;
        if (_M_start) free(_M_start);
        _M_start = p; _M_finish = p + n; _M_end_of_storage = p + cap;
    }
    _M_finish->PStringData = value.PStringData;
    ++_M_finish;
}
bool NParticleEngine::CParticleEmitter::IsVisible() { return _pad000[0] != 0; }

static int Same(const CCharString& s, const char* t) { CCharString w; w.PStringData = (void*)t; bool r = StringsEqual(s, w); w.PStringData = 0; return r; }

int main()
{
    // emitters: a b a c(dead) d(invisible) <null> b a e f g h i (12 visible names counted)
    static const char* label[] = { "a", "b", "a", "c", "d", 0, "b", "a", "e", "f", "g", "h", "i" };
    const int count = sizeof(label) / sizeof(label[0]);
    static NParticleEngine::CParticleEmitter emitters[16];
    static CEmitterListNode nodes[16];
    CEmitterListNode sentinel;
    CEmitterListNode* prev = &sentinel;
    int i;
    for (i = 0; i < count; ++i) {
        NParticleEngine::CParticleEmitter* e = 0;
        if (label[i]) {
            e = &emitters[i];
            e->_pad000[0] = (label[i][0] != 'd');
            e->Dead = (label[i][0] == 'c');
            e->Priority = 5; e->ContinuousEmitter = 1;
            e->Name.PStringData = (void*)label[i];
        }
        nodes[i]._M_data = e;
        nodes[i]._M_prev = prev;
        prev->_M_next = &nodes[i];
        prev = &nodes[i];
    }
    prev->_M_next = &sentinel;
    sentinel._M_prev = prev;
    CParticleEmitterManager manager;
    manager.Emitters._M_node = &sentinel;
    CCharStringVector names; names._M_start = names._M_finish = names._M_end_of_storage = 0;
    long total = manager.GetVisibleEmitterNames(names);
    int ok = 1;
    static const char* expect[] = { "a (3)", "b (2)", "e (1)", "f (1)", "g (1)", "h (1)", "i (1)" };
    if (total != 10) { ok = 0; printf("total %ld\n", total); }
    if (names.size() != 7) { ok = 0; printf("size %lu\n", (unsigned long)names.size()); }
    for (i = 0; ok && i < 7; ++i)
        if (!Same(names[i], expect[i])) { ok = 0; printf("name %d '%s'\n", i, Txt(names[i])); }
    // empty list
    CEmitterListNode empty; empty._M_next = empty._M_prev = &empty;
    manager.Emitters._M_node = &empty;
    CCharStringVector none; none._M_start = none._M_finish = none._M_end_of_storage = 0;
    if (manager.GetVisibleEmitterNames(none) != 0 || none.size() != 0) { ok = 0; printf("empty case\n"); }
    if (ok) printf("00aebc90_TEST PASS\n");
    return ok ? 0 : 1;
}