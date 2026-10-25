// behaviour test for 0x00B299D0 CEngine::AddEngineComponent<CEngineResourceManager>: progress text shown once per call, the global pointer published and registered with the component, the component appended (with STLport growth) and asked to set up its console variable
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>
#if defined(_MSC_VER)
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
inline void* operator new(size_t, void* p, int) { return p; }
static int gStrings = 0, gProgress = 0, gProgressOk = 0, gSetups = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { ++gStrings; (void)length; }
    ~CCharString() { --gStrings; }
};
namespace NProgressDisplay {
void FASTCALL DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    ++gProgress;
    if (std::strcmp(text.PStringData, "Engine: Add Engine Component") == 0 && progress == -1.0f && !a && !b && gStrings == 1) ++gProgressOk;
}
}
namespace stlp_test {
struct __true_type { __true_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value) { for (; __n > 0; --__n, ++__first) *__first = __value; return __first; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p, 0) _T1(__val); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) std::free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base { public: _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) { _Construct(this->_M_finish, __x); ++this->_M_finish; }
        else _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend) __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}
template <class T> class CArray : public stlp_test::vector<T> {
public:
    CArray() { this->_M_start = 0; this->_M_finish = 0; this->_M_end_of_storage._M_data = 0; }
    ~CArray() { std::free(this->_M_start); }
};
class CEngineComponent {
public:
    virtual ~CEngineComponent() { }
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
    CEngineComponent() : GlobalPointerAddress(0), RenderEnabled(false), KeepAlive(0) { }
    void SetGlobalComponentPointer(void** address) { GlobalPointerAddress = address; }
    void SetupEnableConsoleVariable() { ++gSetups; }
};
class CEngineResourceManager : public CEngineComponent { };
class CEngine {
public:
    CArray<CEngineComponent*> EngineComponents;
    template <class T> void AddEngineComponent(T* component, T*& globalPointer, unsigned char flag)
    {
        {
            CCharString text("Engine: Add Engine Component");
            NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
        }
        globalPointer = component;
        component->SetGlobalComponentPointer((void**)&globalPointer);
        EngineComponents.push_back(component);
        component->SetupEnableConsoleVariable();
    }
};
static CEngineResourceManager* gPointers[3];
static int Check()
{
    CEngine engine;
    CEngineResourceManager parts[3];
    for (int i = 0; i < 3; ++i) {
        engine.AddEngineComponent(&parts[i], gPointers[i], (unsigned char)i);
        if (gPointers[i] != &parts[i]) return 1;
        if (parts[i].GlobalPointerAddress != (void**)&gPointers[i]) return 2;
        if (engine.EngineComponents.size() != (unsigned int)(i + 1)) return 3;
        if (engine.EngineComponents._M_start[i] != &parts[i]) return 4;
    }
    if (engine.EngineComponents._M_start[0] != &parts[0] || engine.EngineComponents._M_start[1] != &parts[1]) return 5;
    if (gProgress != 3 || gProgressOk != 3 || gStrings != 0 || gSetups != 3) return 6;
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("00b299d0_TEST FAIL %d\n", r); return 1; }
    std::printf("00b299d0_TEST PASS\n");
    return 0;
}