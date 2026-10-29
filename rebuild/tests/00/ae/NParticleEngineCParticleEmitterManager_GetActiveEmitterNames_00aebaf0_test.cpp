// Behaviour test for 00aebaf0: src/00aebaf0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. NParticleEngine::CParticleEmitterManager::GetActiveEmitterNames(vector<CCharString>&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NParticleEngine::CParticleEmitterManager::GetActiveEmitterNames(vector<CCharString>& names) -- collect the distinct names of the live emitters with a count each, append " (n)"-style counts to the names, and return the total number of live emitters
// Leaf: GetActiveEmitterNames
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
struct __false_type { __false_type() {} };
struct SScalarTag { SScalarTag() { } };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    CCharString* begin() { return _M_start; }
    CCharString& operator[](unsigned long i) { return *(begin() + i); }
};
struct CLongVector {                                    // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) long(value);
            ++_M_finish;
        } else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
    long& operator[](unsigned long i) { return _M_start[i]; }
};
extern "C" void* __cdecl malloc(unsigned int size);
// Same-TU instantiations of the two STLport grow paths (out of line in retail).
#pragma auto_inline(off)
__declspec(noinline) void CLongVector::InsertGrow(long* where, const long& value, const SScalarTag&, unsigned long count, bool noTail)
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
__declspec(noinline) void CCharStringVector::_M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize > fill_len ? oldSize : fill_len);
    CCharString* start = (CCharString*)malloc(len * sizeof(CCharString));
    CCharString* finish = start;
    for (CCharString* p = _M_start; p != position; ++p, ++finish)
        new (finish) CCharString(*p);
    for (unsigned long i = 0; i < fill_len; ++i, ++finish)
        new (finish) CCharString(x);
    if (!atend)
        for (CCharString* p = position; p != _M_finish; ++p, ++finish)
            new (finish) CCharString(*p);
    for (CCharString* p = _M_start; p != _M_finish; ++p)
        p->~CCharString();
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
    long GetActiveEmitterNames(CCharStringVector& names);
};
long CParticleEmitterManager::GetActiveEmitterNames(CCharStringVector& names)
{
    CLongVector counts;
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
// ---- stubs ----
// shared, reference-counted string data as the retail CCharString (copies share the data pointer)
struct SData { int refs; char text[60]; };
static int g_live;
static SData* NewData(const char* text) { SData* d = (SData*)malloc(sizeof(SData)); d->refs = 1; strcpy(d->text, text); ++g_live; return d; }
static void Release(void* p) { SData* d = (SData*)p; if (d && --d->refs == 0) { free(d); --g_live; } }
static const char* Text(const CCharString& s) { return ((SData*)s.m_pData)->text; }
static CCharString Make(const char* text) { CCharString* s = (CCharString*)malloc(sizeof(CCharString)); s->m_pData = NewData(text); CCharString r(*s); Release(s->m_pData); free(s); return r; }
CCharString::CCharString(const CCharString& other) { m_pData = other.m_pData; ++((SData*)m_pData)->refs; }
CCharString::~CCharString() { Release(m_pData); }
const CCharString& CCharString::operator+=(const CCharString& other)
{
    char buf[60]; strcpy(buf, Text(*this)); strcat(buf, Text(other));
    Release(m_pData); m_pData = NewData(buf); return *this;
}
CCharString __fastcall GFIntToCharString(long value) { char buf[16]; sprintf(buf, "%ld", value); return Make(buf); }
CCharString __fastcall operator+(const char* left, const CCharString& right) { char buf[60]; strcpy(buf, left); strcat(buf, Text(right)); return Make(buf); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { char buf[60]; strcpy(buf, Text(left)); strcat(buf, right); return Make(buf); }
using namespace NParticleEngine;
int main()
{
    static long ebuf[6][sizeof(CParticleEmitter) / 4]; memset(ebuf, 0, sizeof ebuf);
    CParticleEmitter* e = (CParticleEmitter*)ebuf;
    void* dA = NewData("Fire"); void* dB = NewData("Smoke"); void* dC = NewData("Spark");
    e[0].Name.m_pData = dA; e[1].Name.m_pData = dB; e[2].Name.m_pData = dA; e[3].Name.m_pData = dC; e[3].Dead = 1;
    e[4].Name.m_pData = dB; e[5].Name.m_pData = dA;
    ((SData*)dA)->refs += 2; ((SData*)dB)->refs += 1;   // the emitters hold their names
    CParticleEmitter* order[7] = { &e[0], &e[1], &e[2], 0, &e[3], &e[4], &e[5] };
    SEmitterNode head, nodes[7];
    SEmitterNode* prev = &head;
    for (int i = 0; i < 7; ++i) { nodes[i]._M_data = order[i]; prev->_M_next = &nodes[i]; nodes[i]._M_prev = prev; prev = &nodes[i]; }
    prev->_M_next = &head; head._M_prev = prev; head._M_data = 0;
    CParticleEmitterManager mgr; mgr.Emitters = &head;
    // empty result vector: names grow through the out-of-line path
    CCharStringVector names; names._M_start = names._M_finish = names._M_end_of_storage = 0;
    long total = mgr.GetActiveEmitterNames(names);
    CHECK(total == 5);
    CHECK(names.size() == 2);
    CHECK(strcmp(Text(names._M_start[0]), "Fire (3)") == 0);
    CHECK(strcmp(Text(names._M_start[1]), "Smoke (2)") == 0);
    CHECK(((SData*)dA)->refs == 3 && ((SData*)dB)->refs == 2);   // the original names are untouched; the vector holds new strings
    // spare capacity: names copied in place; no live emitters gives an empty result
    CCharStringVector room; room._M_start = (CCharString*)malloc(4 * sizeof(CCharString)); room._M_finish = room._M_start; room._M_end_of_storage = room._M_start + 4;
    CCharString* roomStart = room._M_start;
    total = mgr.GetActiveEmitterNames(room);
    CHECK(total == 5 && room._M_start == roomStart && room.size() == 2);
    CHECK(strcmp(Text(room._M_start[0]), "Fire (3)") == 0 && strcmp(Text(room._M_start[1]), "Smoke (2)") == 0);
    for (int i = 0; i < 6; ++i) e[i].Dead = 1;
    CCharStringVector none; none._M_start = none._M_finish = none._M_end_of_storage = 0;
    CHECK(mgr.GetActiveEmitterNames(none) == 0 && none.size() == 0);
    // empty list
    head._M_next = head._M_prev = &head;
    CHECK(mgr.GetActiveEmitterNames(none) == 0 && none.size() == 0);
    if (fails == 0) printf("00aebaf0_TEST PASS\n"); else printf("00aebaf0_TEST FAIL\n");
    return fails;
}