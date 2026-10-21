// CMainGameComponent::ConsoleForceUpdateTickSpeed(CMainGameComponent& component, const std::vector<CGenericVar>& args) -- console command: set the forced update tick (on/off, speed multiplier, desired frame rate) unless the editor is active
// Leaf: ?ConsoleForceUpdateTickSpeed@CMainGameComponent@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x00416C35, 85 bytes, plain `ret` (static __fastcall: component in ecx, args in edx); Ego_r.pdb
// ?ConsoleForceUpdateTickSpeed@CMainGameComponent@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z.
// Body: args[0].ValBool, args[1].ValFloat, args[2].ValFloat read first; nothing when component.IsEditorActive() (virtual
// slot 9, +0x24; 0x004197B0 in the CMainGameComponent vtable 0x0122F180); else ForceUpdateTick (+0x34),
// ForceUpdateTickSpeedMultipler (+0x38), ForceUpdateTickSpeedDesiredFramerate (+0x3c); when forcing,
// ForceUpdateNoFailedUpdates (+0x40) = 0 and a multiplier below 0.1f (0x0122EE58) becomes 1.0f.
// Layout: Ego_r.pdb CMainGameComponent +0x34..+0x40 and CGenericVar (28 bytes: vptr, Type, ValString, ValSLONG, ValULONG,
// ValBool +0x14, ValFloat +0x18) -- retail agrees (element stride 0x1c, fields +0x14 / +0x18). No engine/ header for
// CMainGameComponent; vtable slots 0..9 from the retail vtable.
// Opt: size TU with frame pointers (`push ebp / mov ebp,esp`, `leave`, `and [esi+40h],0`).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CGenericVar {
public:
    virtual ~CGenericVar();
    long          Type;                                  // +0x04 EGenericVarType
    void*         ValString;                             // +0x08 CCharString
    long          ValSLONG;                              // +0x0c
    unsigned long ValULONG;                              // +0x10
    bool          ValBool;                               // +0x14
    float         ValFloat;                              // +0x18
};
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
}
class CMainGameComponent {
public:
    virtual void _slot00();
    virtual void _slot01();
    virtual void _slot02();
    virtual void ChangeTextureColourDepth(long depth);   // slot 3
    virtual void SetQuit();                              // slot 4
    virtual void _slot05();
    virtual void _slot06();
    virtual void _slot07();
    virtual void PostInit();                             // slot 8
    virtual bool IsEditorActive() const;                 // slot 9 (+0x24)
    unsigned char _pad04[0x34 - 0x4];
    bool          ForceUpdateTick;                       // +0x34
    float         ForceUpdateTickSpeedMultipler;         // +0x38
    float         ForceUpdateTickSpeedDesiredFramerate;  // +0x3c
    long          ForceUpdateNoFailedUpdates;            // +0x40
protected:
    static void __fastcall ConsoleForceUpdateTickSpeed(CMainGameComponent& component, const std::vector<CGenericVar>& args);
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
void __fastcall CMainGameComponent::ConsoleForceUpdateTickSpeed(CMainGameComponent& component, const std::vector<CGenericVar>& args)
{
    bool force = args[0].ValBool;
    float multiplier = args[1].ValFloat;
    float framerate = args[2].ValFloat;
    if (component.IsEditorActive())
        return;
    component.ForceUpdateTick = force;
    component.ForceUpdateTickSpeedMultipler = multiplier;
    component.ForceUpdateTickSpeedDesiredFramerate = framerate;
    if (force)
    {
        component.ForceUpdateNoFailedUpdates = 0;
        if (multiplier < 0.1f)
            component.ForceUpdateTickSpeedMultipler = 1.0f;
    }
}