// CSimpleMemoryPool::`scalar deleting destructor'(unsigned int flags) -- the inline destructor (this class's vtable, its cleanup call, the base destructor), then free this when bit 0 of flags is set
// Leaf: ??_GCSimpleMemoryPool@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00987F30, 43 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x0129A064 (RTTI .?AVCSimpleMemoryPool@@, bases CBaseClassNonCopyable : CBaseClass); the vtable's slot 0.
// Callees: CSimpleMemoryPool::ClearMemoryBlocks() 0x00A5C600, CBaseClassNonCopyable::~CBaseClassNonCopyable() 0x0099A300, operator delete 0x00BFE9BC.
// Shape (family K048, representative 0x00987F30): the compiler's scalar deleting destructor around a one-call inline
//   destructor; a `new` of the class in this file makes VC7.1 emit the vtable and the ??_G COMDAT.
class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CSimpleMemoryPool : public CBaseClassNonCopyable {   // RTTI: CBaseClassNonCopyable : CBaseClass
public:
    CSimpleMemoryPool() { }
    virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
private:
    void ClearMemoryBlocks();                            // 0x00A5C600 (Ego_r.pdb private)
};
CSimpleMemoryPool* MakePool_00987f30() { return new CSimpleMemoryPool; }