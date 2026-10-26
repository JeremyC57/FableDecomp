// CEngineManager::AddChangedArea(const C2DBoxI& area) -- queue a changed world area (push_back onto ChangedAreas)
// Leaf: ?AddChangedArea@CEngineManager@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640560, 69 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?AddChangedArea@CEngineManager@@QAEXABVC2DBoxI@@@Z`; the one caller is CWorldMap::HeightAreaChanged
// 0x004FEB70 (`CTBaseSingleton<CEngineManager>::Get()` 0x00436FB0 then this). Catalog label "std_vector_push_back" is
// the inlined body.
// Layout: Ego_r.pdb CEngineManager { vptr; list<CIEngine*> Engines; vector<C2DBoxI> ChangedAreas }; the retail STLport
// list is one 4-byte node pointer (the landed AddMap 0x006403A0 model: sentinel at +4), so ChangedAreas sits at +8 (PDB
// +0x10). No engine header exists; modelled locally.
// Callees: the vector's STLport _M_insert_overflow(pos, x, const __true_type&, size_type, bool) @ 0x00640670 (`ret 14h`).
// Shape: STLport push_back inline: placement-new null test with C2DBoxI's implicit copy (four dwords through esi), else
// _M_insert_overflow with the empty __true_type tag homed in the dead `area` argument slot (needs the real
// _M_insert_overflow body out of line in the TU, as for vector<CThingMap*> at 0x00639170).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class C2DBoxI { public:
    long TLX;   // +0x00
    long TLY;   // +0x04
    long BRX;   // +0x08
    long BRY;   // +0x0C
};

namespace std {

struct __true_type { __true_type() {} };

template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08

    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x00640670

    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

// The real STLport _M_insert_overflow body (0x00640670 for T = C2DBoxI), out of line in the TU.
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int n, bool atend)
{
    unsigned int oldSize = _M_finish - _M_start;
    unsigned int len = oldSize + (oldSize < n ? n : oldSize);
    T* ns = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* nf = ns;
    T* p;
    for (p = _M_start; p != pos; ++p) { *nf++ = *p; }
    for (; n != 0; --n) { *nf++ = x; }
    if (!atend)
        for (p = pos; p != _M_finish; ++p) { *nf++ = *p; }
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage = ns + len;
}
#pragma auto_inline(on)

}

class CEngineManager
{
public:
    virtual ~CEngineManager();                           // +0x00 vptr
    void*                   Engines;                     // +0x04 (STLport list<CIEngine*>: node pointer)
    std::vector<C2DBoxI>    ChangedAreas;                // +0x08
    void AddChangedArea(const C2DBoxI& area);
};

void CEngineManager::AddChangedArea(const C2DBoxI& area)
{
    ChangedAreas.push_back(area);
}