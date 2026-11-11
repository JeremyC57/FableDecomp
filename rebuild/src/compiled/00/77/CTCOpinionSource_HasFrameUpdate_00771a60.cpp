// CTCOpinionSource::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCOpinionSource@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00771A60, thiscall, `ret`.
// Identity: slot 8 of CTCOpinionSource's vtable 0x0126A81C (defined here: the slot holds this body in 1 vtable(s), all of CTCOpinionSource or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCOpinionSource@@UBE_NXZ; `ret` matches the signature.
class CTCOpinionSource {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCOpinionSource::HasFrameUpdate() const
{
    return true;
}
