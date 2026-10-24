// boost::scoped_ptr::~scoped_ptr() -- the scoped_ptr<NCameraModes::CPhysicsForceModelBase> destructor: checked_delete of the owned force model (its non-virtual destructor, then operator delete)
// Leaf: ??1?$scoped_ptr@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00850530, 25 bytes, plain `ret` (ecx = the scoped_ptr).
// Identity: Ego_r.pdb CPhysicsLeaf<NCameraModes::CPhysicsTargetModelGoTo, CPhysicsForceModelGotoOutsideSimple,
//   CPhysicsCoordCartesian> holds `scoped_ptr<NCameraModes::CPhysicsForceModelBase> OurForce` at +0xc; its landed
//   scalar deleting destructor 0x00850670 calls this body on `lea ecx,[esi+0Ch]` (the other caller is 0x0085CA58).
//   The manifest label "DiscardAsyncData" is descriptive and wrong.
// Callees: NCameraModes::CPhysicsForceModelBase::~CPhysicsForceModelBase() 0x00850360 (compiler-generated, non-virtual;
//   manifest label "~CPatchTesselationEdgeStrip" is propagated), operator delete 0x00BFE9BC.
// Shape: boost `~scoped_ptr() { boost::checked_delete(ptr); }` with checked_delete inline: the pointer is loaded once,
//   null-tested by the delete expression, destroyed and freed; the member is not cleared.
void __cdecl operator delete(void* block);               // 0x00BFE9BC

namespace NCameraModes {
class CPhysicsForceModelBase {                           // polymorphic (FrameUpdate, Do*Evasion ...), non-virtual destructor
public:
    virtual void FrameUpdate();
    ~CPhysicsForceModelBase();                           // 0x00850360
private:
    unsigned char _body[0xac - 4];                       // retail 0xAC: Things +0x08, Themes +0x2C, Ground +0x50 (each
                                                         // CPhysicsCollisionPointsObject 0x24 with STLport lists, PDB 0x3C)
};
}

namespace boost {
template <class T> inline void __fastcall checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}

template <class T> class scoped_ptr {
public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
private:
    T* ptr;
};
}

template class boost::scoped_ptr<NCameraModes::CPhysicsForceModelBase>;