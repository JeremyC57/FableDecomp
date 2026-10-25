// CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag) -- (T = CEngineWorldMap) report load progress ("Engine: Add Engine Component"), publish the component through its global pointer, tell it where that pointer lives, append it to the engine's component list and let it set up its console enable variable
// Leaf: ??$AddEngineComponent@VCEngineWorldMap@@@CEngine@@AAEXPAVCEngineWorldMap@@AAPAV1@E@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2A480, 148 bytes (+ int3 padding), thiscall, `ret 0Ch`.
// Identity: the single caller is the CEngine constructor (call at 0x00B268D6), which passes `new` 0x1C0 bytes built by
//   0x00B423F0 -- that constructor stores vtable 0x012A1F50, RTTI `.?AVCEngineWorldMap@@` -- and the global pointer 0x01436E8C; the
//   Ego_r name set holds exactly this instantiation (private: component, reference to the global pointer, an unsigned
//   char flag this body never reads).
//   Label correction: the manifest calls this AddEngineComponent<CEngineShadowRenderer> (propagated).
//   Family F009 (20 byte-identical bodies, one per engine component the CEngine constructor adds).
// Callees: CCharString(const char*, long = -1) 0x0099EBF0, NProgressDisplay::DisplayProgress 0x009E9F40 (`__fastcall`:
//   text in ecx, the first bool in dl, the float and the second bool on the stack), ~CCharString 0x0099EAE0,
//   CEngineComponent::SetGlobalComponentPointer(void**) 0x00B59750, STLport vector<CEngineComponent*>::_M_insert_overflow
//   0x00B292B0 (push_back growth), CEngineComponent::SetupEnableConsoleVariable() 0x00B59760 (both Ego_r names, the
//   only two non-virtual CEngineComponent members, in that order).
// Layout: CEngine::EngineComponents (CArray<CEngineComponent*>, a 12-byte STLport vector) at +0x168 in retail
//   (engine/CEngine.h puts it at +0x178: the Ego_r CArrays are 16 bytes), so `this` is a local model; CEngineComponent
//   {vptr, GlobalPointerAddress +4, RenderEnabled +8, KeepAlive +9} as in Ego_r.
// Shape: the progress text is a named CCharString in its own block (built before the arguments are pushed); push_back is
//   STLport's inline form (placement-new null test) with the T* -> CEngineComponent* temporary homed in the dead
//   `component` slot and the __true_type tag in the dead `flag` slot.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E (in the same-TU overflow body)
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    // Same-TU body: retail's 0x00B292B0 (vector<CEngineComponent*>, family F007) is a COMDAT of this TU; VC7.1 keeps it
    // out of line, as retail does.
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);

        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};

} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CEngineComponent {
public:
    virtual ~CEngineComponent();
    void** GlobalPointerAddress;                            // +0x04
    bool RenderEnabled;                                     // +0x08
    unsigned char KeepAlive;                                // +0x09
    void SetGlobalComponentPointer(void** address);         // 0x00B59750
    void SetupEnableConsoleVariable();                      // 0x00B59760
};

class CEngineWorldMap : public CEngineComponent { };   // CEngineComponent subobject at +0 (no `this` adjustment in the body)

class CEngine {
public:
    unsigned char _pad000[0x168];
    CArray<CEngineComponent*> EngineComponents;             // +0x168 (retail; Ego_r +0x178 with 16-byte CArrays)
private:
    template <class T> void AddEngineComponent(T* component, T*& globalPointer, unsigned char flag);
};

template <class T> void CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag)
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

template void CEngine::AddEngineComponent<CEngineWorldMap >(CEngineWorldMap*, CEngineWorldMap*&, unsigned char);