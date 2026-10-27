// CDynamicObjectHeap<56,16>::`scalar deleting destructor'(unsigned int flags) -- the inline destructor (this class's vtable, its cleanup call, the base destructor), then free this when bit 0 of flags is set
// Leaf: ??_G?$CDynamicObjectHeap@$0DI@$0BA@@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B49710, 43 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x012A20FC (RTTI .?AV?$CDynamicObjectHeap@$0DI@$0BA@@@ = <56, 16>); Ego_r.pdb ??_G?$CDynamicObjectHeap@$0DI@$0BA@@@UAEPAXI@Z; the vtable's slot 0.
// Callees: CDynamicObjectHeap<56,16>::Release() 0x00B48E10 (landed), CDynamicObjectHeapBase::~CDynamicObjectHeapBase() 0x00F36C90, operator delete 0x00BFE9BC.
// Shape (family K048, representative 0x00987F30): the compiler's scalar deleting destructor around a one-call inline
//   destructor; a `new` of the class in this file makes VC7.1 emit the vtable and the ??_G COMDAT.
class CDynamicObjectHeapBase {                           // Ego_r.pdb: vptr, AllocCount, TotalAllocatorBlocks, RefCountReleaseEnabled, HeapNext
public:
    CDynamicObjectHeapBase(bool refCountRelease);
    virtual ~CDynamicObjectHeapBase();                   // 0x00F36C90
    long AllocCount;                                     // +0x04
    long TotalAllocatorBlocks;                           // +0x08
    bool RefCountReleaseEnabled;                         // +0x0C
    CDynamicObjectHeapBase* HeapNext;                    // +0x10
};
template <long ObjectSize, long BlockCount> class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    CDynamicObjectHeap(bool refCountRelease) : CDynamicObjectHeapBase(refCountRelease) { }
    virtual ~CDynamicObjectHeap() { Release(); }
private:
    virtual void Release();                              // 0x00B48E10 for <56, 16> (FableWin.pdb private virtual)
    unsigned char _body[0x3a0 - 0x14];                       // FreeList, StartBlock, AllowHeapToGrow (not touched here)
};
// The constructor's vtable store makes VC7.1 emit the vtable and with it the scalar deleting destructor COMDAT.
CDynamicObjectHeap<56, 16>* MakeHeap_00b49710() { return new CDynamicObjectHeap<56, 16>(true); }