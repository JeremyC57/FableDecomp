// std::__copy(CLipSyncPhonemeSample* first, CLipSyncPhonemeSample* last, CLipSyncPhonemeSample* result, const random_access_iterator_tag&, int*) -- STLport random-access copy: last - first element assignments `*result = *first` (string, id, counted frame pointer), return the advanced result
// Flags: /Gr
// Leaf: ??$__copy@PAVCLipSyncPhonemeSample@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A49E00, 146 bytes, /Gr `__fastcall` (first ecx, last edx; result, the tag and the
//   distance pointer on the stack, `ret 0Ch`).
// Identity (probable): the 16-byte element is CLipSyncPhonemeSample (Ego_r.pdb { CCharString Symbol; unsigned long ID;
//   CCountedPointer<CSpeechFrame> SampleFrame; }, the element of the proven vector<CLipSyncPhonemeSample>::_M_insert_overflow
//   0x00A4AC60 two functions on); its callers 0x00A495B4 / 0x00A4A17C / 0x00A4A958 / 0x00A4ABE4 are that vector's own
//   insert/erase/assign paths in the same address run, and the next function 0x00A49EA0 is the element's destructor.
// Callees: CCharString::operator= 0x0099EFB0, the counted pointer's DeleteFunc (+4), operator delete 0x00BFE9BC.
// Shape: STLport 4.6 _algobase.h random-access __copy (`for (_Distance __n = __last - __first; __n > 0; --__n)`) with the
//   element's implicit operator= inlined: the string assigned out of line, the id copied, then CCountedPointer's
//   operator= (nothing at all when the info blocks match; else release the old block -- count re-read after the decrement
//   -- copy both words and add a reference). The source's info and data are read into locals before the compare (retail
//   keeps them in ebx and a frame slot across the release; reading the members after it re-loads them, DIFFER 138v146).
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo { public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info && --Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CCharString {
public:
    void* m_pData;                                     // +0x00
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class CSpeechFrame;

class CLipSyncPhonemeSample {                          // Ego_r.pdb, 16 bytes
public:
    CCharString                   Symbol;              // +0x00
    unsigned long                 ID;                  // +0x04
    CCountedPointer<CSpeechFrame> SampleFrame;         // +0x08
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CLipSyncPhonemeSample* __copy(CLipSyncPhonemeSample*, CLipSyncPhonemeSample*, CLipSyncPhonemeSample*,
                                       const random_access_iterator_tag&, int*);
}