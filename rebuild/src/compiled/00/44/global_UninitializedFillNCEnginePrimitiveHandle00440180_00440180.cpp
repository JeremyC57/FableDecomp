// _global::UninitializedFillN_CEnginePrimitiveHandle_00440180(CEnginePrimitiveHandle* first, unsigned int n, const CEnginePrimitiveHandle& x) -- STLport uninitialized_fill_n<CEnginePrimitiveHandle*> (the three-argument front end with the __false_type helper inlined): copy-construct n primitive handles, return first + n
// Leaf: UninitializedFillN_CEnginePrimitiveHandle_00440180
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00440180, length 49 bytes, `ret 4`.
// Callers: one site, 0x004423F3 in the vector<CEnginePrimitiveHandle> insert helper 0x00442350 (`push &x ; mov edx,n ;
//   mov ecx,first ; call` -- one stack argument, no tag).
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<..>>*, ...>` is propagated. The body is
//   byte-identical to the landed `__uninitialized_fill_n<CEnginePrimitiveHandle*>` 0x0043F6C0 except for `ret 4`: this is the
//   top-level `uninitialized_fill_n(first, n, x)` whose `__uninitialized_fill_n(..., _IS_POD_ITER(..))` call was inlined, so
//   only &x is on the stack. Model of the class as the landed 0x0043F6C0 / 0x004400C0: CEnginePrimitiveHandle (8 bytes,
//   vtable 0x0122F598 : CBaseClass) = vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +0x04.
// Shape: `__fastcall` (first in ecx -> eax as `cur`, n in edx -> esi counted down, &x on the stack -> ecx). The inline
//   copy constructor stores the vtable and copies the counted pointer (`++P->RefCount` at +4 when non-null); placement-new
//   null test; /O2 loop-head pad. The inner helper needs `__forceinline` (a plain inline cdecl helper stays a call).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    long RefCount;                                       // +0x04
};
class CIVCountedPointer {                                // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* P;
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
class CEnginePrimitiveHandle : public CBaseClass {       // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                              // +0x04
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x) : CBaseClass(x), Data(x.Data) {}
    virtual void v001();
};

struct __false_type { __false_type() { } };     // STLport _IS_POD_ITER result for a class element

__forceinline CEnginePrimitiveHandle* __uninitialized_fill_n(CEnginePrimitiveHandle* first, unsigned int n, const CEnginePrimitiveHandle& x, const __false_type&)
{
    CEnginePrimitiveHandle* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CEnginePrimitiveHandle(x);
    return cur;
}

CEnginePrimitiveHandle* __fastcall UninitializedFillN_CEnginePrimitiveHandle_00440180(CEnginePrimitiveHandle* first, unsigned int n, const CEnginePrimitiveHandle& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}