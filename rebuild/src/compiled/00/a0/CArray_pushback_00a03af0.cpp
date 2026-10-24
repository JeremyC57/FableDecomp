// CArray::push_back() -- append one default (null) handle slot to the CArray<CHandleBasedMemoryPool::CHandleInfo>: a placement copy at the finish when there is room, else the reallocating insert for one element
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03AF0, length 45 bytes (`ret`, + int3 padding).
// Identity: Ego_r.pdb name set `?push_back@?$CArray@VCHandleInfo@CHandleBasedMemoryPool@@@@QAEXXZ` -- this source emits exactly
// that symbol (the catalog's "Vector_PushBack_Lazy" is a propagated label). CHandleInfo is 4 bytes (one union: the area
// pointer, or the next unused handle index) with a default constructor (`??0CHandleInfo@CHandleBasedMemoryPool@@QAE@XZ`).
// The landed CHandleBasedMemoryPool::InternalAllocateHandle 0x00A03040 inlines the one-argument push_back of the same array.
// Callees: the retail vector's reallocating insert @ 0x00A03900 (pos, const T& x, const SScalarTag& tag, fill count, at end).
// Shape: push_back() is `push_back(T())` over the inline one-argument push_back: the default-constructed temporary is the
// zero word at [esp+4] stored before the capacity test; the room path is the null-checked placement copy (folded to the
// constant 0); the full path pushes 1, 1, the 1-byte empty tag temporary (byte 3 of the 8-byte frame) and the value's
// address, with `this` still in ecx.
// Leaf: ?push_back@?$CArray@VCHandleInfo@CHandleBasedMemoryPool@@@@QAEXXZ
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

class CHandleBasedMemoryPool {
public:
    class CHandleInfo {
    public:
        union {
            void* PArea;
            long  NextUnusedHandle;
        };
        CHandleInfo() : PArea(0) { }
    };
};

template <class T> class CArray {         // TLC CArray<T>: the STLport vector words {start, finish, end of storage}, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* pos, const T& x, const SScalarTag& tag, unsigned long fillLen, bool atEnd);   // 0x00A03900
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, SScalarTag(), 1, true);
    }
    void push_back() { push_back(T()); }
};

template void CArray<CHandleBasedMemoryPool::CHandleInfo>::push_back();