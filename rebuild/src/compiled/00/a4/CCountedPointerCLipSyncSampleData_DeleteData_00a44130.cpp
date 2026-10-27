// CCountedPointer<CLipSyncSampleData>::DeleteData(void*) -- counted-pointer deleter: `delete (CLipSyncSampleData*)p`
// Retail: Fable.exe 0x00A44130, 50 bytes, static __fastcall: if p, the inlined ~CLipSyncSampleData ->
// ~CArray<CLipSyncFrameData> -> STLport ~vector: __destroy_aux(_M_start, _M_finish, __false_type) (0x00A43F80, the
// tag homed in a frame byte), `if (_M_start) free(_M_start)` (0x00BFEA14), then operator delete(p) (0x00BFE9BC).
// Identity: the only reference is 0x00A441BD, where CCountedPointer<T>::Reset 0x00A44170 stores this deleter in a new
// pointer-info block; Reset's caller 0x00A42EC0 news the 0x10-byte object (three zeroed vector words: Ego_r.pdb
// CLipSyncSampleData = FrameData CArray<CLipSyncFrameData> + FramesPerSecond float, retail 0xC + 4), and 0x00A43F80
// destroys 12-byte CLipSyncFrameData elements (Phonemes CArray<CLipSyncPhoneme>, each 12 bytes with the CCharString
// Symbol first). Ego_r.pdb: ?DeleteData@?$CCountedPointer@VCLipSyncSampleData@@@@SIXPAX@Z.
// Leaf: ?DeleteData@?$CCountedPointer@VCLipSyncSampleData@@@@SIXPAX@Z
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};

namespace std {
    struct __false_type { __false_type() { } };
    template <class _Tp> class allocator {
    public:
        void deallocate(_Tp* __p, unsigned int) { free(__p); }
    };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };

    template <class _ForwardIterator>
    void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00A43F80
    template <class _ForwardIterator>
    inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
        __destroy_aux(__first, __last, __false_type());
    }

    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        ~_Vector_base()
        {
            if (_M_start != 0)
                _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
        }
        _Tp* _M_start;                                  // +0x00
        _Tp* _M_finish;                                 // +0x04
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : public _Vector_base<_Tp, _Alloc> {
    public:
        ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    };
}

template <class T> class CArray : public std::vector<T> { };

class CLipSyncPhoneme {
public:
    CCharString   Symbol;                               // +0x00
    unsigned long ID;                                   // +0x04
    float         Weight;                               // +0x08
};

class CLipSyncFrameData {
public:
    CArray<CLipSyncPhoneme> Phonemes;                   // +0x00
};

class CLipSyncSampleData {
public:
    CArray<CLipSyncFrameData> FrameData;                // +0x00
    float                     FramesPerSecond;          // +0x0C
};

template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* p) { delete (T*)p; }
};

template class CCountedPointer<CLipSyncSampleData>;