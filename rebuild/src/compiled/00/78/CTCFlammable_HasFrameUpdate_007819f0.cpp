// CTCFlammable::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFlammable@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007819F0, thiscall, `ret`.
// Identity: slot 8 of CTCFlammable's vtable 0x0126B1A4 (defined here: the slot holds this body in 1 vtable(s), all of CTCFlammable or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFlammable@@UBE_NXZ; `ret` matches the signature.
class CTCFlammable {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCFlammable::HasFrameUpdate() const
{
    return true;
}
