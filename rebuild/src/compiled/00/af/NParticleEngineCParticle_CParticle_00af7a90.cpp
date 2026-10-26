// NParticleEngine::CParticle::CParticle() -- default constructor: base CBase(), own vtable, the RandomAcceleration scoped_ptr and the two CArray members zeroed by their inline constructors, then RandomAcceleration.reset() deletes any held object
// Leaf: ??0CParticle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7A90, length 84 bytes.
// Callees: CBase::CBase() @ 0x0099A2F0; the held CRandomAcceleration's virtual deleting destructor
//          (vtable slot 0) on the reset path.
// Vtable 0x0129F4F0 = NParticleEngine::CParticle (rtti-vtables.tsv).
// Layout (PDB CParticle, 0xa4; retail 0x9c because the two CArray members are 12 bytes, not 16):
// Orientation +0x04 .. ClosestAttractorDistance +0x7c (plain data, not touched here),
// RandomAcceleration scoped_ptr +0x80, PrimIDList CArray<CArray<long> > +0x84,
// ParentHandleList CArray<CEnginePrimitiveHandle> +0x90.
// Shape: every zero store follows the vtable store, so the members are objects with inline
// constructors (scoped_ptr() : p(0), CArray() : data/size/capacity 0) sharing the eax zero register;
// the body's reset() re-reads the pointer it has just zeroed (`old = p; p = 0; if (old) delete old;`)
// and deletes through the virtual destructor (`push 1 ; call [eax]`).

struct CBase {
    virtual void v00();
    CBase();                                                              // 0x0099A2F0
};

namespace NParticleEngine {

class CRandomAcceleration { public:
    virtual ~CRandomAcceleration();                                       // slot 0: scalar deleting destructor
};

struct CRandomAccelerationScopedPtr {                                     // scoped_ptr<CRandomAcceleration>
    CRandomAcceleration* m_p;
    CRandomAccelerationScopedPtr() : m_p(0) {}
    void reset(CRandomAcceleration* p = 0)
    {
        CRandomAcceleration* old = m_p;
        m_p = p;
        if (old)
            delete old;
    }
};

struct CLongArrayArray {                                                  // retail CArray<CArray<long> > (12 bytes)
    void* m_pData;
    long  m_size;
    long  m_capacity;
    CLongArrayArray() : m_pData(0), m_size(0), m_capacity(0) {}
};

struct CPrimitiveHandleArray {                                            // retail CArray<CEnginePrimitiveHandle> (12 bytes)
    void* m_pData;
    long  m_size;
    long  m_capacity;
    CPrimitiveHandleArray() : m_pData(0), m_size(0), m_capacity(0) {}
};

class CParticle : public CBase { public:                                                // vtable 0x0129F4F0
    unsigned char                _state[0x7c];                            // +0x04 Orientation .. ClosestAttractorDistance
    CRandomAccelerationScopedPtr RandomAcceleration;                      // +0x80
    CLongArrayArray              PrimIDList;                              // +0x84
    CPrimitiveHandleArray        ParentHandleList;                        // +0x90
    virtual void v00();
    CParticle();
};

CParticle::CParticle()
{
    RandomAcceleration.reset();
}

} // namespace NParticleEngine