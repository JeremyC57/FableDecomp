// NTCMessages::CMessageBase::GetType() const -- returns 0
// Leaf: ?GetType@CMessageBase@NTCMessages@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C6FD0, thiscall, `ret`.
// Identity: slot 1 of NTCMessages::CMessageBase's vtable 0x012398F8 (defined here: the slot holds this body in 1 vtable(s), all of NTCMessages::CMessageBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CMessageBase@NTCMessages@@UBEKXZ; `ret` matches the signature.
namespace NTCMessages {
class CMessageBase {
public:
    virtual unsigned long GetType() const;
};
unsigned long CMessageBase::GetType() const
{
    return 0;
}
}
