// _STL::__copy(CScriptThing* first, CScriptThing* last, CScriptThing* result, const random_access_iterator_tag&, int*) -- assign the script things of [first, last) forward over result (counted-pointer share), return the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC2A0, length 132 bytes (+ int3 padding), `ret 0Ch`.
// Identity: callers 0x008AC87C and 0x008AC9E9 (the vector<CScriptThing> insert/erase/assign paths of the script-interface
//   TU) and 0x00CD316D. The catalog label `_Move<CScriptThing*,CScriptThing*>` is propagated (Dinkumware spelling).
// Callees: operator delete @ 0x00BFE9BC; the pointee's delete function through CCPPointerInfo::DeleteFunc.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` in a /Gr TU
//   (`__fastcall`: first in ecx, last in edx, result + the tag reference + the null distance pointer on the stack):
//   n = (last - first) / 12 signed, counted down in the `push ecx` frame word; result + n is stored back into result's
//   slot up front and returned; CScriptThing's implicit operator= is CCountedPointer::operator= -> ShareData inlined
//   (compare Info, release the old block, copy Data/Info, add a reference). Ego_r.pdb CScriptThing { vptr;
//   CCountedPointer<CScriptThing> PImp } with a local model (the generated engine/CScriptThing.h is flattened data only).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

struct CCPPointerInfo {                                 // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04
    void* Data;                                         // +0x08
};

template <class T>
class CCountedPointer {                                 // Ego_r.pdb, 8 bytes
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};

class CBaseClass {                                      // the polymorphic root (vptr at +0)
public:
    virtual ~CBaseClass();                              // 0x0099A2E0
};

class CScriptThing : public CBaseClass {                // Ego_r.pdb, 12 bytes; vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;                 // +0x04
};

namespace _STL {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CScriptThing* __fastcall __copy<CScriptThing*, CScriptThing*, int>(CScriptThing*, CScriptThing*, CScriptThing*,
                                                                            const random_access_iterator_tag&, int*);
}