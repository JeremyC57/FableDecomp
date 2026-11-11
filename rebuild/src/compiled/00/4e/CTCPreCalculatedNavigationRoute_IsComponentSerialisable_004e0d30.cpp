// CTCPreCalculatedNavigationRoute::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCPreCalculatedNavigationRoute@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0D30, thiscall, `ret`.
// Identity: slot 23 of CTCPreCalculatedNavigationRoute's vtable 0x01242444 (defined here: the slot holds this body in 1 vtable(s), all of CTCPreCalculatedNavigationRoute or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCPreCalculatedNavigationRoute@@UBE_NXZ; `ret` matches the signature.
class CTCPreCalculatedNavigationRoute {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCPreCalculatedNavigationRoute::IsComponentSerialisable() const
{
    return true;
}
