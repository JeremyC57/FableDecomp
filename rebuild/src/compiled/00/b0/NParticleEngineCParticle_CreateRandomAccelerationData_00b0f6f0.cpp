// NParticleEngine::CParticle::CreateRandomAccelerationData(unsigned long& seed) -- replaces the particle's random-acceleration block with a freshly seeded one (pool-allocated), deleting the old block
// Leaf: CreateRandomAccelerationData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0F6F0, length 65 bytes, `ret 4` (ends in the deleting-destructor tail jump).
// Signature from Ego_r.pdb: ?CreateRandomAccelerationData@CParticle@NParticleEngine@@QAEXAAK@Z (no direct caller or
//   reference in retail: an unreferenced out-of-line copy).
// Callees: CRandomAcceleration::operator new(unsigned int) @ 0x00AF8680 (`??2CRandomAcceleration@@SIPAXI@Z`, static
//   __fastcall: the 0x18 size in ecx, the LOCAL_HEAP_CRandomAcceleration pool), CRandomAcceleration::CRandomAcceleration(
//   unsigned long&) @ 0x00B0A210, and the old block's virtual deleting destructor (vtable slot 0, flag 1).
// Layout (Ego_r.pdb): CParticle::RandomAcceleration is a boost::scoped_ptr<CRandomAcceleration> at +0x80.
// Shape: `RandomAcceleration.reset(new CRandomAcceleration(seed))` with boost's `this_type(p).swap(*this)` reset inlined:
//   the old pointer is read, the new one stored, then the temporary's checked_delete runs `delete old` -- a null test and
//   `call [vtable]` with 1, which VC turns into a sibling `jmp dword ptr [edx]` reusing the `seed` argument slot.
namespace NParticleEngine {

class CRandomAcceleration {
public:
    unsigned char TimeOffsetX;                       // +0x04
    unsigned char TimeOffsetY;                       // +0x05
    unsigned char TimeOffsetZ;                       // +0x06
    unsigned char IndexX;                            // +0x07
    unsigned char IndexY;                            // +0x08
    unsigned char IndexZ;                            // +0x09
    unsigned char _pad_0x0a[0x0E];                   // to 0x18 bytes
    CRandomAcceleration(unsigned long& seed);        // 0x00B0A210
    virtual ~CRandomAcceleration();                  // slot 0: deleting destructor
    static void* __fastcall operator new(unsigned int size);   // 0x00AF8680
    static void __fastcall operator delete(void* p);
};

}   // namespace NParticleEngine

namespace boost {

template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}

template <class T> class scoped_ptr {
    T* ptr;
    typedef scoped_ptr<T> this_type;
public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
    void reset(T* p = 0) { this_type(p).swap(*this); }
    void swap(scoped_ptr& b)
    {
        T* tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }
    T* get() const { return ptr; }
};

}   // namespace boost

namespace NParticleEngine {

class CParticle {
public:
    unsigned char _pad_0x00[0x80];
    boost::scoped_ptr<CRandomAcceleration> RandomAcceleration;   // +0x80
    void CreateRandomAccelerationData(unsigned long& seed);
};

void CParticle::CreateRandomAccelerationData(unsigned long& seed)
{
    RandomAcceleration.reset(new CRandomAcceleration(seed));
}

}   // namespace NParticleEngine