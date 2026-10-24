// CTCShapeManager::CTCShapeManager(CThing& thing) -- the shape-manager component: empty shape list, flags cleared, registered in the global shape-manager vector, selection at end()
// Leaf: ??0CTCShapeManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00762090, 124 bytes (+int3 pad to 128), `ret 4`, returns this.
// Identity: the vtable it stores, 0x0126A2A4, is .?AVCTCShapeManager@@ (RTTI locator; rtti-vtables.tsv); Ego_r.pdb
//   ??0CTCShapeManager@@QAE@AAVCThing@@@Z. The catalog's "CEngineSubPrimitive2DClockSprite_Vector_PushBack" is an
//   autoname. Only caller 0x004D3C43 (CTCShapeManager::Alloc).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800; std::vector<CTCShapeManager*>::_M_insert_overflow(pos, x,
//   const __true_type&, fill_len, atend) @ 0x00765210 on the global vector @ 0x013BAD80 (its atexit cleanup is
//   0x01229AB0).
// Shape: one zero register (ecx) for the PersistantShapes vector words and the three flag bytes (initialiser list);
//   the body's inline STLport push_back(this): `this` is an rvalue, so its const-ref temporary is stored into the dead
//   `thing` argument slot (`mov [esp+8],esi`) and the growth path's empty __true_type tag is homed on that same slot;
//   the placement-new null test reuses the zero register and finish is re-read after the store. The overflow callee
//   has a same-TU auto_inline(off) body that reads x only through its reference and never touches the tag (GOTCHAS
//   retry 10e/10f), which is what lets VC7.1 put both temporaries on the argument slot. SelectedShape =
//   PersistantShapes.end() is the last body statement.
// Layout: Ego_r.pdb CTCShapeManager with retail 12-byte STLport vectors: CTCBase (vptr, ParentThing +4, ToBeRemoved +8,
//   IsInGlobalUpdateTCs +9), PersistantShapes +0x0C, SelectedShape +0x18, Activated +0x1C, NewShapeActivated +0x1D,
//   LeavingShape +0x1E, ClosedShapeBounds +0x20 (C2DBoxF, no constructor code). Local CTCBase model: engine/CTCBase.h
//   carries the vptr as a data member and no constructor.
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;
struct CCPPointerInfo;

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
};

struct C2DBoxF {
    float MinX, MinY, MaxX, MaxY;
};

namespace _STL {

struct __true_type { __true_type() { } };

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
          + ((const char*)__last - (const char*)__first);
}

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend);
    void push_back(const T& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(__x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

// 0x00765210 is retail's out-of-line COMDAT; this same-TU body (STLport's __true_type growth path) reads x only through
// its reference and never touches the tag.
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* __position, const T& __x, const __true_type&,
                                                        unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    T* __new_start = __len != 0 ? (T*)malloc(__len * sizeof(T)) : 0;
    T* __new_finish = (T*)__copy_trivial(_M_start, __position, __new_start);
    for (unsigned int __n = __fill_len; __n > 0; --__n, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        __new_finish = (T*)__copy_trivial(__position, _M_finish, __new_finish);
    if (_M_start)
        free(_M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)

}

class CTCShapeManager : public CTCBase {
public:
    _STL::vector<CCountedPointer<NShape::CShape> > PersistantShapes;  // +0x0C
    CCountedPointer<NShape::CShape>* SelectedShape;                   // +0x18
    bool Activated;                                                   // +0x1C
    bool NewShapeActivated;                                           // +0x1D
    bool LeavingShape;                                                // +0x1E
    C2DBoxF ClosedShapeBounds;                                        // +0x20
    CTCShapeManager(CThing& thing);
    virtual ~CTCShapeManager();                                       // 0x00762110
};

extern _STL::vector<CTCShapeManager*> g_ShapeManagers;               // 0x013BAD80

CTCShapeManager::CTCShapeManager(CThing& thing)
    : CTCBase(thing), Activated(false), NewShapeActivated(false), LeavingShape(false)
{
    g_ShapeManagers.push_back(this);
    SelectedShape = PersistantShapes.end();
}