// CTCShapeManager::~CTCShapeManager() -- unregister from the global shape-manager vector (remove + erase), reset the selection, destroy the shape list
// Leaf: ??1CTCShapeManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00762110, 174 bytes (+int3 pad to 176), plain `ret`.
// Identity: stores CTCShapeManager's vtable 0x0126A2A4; only caller is its scalar deleting destructor 0x00765810
//   (vtable slot 0). Ego_r.pdb ??1CTCShapeManager@@UAE@XZ. The catalog's "CEngineSubPrimitive2DClockSprite_Destructor"
//   is an autoname.
// Callees: STLport __find<CTCShapeManager**, CTCShapeManager*>(first, last, val, const random_access_iterator_tag&)
//   @ 0x007640F0 (/Gr), memmove @ 0x00BFEAE6, __destroy_aux<CCountedPointer<NShape::CShape>*>(first, last,
//   const __false_type&) @ 0x00764C40 (/Gr), free @ 0x00BFEA14, CTCBase::~CTCBase @ 0x00686830.
// Shape: `g_ShapeManagers.erase(remove(begin(), end(), this))` with STLport remove/remove_copy inline (the remove
//   value is `this` kept in esi; the global finish is re-read after every element store, the copy loop runs against
//   the saved last in ebx) and the single-element erase inline (`__copy_trivial` memmove when position + 1 != end,
//   then --finish); `SelectedShape = PersistantShapes.end()`; the member vector's destructor (`_Destroy` through the
//   out-of-line __destroy_aux, then `if (start) free(start)`) and the base destructor. Both empty tag temporaries share
//   the frame byte at [esp+0Fh] next to the remove value's slot; the two tag-taking COMDATs have same-TU
//   auto_inline(off) bodies that never touch the tag.
// Layout: as the constructor 0x00762090 (PersistantShapes +0x0C, SelectedShape +0x18).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

class CThing;
class CCPPointerInfo;

class CTCBase {
public:
    CThing* ParentThing;                                     // +0x04
    bool ToBeRemoved;                                        // +0x08
    bool IsInGlobalUpdateTCs;                                // +0x09
    CTCBase(CThing& thing);                                  // 0x00686800
    virtual ~CTCBase();                                      // 0x00686830
};

namespace NShape {
class CShape;
}

template <class T> class CCountedPointer {
public:
    T* Data;                                                 // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    ~CCountedPointer();                                      // inlined into 0x00764C40
};

class C2DBoxF { public:
    float MinX, MinY, MaxX, MaxY;
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _Tp> inline random_access_iterator_tag iterator_category(const _Tp*)
{
    return random_access_iterator_tag();
}

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
          + ((const char*)__last - (const char*)__first);
}

// 0x007640F0 / 0x00764C40 are retail's out-of-line COMDATs; these same-TU bodies never touch the tag.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last,
                                                         const _Tp& __val, const random_access_iterator_tag&)
{
    for ( ; __first != __last; ++__first)
        if (*__first == __val)
            break;
    return __first;
}

template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        __first->~CCountedPointer();
}
#pragma auto_inline(on)

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, iterator_category(__first));
}

template <class _InputIter, class _OutputIter, class _Tp>
inline _OutputIter remove_copy(_InputIter __first, _InputIter __last, _OutputIter __result, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        if (!(*__first == __val)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}

template <class _ForwardIter, class _Tp>
inline _ForwardIter remove(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    __first = find(__first, __last, __val);
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return remove_copy(++__next, __last, __first, __val);
    }
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* __position)
    {
        if (__position + 1 != end())
            __copy_trivial(__position + 1, _M_finish, __position);
        --_M_finish;
        return __position;
    }
};

}

class CTCShapeManager : public CTCBase {
public:
    std::vector<CCountedPointer<NShape::CShape> > PersistantShapes;  // +0x0C
    CCountedPointer<NShape::CShape>* SelectedShape;                   // +0x18
    bool Activated;                                                   // +0x1C
    bool NewShapeActivated;                                           // +0x1D
    bool LeavingShape;                                                // +0x1E
    C2DBoxF ClosedShapeBounds;                                        // +0x20
    CTCShapeManager(CThing& thing);                                   // 0x00762090
    virtual ~CTCShapeManager();
};

extern std::vector<CTCShapeManager*> g_ShapeManagers;               // 0x013BAD80

CTCShapeManager::~CTCShapeManager()
{
    g_ShapeManagers.erase(std::remove(g_ShapeManagers.begin(), g_ShapeManagers.end(), this));
    SelectedShape = PersistantShapes.end();
}