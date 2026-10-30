// CInputProcessJumpingAndRolling::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CInputProcessJumpingAndRolling inlined (destroying InitialFacingDirections, Pulses, then ~CAInputProcess), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInputProcessJumpingAndRolling@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048D390, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CInputProcessJumpingAndRolling -- proven; evidence: vtable 0x01238050 slot 0 (RTTI .?AVCInputProcessJumpingAndRolling@@ : CInputProcessGameBase : CAInputProcess); the destroyed members are Ego_r.pdb InitialFacingDirections (list<C3DVector>, +0x3C) and Pulses (list<pair<C2DVector, long> >, +0x38), and 0x00A0D3D0 stores the CAInputProcess vtable 0x0129CA10.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: ~list<C3DVector> 0x0048CA60, ~list<pair<C2DVector, long> > 0x0048CA00, CAInputProcess::~CAInputProcess 0x00A0D3D0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CAInputProcess is implicit (CAInputProcess's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more.
// Model: local -- engine/CInputProcessJumpingAndRolling.h exists but is data-only (vptr flattened into a word), which cannot carry the virtual destructor chain.
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {    // STLport: one node-proxy word
public:
    void* _M_node;
    list();
    ~list();
};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                      // 0x0099A2F0 (FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();             // 0x0099A300 (FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CAInputProcess : public CBaseClassNonCopyable {   // Ego_r.pdb 20 bytes; CInputProcessGameBase adds World .. PGamePlayerInterface
public:
    CAInputProcess();
    virtual ~CAInputProcess();                    // 0x00A0D3D0
    unsigned char _base[0x10];
};
class CInputProcessGameBase : public CAInputProcess {
public:
    void* World;                                  // +0x14
    void* MyPlayer;                               // +0x18
    void* DefinitionManager;                      // +0x1c
    void* Component;                              // +0x20
    void* PGamePlayerInterface;                   // +0x24
};
class CInputProcessJumpingAndRolling : public CInputProcessGameBase {
public:
    C2DVector PulseVector;                                          // +0x28
    long PulseStartTimer;                                           // +0x30
    long PulseEndTimer;                                             // +0x34
    std::list<std::pair<C2DVector, long> > Pulses;                  // +0x38
    std::list<C3DVector> InitialFacingDirections;                   // +0x3c
    C3DVector FacingDirection;                                      // +0x40
    bool StickPulled;                                               // +0x4c
    long WFLastRoll;                                                // +0x50
};
void* MakeOne()
{
    return new CInputProcessJumpingAndRolling;
}