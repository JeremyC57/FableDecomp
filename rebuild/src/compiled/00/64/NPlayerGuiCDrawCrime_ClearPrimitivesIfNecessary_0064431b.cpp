// NPlayerGui::CDrawCrime::ClearPrimitivesIfNecessary() -- when not drawing this frame, clear the player GUI PCrime bar's primitive handles
// Leaf: ?ClearPrimitivesIfNecessary@CDrawCrime@NPlayerGui@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064431B, 25 bytes, thiscall, plain `ret` (the bar call is a tail jump).
// Identity (proven): slot 0 of vtable 0x0125A568 (RTTI .?AVCDrawCrime@NPlayerGui@@); Ego_r.pdb ?ClearPrimitivesIfNecessary@CDrawCrime@NPlayerGui@@UAEXXZ;
//   the bar is CPlayerGui::PCrime (Ego_r.pdb CCountedPointer<NPlayerGui::CCrimeStatBar> +0x300, retail +0x2EC), whose slot 1 is
//   CStatBar::ClearPrimitiveHandles (Ego_r.pdb UAEXXZ). The manifest's vfunc_0 label is a placeholder.
// Shape (family M176, representative 0x0064431B): `if (!Drawing && PlayerGui->PCrime) PlayerGui->PCrime->ClearPrimitiveHandles();`
//   (CCountedPointer's operator bool / operator-> on the object word). Size TU.
#pragma optimize("s", on)
class CPlayerGui;
namespace NPlayerGui {
class CStatBar {
public:
    virtual void v00();
    virtual void ClearPrimitiveHandles();                 // slot 1 (+0x04)
};
class CCrimeStatBar : public CStatBar { };
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
    unsigned char _pad000[0x2EC];
    CCountedPointer<NPlayerGui::CCrimeStatBar> PCrime;              // +0x2EC (Ego_r.pdb +0x300)
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
class CDrawCrime : public CDrawBase {
public:
    virtual void ClearPrimitivesIfNecessary();
};
void CDrawCrime::ClearPrimitivesIfNecessary()
{
    if (!Drawing && PlayerGui->PCrime)
        PlayerGui->PCrime->ClearPrimitiveHandles();
}
}