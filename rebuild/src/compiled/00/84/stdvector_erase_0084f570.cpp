// std::vector::erase(iterator position) [vector<CCountedPointer<T> >] -- erase one counted pointer: shift the tail down one slot, release and clear the vacated last slot, return the position
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084F570, length 97 bytes (+15 int3 padding).
// NOT CScriptBase::RemovePredicate (propagated label): an STLport vector<CCountedPointer<T> >::erase(iterator) on
// 8-byte elements (callers 0x0084E37B, 0x0084E85F). Callees: the STLport `__copy` for random-access ranges @ 0x0084F330
// (`__fastcall` first/last in ecx/edx; result, `const random_access_iterator_tag&`, `(int*)0` on the stack;
// manifest calls it _Move), operator delete @ 0x00BFE9BC. The counted pointer's count block is {count, __fastcall
// destroy(object), object}. Element type unknown (descriptive name).
// Shape: STLport `if (pos + 1 != end()) __copy_ptrs(pos + 1, _M_finish, pos, ...); --_M_finish; _Destroy(_M_finish);`
// with the counted pointer's inline destructor (null test, `dec [count]`, re-read, destroy + delete, then both words
// zeroed); the empty iterator tag sits in the dead `position` argument slot in retail.
struct CPredicateHolderElement;

struct random_access_iterator_tag {
    random_access_iterator_tag() {}
};

struct SCountBlock {
    long References;                                 // +0
    void (__fastcall* Destroy)(void*);               // +4
    void* Object;                                    // +8
};

template <class T> struct CCountedPointer {
    T* Data;
    SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->References;
            if (Info->References == 0)
            {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

typedef CCountedPointer<CPredicateHolderElement> CElement;

CElement* __fastcall __copy(CElement* first, CElement* last, CElement* result,
                            const random_access_iterator_tag& tag, int* distance);   // 0x0084F330

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* end() { return _M_finish; }
    T* erase(T* position);
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <> CElement* vector<CElement>::erase(CElement* position)
{
    if (position + 1 != end())
        __copy(position + 1, _M_finish, position, random_access_iterator_tag(), (int*)0);
    --_M_finish;
    _Destroy(_M_finish);
    return position;
}