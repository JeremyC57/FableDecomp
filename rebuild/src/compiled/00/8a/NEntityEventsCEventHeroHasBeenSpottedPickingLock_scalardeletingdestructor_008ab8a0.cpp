// NEntityEvents::CEventHeroHasBeenSpottedPickingLock::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CEventHeroHasBeenSpottedPickingLock inlined (PSpotter then PTarget, then CEventBase's inline destructor) and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEventHeroHasBeenSpottedPickingLock@NEntityEvents@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AB8A0, 47 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable slot 0.
// Identity (proven): NEntityEvents::CEventHeroHasBeenSpottedPickingLock::`scalar deleting destructor'; evidence: slot 0 of vtable 0x012781C8 (RTTI
//   .?AVCEventHeroHasBeenSpottedPickingLock@NEntityEvents@@ : CEventBase@NEntityEvents), the vtable the J108 constructor 0x008AB840 stores.
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family J109: deleting destructors of byte-identical shape; retail has no
//   identical-code folding).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (Ego_r.pdb ??1CBaseIntelligentPointer@@UAE@XZ;
//   each CIntelligentPointer<CThing> member's implicit destructor is its base's, called directly), operator delete 0x00BFE9BC.
// Shape: no vtable store for CEventHeroHasBeenSpottedPickingLock (its destructor is implicit: VC7.1 resets the vptr only in user-written destructors);
//   the members are destroyed in reverse order, then CEventBase's user-written inline destructor stores 0x0124F2C4 (RTTI
//   .?AVCEventBase@NEntityEvents@@). The deleting destructor is emitted with the vtable in a TU that constructs the class:
//   MakeOne is that construction (a modelling device; the constructor is J108's).
// Model: local, as J108 (no engine headers for the event classes).
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
public:
    CBaseObjectPointer* IntelligentPointer;                 // +0x04
};
class CThing : public CBaseObject { };                      // partial view
class CBaseIntelligentPointer {                             // 8 bytes
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};                                                          // implicit destructor (its vtable's one slot)
namespace NEntityEvents {
class CEventBase {                                          // Ego_r.pdb 8 bytes, vtable 0x0124F2C4
public:
    CEventBase() : WorldFrame(0) { }
    virtual ~CEventBase() { }
    long WorldFrame;                                        // +0x04
};
class CEventHeroHasBeenSpottedPickingLock : public CEventBase {   // Ego_r.pdb 24 bytes, vtable 0x012781C8
public:
    CEventHeroHasBeenSpottedPickingLock(CThing* a, CThing* b) : PTarget(a), PSpotter(b) { }   // J108 (0x008AB840)
    CIntelligentPointer<CThing> PTarget;                  // +0x08
    CIntelligentPointer<CThing> PSpotter;                  // +0x10
};
}   // namespace NEntityEvents
void* MakeOne(CThing* a, CThing* b)
{
    return new NEntityEvents::CEventHeroHasBeenSpottedPickingLock(a, b);
}