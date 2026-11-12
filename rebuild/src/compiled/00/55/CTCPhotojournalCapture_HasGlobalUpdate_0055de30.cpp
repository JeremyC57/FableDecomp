// CTCPhotojournalCapture::HasGlobalUpdate() -- returns true
// Leaf: ?HasGlobalUpdate@CTCPhotojournalCapture@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055DE30, thiscall, `ret`.
// Identity: slot 28 of CTCPhotojournalCapture's vtable 0x0124C4B4 (defined here: the slot holds this body in 1 vtable(s), all of CTCPhotojournalCapture or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasGlobalUpdate(void) at this slot; Ego_r/FableWin name set: ?HasGlobalUpdate@CTCPhotojournalCapture@@UAE_NXZ; `ret` matches the signature.
class CTCPhotojournalCapture {
public:
    virtual bool HasGlobalUpdate();
};
bool CTCPhotojournalCapture::HasGlobalUpdate()
{
    return true;
}
