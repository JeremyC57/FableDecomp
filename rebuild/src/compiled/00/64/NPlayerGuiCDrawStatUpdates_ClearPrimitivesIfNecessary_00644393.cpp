// NPlayerGui::CDrawStatUpdates::ClearPrimitivesIfNecessary() -- when not drawing this frame, clear the player GUI PStatUpdates bar's primitive handles
// Leaf: ?ClearPrimitivesIfNecessary@CDrawStatUpdates@NPlayerGui@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00644393, 25 bytes, thiscall, plain `ret` (the bar call is a tail jump).
// Identity (proven): slot 0 of vtable 0x0125A5F4 (RTTI .?AVCDrawStatUpdates@NPlayerGui@@); Ego_r.pdb ?ClearPrimitivesIfNecessary@CDrawStatUpdates@NPlayerGui@@UAEXXZ;
//   the bar is CPlayerGui::PStatUpdates (Ego_r.pdb CCountedPointer<NPlayerGui::CMultiStatBar> +0x308, retail +0x2F4), whose slot 1 is
//   CStatBar::ClearPrimitiveHandles (Ego_r.pdb UAEXXZ). The manifest's vfunc_0 label is a placeholder.
// Shape (family M176, representative 0x0064431B): `if (!Drawing && PlayerGui->PStatUpdates) PlayerGui->PStatUpdates->ClearPrimitiveHandles();`
//   (CCountedPointer's operator bool / operator-> on the object word). Size TU.
#pragma optimize("s", on)
class CPlayerGui;
namespace NPlayerGui {
class CStatBar {
public:
    virtual void v00();
    virtual void ClearPrimitiveHandles();                 // slot 1 (+0x04)
};
class CMultiStatBar : public CStatBar { };
}
template <class T> class CCountedPointer {               // 8 bytes: object, pointer info
public:
    T* Data;
    void* Info;
    operator bool() const { return Data != 0; }
    T* operator->() const { return Data; }
};
class CPlayerGui {
public:
    unsigned char _pad000[0x2F4];
    CCountedPointer<NPlayerGui::CMultiStatBar> PStatUpdates;              // +0x2F4 (Ego_r.pdb +0x308)
};
namespace NPlayerGui {
class CDrawBase {
public:
    virtual void ClearPrimitivesIfNecessary();            // slot 0
    CPlayerGui* PlayerGui;                                // +0x04
    void* PlayerManager;                                  // +0x08
    void* World;                                          // +0x0c
    void* Component;                                      // +0x10
    void* Bank;                                           // +0x14
    bool Drawing;                                         // +0x18
};
class CDrawStatUpdates : public CDrawBase {
public:
    virtual void ClearPrimitivesIfNecessary();
};
void CDrawStatUpdates::ClearPrimitivesIfNecessary()
{
    if (!Drawing && PlayerGui->PStatUpdates)
        PlayerGui->PStatUpdates->ClearPrimitiveHandles();
}
}