// CGameComponent::`scalar deleting destructor'(unsigned int) -- reset both vptrs, run ~CBaseClassNonCopyable, free when bit 0 is set
// Leaf: ??_GCGameComponent@@UAEPAXI@Z
// Retail: Fable.exe @ 0x00415940, 43 bytes, `ret 4`; slot 0 of vtable 0x0122ECB0, whose COL (0x0132CFC8) names
// .?AVCGameComponent@@ with bases CBaseClassNonCopyable : CBaseClass at +0 and CDeviceResetCallback at +4. Catalog label
// CLandscapeBackgroundPatch::`vector_deleting_destructor' is wrong (no array path). Body: the inline user destructor
// stores the CGameComponent vptrs 0x0122ECB0 (+0) and 0x0122ECA4 (+4), calls the out-of-line ~CBaseClassNonCopyable
// (0x0099A300), then operator delete (0x00BFE9BC) when (flags & 1); returns this.
// Layout: Ego_r.pdb CGameComponent (16 bytes): Quit +8, Running +9, Game +0xc. The CDeviceResetCallback vtable in this
// object has two __purecall slots and no destructor slot, so that base has no virtual (or user) destructor.
// Shape: /O2 (aligned entry, `add esp,4` after delete). The constructor (0x0051D8E0) stores the callback vptr inline;
// a placement-new helper makes VC7.1 emit the vtable and ??_G here.
#include <new>
class CGame;
class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CDeviceResetCallback {
public:
    virtual bool OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
class CGameComponent : public CBaseClassNonCopyable, public CDeviceResetCallback {
public:
    bool   Quit;                                         // +0x08
    bool   Running;                                      // +0x09
    CGame* Game;                                         // +0x0c
    CGameComponent(CGame* game) : Quit(false), Running(false), Game(game) {}
    virtual ~CGameComponent() {}
    virtual void Init();                                 // slot 1 (0x00415920, empty)
    virtual void Run() = 0;                              // slot 2 (__purecall; name descriptive)
    virtual void Shutdown() = 0;                         // slot 3 (__purecall; name descriptive)
protected:
    virtual void SetQuit();                              // slot 4 (0x00415930, empty)
};
class CGameComponent_Concrete : public CGameComponent {
public:
    CGameComponent_Concrete() : CGameComponent(0) {}
    virtual void Run();
    virtual void Shutdown();
    virtual bool OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
};
void ConstructCGameComponent(void* p)                    // emission helper, not a retail function: CGameComponent's vtable (and its ??_G) is emitted
{
    new (p) CGameComponent_Concrete;
}