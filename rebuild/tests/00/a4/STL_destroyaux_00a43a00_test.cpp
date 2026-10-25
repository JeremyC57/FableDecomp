// Behaviour test for std::__destroy_aux(NGameText::CDataBankTextEntry*, ..., const __false_type&) (0x00A43A00): each
// entry's counted text pointer is released (count decremented, Delete() at zero) and nulled; null pointers are skipped.
#include <cstdio>
#include <new>

static int gDeleted = 0;

class CIVCountedPointeeBase {
public:
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() { ++gDeleted; }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        T* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { };
class CDataBankTextEntry {
public:
    CIVCountedPointer<CTextEntry> PText;
    long                          NarratorIndex;
};
}

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
}

int main()
{
    NGameText::CTextEntry shared, single;
    shared.IVRefCount = 2;
    single.IVRefCount = 1;
    static double storage[16];
    NGameText::CDataBankTextEntry* e = (NGameText::CDataBankTextEntry*)storage;
    for (int i = 0; i < 4; ++i)
        new (&e[i]) NGameText::CDataBankTextEntry();
    e[0].PText.Object = &shared;
    e[1].PText.Object = 0;
    e[2].PText.Object = &shared;
    e[3].PText.Object = &single;
    std::__destroy_aux(e, e + 4, std::__false_type());
    int fails = 0;
    if (shared.IVRefCount != 0 || single.IVRefCount != 0) ++fails;
    if (gDeleted != 2) ++fails;
    for (int i = 0; i < 4; ++i)
        if (e[i].PText.Object != 0) ++fails;
    if (fails) { std::printf("00a43a00_TEST FAIL\n"); return 1; }
    std::printf("00a43a00_TEST PASS\n");
    return 0;
}