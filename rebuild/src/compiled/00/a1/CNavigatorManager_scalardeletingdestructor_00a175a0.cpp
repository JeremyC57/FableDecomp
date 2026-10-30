// CNavigatorManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CNavigatorManager inlined (destroying QueueManager, QuadTrees, then ~CClassFactory), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCNavigatorManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A175A0, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CNavigatorManager -- proven; evidence: vtable 0x0129CA84 slot 0 (RTTI .?AVCNavigatorManager@@ : CClassFactory); the destroyed members are Ego_r.pdb QueueManager (CNavigatorQueueManager, +0x2C: its only member is the list<pair<long, const CNavigatorBase*> > Navigators, whose destructor 0x00A16CB0 is called) and QuadTrees (list<CNavQuadTreeMap>, +0x24, destructor 0x00A17570); 0x00A37360 stores the CClassFactory vtable 0x0129CF70 (PDB name set: the protected ??1CClassFactory@@MAE@XZ).
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: ~list<pair<long, const CNavigatorBase*> > 0x00A16CB0 (the implicit ~CNavigatorQueueManager), ~list<CNavQuadTreeMap> 0x00A17570, CClassFactory::~CClassFactory 0x00A37360, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CClassFactory is implicit (CClassFactory's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more.
// Model: local -- engine/CNavigatorManager.h exists but is data-only (vptr flattened into a word), which cannot carry the virtual destructor chain.
class CNavigatorBase; class CNavQuadTreeMap; class CNavigatorManagerExternalInterface;
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
class CClassFactory : public CBaseClassNonCopyable {   // retail 0x1C bytes (Ego_r.pdb 40: ClassTypeInfo vector, ClassList list)
public:
    CClassFactory();
protected:
    virtual ~CClassFactory();                     // 0x00A37360
public:
    unsigned char _base[0x18];
};
class CNavigatorQueueManager {
public:
    std::list<std::pair<long, const CNavigatorBase*> > Navigators;    // +0x00
};
class CNavigatorManager : public CClassFactory {
public:
    unsigned long* Seed;                          // +0x1c
    long NumberOfNodeExaminationsLeftThisFrame;   // +0x20
    std::list<CNavQuadTreeMap> QuadTrees;         // +0x24
    CNavigatorManagerExternalInterface* PInterface;   // +0x28
    CNavigatorQueueManager QueueManager;          // +0x2c
};
void* MakeOne()
{
    return new CNavigatorManager;
}