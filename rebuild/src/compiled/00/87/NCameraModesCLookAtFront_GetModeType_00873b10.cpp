// NCameraModes::CLookAtFront::GetModeType() const -- returns (EMode)0xd
// Leaf: ?GetModeType@CLookAtFront@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873B10, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CLookAtFront's vtable 0x01277714 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CLookAtFront or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CLookAtFront@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CLookAtFront {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CLookAtFront::GetModeType() const
{
    return (EMode)0xd;
}
}
