// CTCOnHeroPush::vfunc_9() -- per-frame update: count frames since the push and remove the TC after five seconds' worth
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BBA70, length 70 bytes.
// Callees: CThing::RemoveTC(long type) @ 0x004C9840 (type 0xc4 = this TC; manifest label "CBaseObject::OnPropertyChanged"
// is propagated). Globals: the frames-per-second long @ 0x01375550; the function-static limit @ 0x013BAE74 with its
// guard word @ 0x013BAE78.
// Shape: a function-local static initialised from a global (`test al,dl` on the guard bit, `lea eax,[eax+eax*4]`),
// then `inc dword ptr [ecx+1Ch]` and the compare against the static re-read.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

enum ETCInterfaceType { };
class CThing { public:
    void RemoveTC(ETCInterfaceType type);                       // 0x004C9840
};

struct CTCBase_007bba70 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCOnHeroPush : public CTCBase_007bba70 { public:
    float DistPushed;                               // +0x0c
    float PushDir[3];                               // +0x10
    long  FrameSincePushed;                         // +0x1c
    virtual void vfunc_9();
};

void CTCOnHeroPush::vfunc_9()
{
    static long s_maxFramesSincePushed = CMainGameComponent::GetConstantFPS() * 5;
    ++FrameSincePushed;
    if (FrameSincePushed > s_maxFramesSincePushed)
        ParentThing->RemoveTC((ETCInterfaceType)(0xc4));
}