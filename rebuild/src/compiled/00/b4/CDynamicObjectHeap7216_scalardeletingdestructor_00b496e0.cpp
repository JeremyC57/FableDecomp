// CDynamicObjectHeap<72, 16>::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the inline user destructor (own vtable store, then Release()) and the out-of-line ~CDynamicObjectHeapBase, then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_G?$CDynamicObjectHeap@$0EI@$0BA@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B496E0, 43 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable
//   0x012A20F0 slot 0.
// Identity (proven): CDynamicObjectHeap<72, 16>; evidence: vtable 0x012A20F0 slot 0, RTTI .?AV?$CDynamicObjectHeap@$0EI@$0BA@@@ : CDynamicObjectHeapBase; slot 1 is the Release 0x00B48D40 this destructor calls (the base slot 1 is _purecall); the base destructor 0x00F36C90 stores the CDynamicObjectHeapBase vtable 0x01301CD4; FableWin.pdb names ??_G?$CDynamicObjectHeap@$0EI@$0BA@@@UAEPAXI@Z and ?Release@?$CDynamicObjectHeap@$0EI@$0BA@@@EAEXXZ.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across family H025 (seven bodies with the same shape: vtable store, one call,
//   the base destructor, the flag test; retail has no identical-code folding, so each address is one class).
// Callees: CDynamicObjectHeap<72, 16>::Release() (slot 1, called directly) 0x00B48D40, CDynamicObjectHeapBase::~CDynamicObjectHeapBase 0x00F36C90, operator delete 0x00BFE9BC.
// Shape: VC7.1 resets the vptr in a user-written destructor, so the store is the class's own vtable; the destructor body is
//   one call (`this` is already in ecx), the base destructor follows. The deleting destructor is emitted with the vtable, in
//   a TU that constructs the class: MakeOne is that construction, nothing more.
class CDynamicObjectHeapBase {                          // RTTI base of every CDynamicObjectHeap<N, M>
public:
    CDynamicObjectHeapBase();
    virtual ~CDynamicObjectHeapBase();                  // 0x00F36C90
    virtual void Release() = 0;                         // slot 1 (_purecall in the base vtable 0x01301CD4)
};
template <long ObjectSize, long ObjectsPerBlock> class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    CDynamicObjectHeap(bool enable) { }                 // PDB ??0?$CDynamicObjectHeap@...@@QAE@_N@Z
    virtual ~CDynamicObjectHeap() { Release(); }
private:
    virtual void Release();                             // slot 1: 0x00B48D40 (PDB ?Release@?$CDynamicObjectHeap@...@@EAEXXZ)
};
void* MakeOne_00b496e0()
{
    return new CDynamicObjectHeap<72, 16>(false);
}