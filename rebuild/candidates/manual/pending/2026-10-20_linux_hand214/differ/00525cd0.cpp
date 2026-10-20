// CThingManager::OnLevelsUnload(const std::vector<long>& levels, bool flag) -- collect the things of every unloading level, let each prepare for the unload, kill them all (not immediately), then process the deaths
// Grade: DIFFER(192v179) -- register allocation: retail spills `this` to the frame, keeps the vector's begin/end in ebx/edi across both walks (known zero on the skipped loop) and homes the binder's bool in the dead _M_finish slot; ours keeps `this` in ebx, reloads the vector words per walk and opens a separate binder slot (0x18 frame)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00525CD0, length 177 bytes, `ret 8`.
// Callees: CThingManager::SaveThingsOnLevelReadyForUnloading(long, bool, vector<CThing*>*) @ 0x005252B0 (engine_api.tsv),
//   a CThing member @ 0x005271B0 run on every collected thing (the manifest's "CThingFilter_IsValid::operator()" is a
//   byte-twin label; name unproven), CThing::Kill(bool) @ 0x004C9B80, CThingManager::UpdateDeaths @ 0x0051F070,
//   free @ 0x00BFEA14.
// Shape: the collected things are a local STLport vector<CThing*> (three zero words, storage freed at the end); the level
//   loop re-reads `levels.size()` each pass (`sar` of the byte difference, unsigned `jb`); the two walks are inline
//   for_each over mem_fun / bind2nd(mem_fun, false): the binder's bound bool lives in the frame and the member pointer
//   is held in ebp for the indirect call.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

class CThing {
public:
    bool PrepareForLevelUnload_005271b0();      // 0x005271B0
    void Kill(bool immediately);                // 0x004C9B80
};

struct CLongVector {                            // retail std::vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const long& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CThingVector {                           // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
    CThing** begin() { return _M_start; }
    CThing** end() { return _M_finish; }
};

template<class R, class T> struct mem_fun_t {
    R (T::*m_f)();
    explicit mem_fun_t(R (T::*f)()) : m_f(f) { }
    R operator()(T* p) const { return (p->*m_f)(); }
};
template<class R, class T, class A> struct mem_fun1_t {
    R (T::*m_f)(A);
    explicit mem_fun1_t(R (T::*f)(A)) : m_f(f) { }
    R operator()(T* p, A a) const { return (p->*m_f)(a); }
};
template<class R, class T> inline mem_fun_t<R, T> mem_fun(R (T::*f)()) { return mem_fun_t<R, T>(f); }
template<class R, class T, class A> inline mem_fun1_t<R, T, A> mem_fun(R (T::*f)(A)) { return mem_fun1_t<R, T, A>(f); }
template<class R, class T, class A> struct binder2nd {
    mem_fun1_t<R, T, A> op;
    A value;
    binder2nd(const mem_fun1_t<R, T, A>& o, A v) : op(o), value(v) { }
    R operator()(T* p) const { return op(p, value); }
};
template<class R, class T, class A> inline binder2nd<R, T, A> bind2nd(const mem_fun1_t<R, T, A>& o, A v) { return binder2nd<R, T, A>(o, v); }
template<class I, class F> inline F for_each(I first, I last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

class CThingManager {
public:
    void SaveThingsOnLevelReadyForUnloading(long level, bool flag, CThingVector* things);   // 0x005252B0
    void UpdateDeaths();                        // 0x0051F070
    void OnLevelsUnload(const CLongVector& levels, bool flag);
};

void CThingManager::OnLevelsUnload(const CLongVector& levels, bool flag)
{
    CThingVector things;
    for (unsigned long i = 0; i < levels.size(); ++i)
        SaveThingsOnLevelReadyForUnloading(levels[i], flag, &things);
    for_each(things.begin(), things.end(), mem_fun(&CThing::PrepareForLevelUnload_005271b0));
    for_each(things.begin(), things.end(), bind2nd(mem_fun(&CThing::Kill), false));
    UpdateDeaths();
}
