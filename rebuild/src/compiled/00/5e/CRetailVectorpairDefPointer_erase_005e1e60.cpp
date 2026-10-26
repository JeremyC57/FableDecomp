// CRetailVector_pairDefPointer::erase(iterator) -- SGI vector erase of one 8-byte {key, CDefPointer} element: copy the tail down, pop the last element, release its def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1E60, length 80 bytes.
// Callees: SGI `__copy(first, last, result, const random_access_iterator_tag&, long*)` @ 0x005E0D80 (/Gr `__fastcall`: first in
// ecx, last in edx, result on the stack, the empty tag temporary homed in the dead argument slot of `pos`, a null distance
// pointer); CDefPointeeBase vslot 1 Destroy through the inlined release of the popped element.
// Shape: `if (pos + 1 != finish) copy(...)`, `--finish`, then the element destructor (null test, `dec [ecx+4] ; jne`, the
// virtual destroy, the clear) on the new finish; `pos` is returned. The manifest label "CDefClassBase::UnloadResource" is
// propagated; the vector is named by its element shape (0x00720810 is the 4-byte-element twin).
struct random_access_iterator_tag { random_access_iterator_tag() { } };

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    unsigned long RefCount;                        // +4
    void Release() { if (--RefCount == 0) Destroy(); }
};

struct CDefPointer {                               // CDefPointer<T>
    CDefPointeeBase* Object;                       // +0
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

struct SPairDefPointer {                           // 8-byte element: a key word and the counted def pointer
    long        first;                             // +0
    CDefPointer second;                            // +4
};

SPairDefPointer* __fastcall copy_sgi(SPairDefPointer* first, SPairDefPointer* last, SPairDefPointer* result, const random_access_iterator_tag& tag, long* distance);   // 0x005E0D80

struct CRetailVector_pairDefPointer {              // SGI vector: start / finish / end_of_storage
    SPairDefPointer* start;
    SPairDefPointer* finish;
    SPairDefPointer* end_of_storage;
    SPairDefPointer* erase(SPairDefPointer* pos);
};

SPairDefPointer* CRetailVector_pairDefPointer::erase(SPairDefPointer* pos)
{
    if (pos + 1 != finish)
        copy_sgi(pos + 1, finish, pos, random_access_iterator_tag(), (long*)0);
    --finish;
    finish->~SPairDefPointer();
    return pos;
}