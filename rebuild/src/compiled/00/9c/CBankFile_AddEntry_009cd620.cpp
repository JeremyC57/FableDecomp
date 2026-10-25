// CBankFile::AddEntry(const CCharString& symbol, unsigned long type, const CCharString& filename, CCountedPointer<CBankStateBlock> state) -- add a single-file bank entry: wrap the filename in a one-element CArray<CCharString> and forward to the virtual AddEntryMultipleFiles
// Leaf: ?AddEntry@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CD620, 239 bytes, thiscall `ret 14h` (two references, the type word and the
// by-value 8-byte counted pointer). FableWin/Ego donor (0x00B1BF50):
// ?AddEntry@CBankFile@@QAEXABVCCharString@@K0V?$CCountedPointer@VCBankStateBlock@@@@@Z; the virtual callee (vtable slot 6,
// `call [edx+18h]`) is ?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z.
// Catalogued as "Map_InsertOrReplacePair" (propagated). Callers: CreateWorldLevelWad 0x00A3A410,
// CEngineSurfaceCompositionManager::AddCustomMorphTexture / UpdateDiskCacheTexture, CEngine::GetEditorAnimation,
// CEngineWorldMap::CreateNewBankMap.
// Callees: MSVCR71 malloc 0x00BFEA0E (allocate(1): no n != 0 test), CCharString::CCharString() 0x0099E4B0,
// CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::~CCharString 0x0099EAE0, CCharString::operator=
// 0x0099EFB0, free 0x00BFEA14, operator delete 0x00BFE9BC (the count block).
// Shape: STLport 4.6 `vector(size_type n)` inlined with the constant 1 (the three pointers zeroed, then start/finish/end
// stored; `uninitialized_fill_n(start, 1, _Tp())` unrolled to one placement copy with its null test; the `_Tp()`
// temporary dies straight after); the by-value counted pointer is copied into the outgoing slot (`sub esp,8 ; mov eax,esp`,
// add-ref when the info block is non-null) and the callee-owned parameter is released at the end (`dec [ebx]` -> the
// block's destroy function on its object, then operator delete).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
void __cdecl operator delete(void* p);                 // 0x00BFE9BC

inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCharString
{
public:
    void* PData;                                       // +0x00 shared string data
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

struct CCPPointerInfo                                  // the shared count block
{
    long m_references;                                 // +0
    void (__fastcall* m_pfnDestroy)(void*);            // +4
    void* m_pObject;                                   // +8
};

template <class T>
struct CCountedPointer
{
    T* Data;                                           // +0
    CCPPointerInfo* Info;                              // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
    }
};

namespace std
{
    struct __true_type { __true_type() { } };
    struct __false_type { __false_type() { } };

    template <class _Tp>
    class allocator
    {
    public:
        allocator() { }
        _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
        void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
    };

    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc
    {
    public:
        _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
        _Value _M_data;
    };

    template <class _T1, class _T2>
    inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

    template <class _Tp>
    inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

    template <class _ForwardIter, class _Size, class _Tp>
    inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
    {
        _ForwardIter __cur = __first;
        for (; __n > 0; --__n, ++__cur)
            _Construct(&*__cur, __x);
        return __cur;
    }

    template <class _ForwardIter, class _Size, class _Tp>
    inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
    {
        return __uninitialized_fill_n(__first, __n, __x, __false_type());
    }

    template <class _ForwardIterator>
    inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
        for (; __first != __last; ++__first)
            _Destroy(&*__first);
    }

    template <class _Tp, class _Alloc>
    class _Vector_base
    {
    public:
        _Vector_base(unsigned int __n, const _Alloc& __a)
            : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
        {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }
        ~_Vector_base()
        {
            if (_M_start != 0)
                _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
        }
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : public _Vector_base<_Tp, _Alloc>
    {
    public:
        explicit vector(unsigned int __n)
            : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
        {
            this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
        }
        ~vector() { _Destroy(this->_M_start, this->_M_finish); }
        _Tp& operator[](unsigned int __n) { return *(this->_M_start + __n); }
    };
}

template <class T>
class CArray : public std::vector<T>                  // retail 12 bytes
{
public:
    explicit CArray(unsigned long n) : std::vector<T>(n) { }
};

class CBankStateBlock;

class CBankFile
{
public:
    virtual ~CBankFile();
    virtual void VirtualSlot1();
    virtual void VirtualSlot2();
    virtual void VirtualSlot3();
    virtual void VirtualSlot4();
    virtual void VirtualSlot5();
    virtual void AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames,
                                       CCountedPointer<CBankStateBlock> state);   // slot 6 (+0x18)
    void AddEntry(const CCharString& symbol, unsigned long type, const CCharString& filename,
                  CCountedPointer<CBankStateBlock> state);
};

void CBankFile::AddEntry(const CCharString& symbol, unsigned long type, const CCharString& filename,
                         CCountedPointer<CBankStateBlock> state)
{
    CArray<CCharString> filenames(1);
    filenames[0] = filename;
    AddEntryMultipleFiles(symbol, type, filenames, state);
}