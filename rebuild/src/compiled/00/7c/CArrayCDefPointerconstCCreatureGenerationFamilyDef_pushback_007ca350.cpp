// CArray<CDefPointer<const CCreatureGenerationFamilyDef> >::push_back(const CDefPointer<const CCreatureGenerationFamilyDef>& x) -- append a counted family-def pointer (copy-construct in place, adding a reference to a non-null def), or grow through _M_insert_overflow when full
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CA350, 57 bytes, thiscall `ret 4`.
// Identity: the only caller is CTCCreatureGeneratorCreator::FrameUpdate (0x007C99F0, call at 0x007C9BD5), which fills
//   a CDefPointer local through CDefinitionManager::GetDef<CCreatureGenerationFamilyDef> (0x0074D220), tests the def's
//   +0x48 (DifficultyLevel: PDB +0x50, after a 0x3c retail def base and a 12-byte STLport Creatures vector) against
//   the hero level and pushes the pointer. Ego_r.pdb and FableWin.pdb both define
//   ?push_back@?$CArray@V?$CDefPointer@$$CBVCCreatureGenerationFamilyDef@@@@@@QAEXABV...@Z out of line (and no
//   vector<CDefPointer<const CCreatureGenerationFamilyDef> >::push_back), so this is the CArray member with the STLport
//   body inlined into it (GOTCHAS: CArray::push_back carries the body, the 0x006BF410 recipe). Was landed as
//   "std::vector<CDefPointer<const CDefClassBase> >::push_back" (a stand-in pointee); the catalog label
//   "vector_PushBack_CCountedPtr" is descriptive.
// Callees: vector<...>::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool) @ 0x007CA250.
// Layout: CDefPointer<T> holds a CDefPointeeBase* (Ego_r.pdb), reference count at +4; retail CArray is the bare
//   12-byte STLport vector.
// Shape: STLport push_back: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- the placement-new null test, the copy constructor's
//   add-ref after the pointer store, `add [ecx+4],4`. The empty tag is homed on x's argument slot (`lea edx,[esp+0Ch]`
//   pushed before x): VC7.1 does that only when _M_insert_overflow has a same-TU noinline body that reads x and leaves
//   the tag unnamed (the 0x007B4F20 recipe).
// Leaf: ?push_back@?$CArray@V?$CDefPointer@$$CBVCCreatureGenerationFamilyDef@@@@@@QAEXABV?$CDefPointer@$$CBVCCreatureGenerationFamilyDef@@@@@Z
inline void* operator new(unsigned int, void* p) { return p; }

class CDefPointeeBase {                                     // counted def object: vptr, reference count at +4
public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};

class CDefClassBase : public CDefPointeeBase { };
class CCreatureGenerationFamilyDef : public CDefClassBase { };

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class T, class _Alloc = allocator<T> > class vector {
public:
    typedef T value_type;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    __declspec(noinline) void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x007CA250
};

// Same-TU stand-in for the out-of-line growth path (only the target row is graded): it reads the value through
// its reference and never touches the empty tag.
template <class T, class _Alloc>
void vector<T, _Alloc>::_M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    if (__x.Object == 0 && __atend)
        this->_M_finish = __position + __fill_len;
}
}

template <class T> class CArray : public std::vector<T> {
public:
    void push_back(const T& __x);
};

template <class T> void CArray<T>::push_back(const T& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        std::_Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        this->_M_insert_overflow(this->_M_finish, __x, std::__false_type(), 1UL, true);
}

template void CArray<CDefPointer<const CCreatureGenerationFamilyDef> >::push_back(const CDefPointer<const CCreatureGenerationFamilyDef>&);
