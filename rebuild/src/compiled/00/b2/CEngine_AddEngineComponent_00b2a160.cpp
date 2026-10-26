// CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag) -- (T = CEngineLightingManager) report load progress ("Engine: Add Engine Component"), publish the component through its global pointer, register that pointer with its CEngineComponent base (+0xC), append the base to the engine's component list and set up its console enable variable
// Leaf: ??$AddEngineComponent@VCEngineLightingManager@@@CEngine@@AAEXPAVCEngineLightingManager@@AAPAV1@E@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2A160, 160 bytes, thiscall, `ret 0Ch`.
// Identity (proven): CEngine::AddEngineComponent<CEngineLightingManager>(CEngineLightingManager*, CEngineLightingManager*&, unsigned char); evidence: the single caller 0x00B26360 (call at 0x00B26772) passes `new` 0x46D0 bytes built by the constructor 0x00B482A0, which stores vtables 0x012A223C / 0x012A2274 (RTTI .?AVCEngineLightingManager@@, base array CEngineLightingManager <- CEnginePrimitiveManager <- CEnginePrimitiveRenderUtil <- CBaseClassNonCopyable <- CBaseClass <- CEngineComponent, CEngineComponent at mdisp 0xC = the body's `lea edi, [esi + 0xC]`), and the global pointer 0x01436E9C; the PDB name sets hold exactly this instantiation (Ego_r.pdb sizeof CEngineLightingManager = 0x46DC)
// Label: the manifest's "AddEngineComponent<CEngineLightingManager>" is propagated (family J026: six AddEngineComponent<T> bodies whose T has its
//   CEngineComponent base at a non-zero offset; retail has no identical-code folding, so each address is one T).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z), NProgressDisplay::DisplayProgress
//   0x009E9F40 (?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z, `__fastcall`: text in ecx, the first bool in dl,
//   the float and the second bool on the stack), CCharString::~CCharString 0x0099EAE0, CEngineComponent::SetGlobalComponentPointer
//   0x00B59750 (?SetGlobalComponentPointer@CEngineComponent@@QAEXPAPAX@Z), STLport vector<CEngineComponent*>::_M_insert_overflow
//   0x00B292B0 (same-TU body, kept out of line), CEngineComponent::SetupEnableConsoleVariable 0x00B59760
//   (?SetupEnableConsoleVariable@CEngineComponent@@QAEXXZ).
// Layout: CEngine::EngineComponents (CArray<CEngineComponent*>, a 12-byte STLport vector) at +0x168 in retail (engine/CEngine.h
//   has +0x178 with 16-byte Ego_r CArrays), so `this` is a local model (the landed 0x00B29E30 model); CEngineComponent {vptr,
//   GlobalPointerAddress +4, RenderEnabled +8, KeepAlive +9} as in Ego_r; CEngineLightingManager: primary base CEnginePrimitiveManager (0xC bytes), CEngineComponent at +0xC (RTTI); its own members are not used here.
// Shape: as the landed 0x00B29E30, plus the T* -> CEngineComponent* adjustment: the member calls go through the unchecked
//   base address (`lea edi, [esi + 0xC]`), the push_back argument is the null-checked conversion (`neg ; sbb ; and`)
//   homed in the dead `component` slot.
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

class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
};
class CEngineScene;
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil {   // Ego_r.pdb 0xC bytes = retail (CEngineComponent at +0xC)
public:
    CArray<CEngineScene*>* SceneList;                       // +0x04
    bool                   Enable;                          // +0x08
};
class CEngineLightingManager : public CEnginePrimitiveManager, public CEngineComponent {   // RTTI .?AVCEngineLightingManager@@, CEngineComponent at +0xC
};

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

template void CEngine::AddEngineComponent<CEngineLightingManager >(CEngineLightingManager*, CEngineLightingManager*&, unsigned char);