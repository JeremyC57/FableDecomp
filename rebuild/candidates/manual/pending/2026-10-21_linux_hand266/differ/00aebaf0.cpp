// Grade: DIFFER(398v402) -- register allocation only: retail spills the list iterator to the frame (0x14 frame, this spilled, edi=names, ebx=&emitter->Name) where the lane keeps the iterator in ebx (0x18 frame). The dead-bit test now matches: a ushort bitfield read through (bool)Dead gives retail's byte load + shr 5 + test 1 (a plain if(Dead) folds to test byte,20h). A named const CCharString& name local changed nothing.
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
//   argument slot.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);                           // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag);   // 0x004B8E40
// Stand-in body (out of line in retail): a read-only scan, so VC7.1 knows the call writes no memory.
#pragma auto_inline(off)
__declspec(noinline) CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag)
{
    for (; first != last; ++first)
        if (first->m_pData == value.m_pData)
            break;
    return first;
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
    CCharString& operator[](unsigned long i) { return _M_start[i]; }
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
namespace NParticleEngine {
struct CParticleEmitter {
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
struct CParticleEmitterManager {
    SEmitterNode* Emitters;                             // +0x00 list<CParticleEmitter*> node
    long GetActiveEmitterNames(CCharStringVector& names);
};
long CParticleEmitterManager::GetActiveEmitterNames(CCharStringVector& names)
{
    CLongVector counts;
    for (SEmitterNode* it = Emitters->_M_next; it != Emitters; it = it->_M_next) {
        CParticleEmitter* emitter = it->_M_data;
        if (emitter && !(bool)emitter->Dead) {
            CCharString* last = names._M_finish;
            CCharString* first = names._M_start;
            CCharString* found = FindString_004b8e40(first, last, emitter->Name, SRandomAccessIteratorTag());
            if (found == last) {
                names.push_back(emitter->Name);
                counts.push_back(1);
            } else
                ++counts[found - first];
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
