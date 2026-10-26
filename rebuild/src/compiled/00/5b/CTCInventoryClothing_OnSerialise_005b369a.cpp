// CTCInventoryClothing::OnSerialise(CPersistContext& context, bool inGameSave) -- serialises the base inventory, then the "InGameSave" flag: written from the argument when saving, read into LoadedFromSaveGame when loading
// Leaf: OnSerialise@CTCInventoryClothing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B369A, length 79 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Virtual slot 1 of the CTCInventoryClothing vtable 0x0124356C.
// Callees: CTCInventoryBase::OnSerialise(CPersistContext&, bool) @ 0x005BFD5E, CPersistContext::IsSaving @ 0x004AEF50 (landed;
// the manifest's "IsBinary" is a misread), CPersistContext::Transfer<bool>(name, bool&, const bool& default) @ 0x004045C0 (the
// saving arm, default false), CPersistContext::Transfer(name, bool&) @ 0x004107C0 (the out-of-line default-less overload, the
// loading arm). String "InGameSave" @ 0x012539D0.
// Model: engine/CTCInventoryClothing.h (PDB) puts LoadedFromSaveGame at +0x181; the retail body uses +0x15D (the retail base and
// the STLport map/vector members are narrower), so a local view carries the PDB name at the retail offset.
// Shape: the saving arm transfers the PARAMETER by reference (`lea eax,[ebp+0Ch]`); its `false` default temporary is homed in
// the high byte of the dead `context` slot (`lea eax,[ebp+0Bh]`, `mov byte ptr [ebp+0Bh],0`); `mov ecx,edi` is hoisted above the
// branch because both arms call on `context`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CPersistContext { public:
    void Transfer(const char* name, bool& value, const bool& defaultValue); // 0x004045C0
    void Transfer(const char* name, bool& value);                           // 0x004107C0
public:
    bool IsSaving() const;   // 0x004AEF50
};
struct CTCInventoryBase_Retail {
    void OnSerialise(CPersistContext& context, bool inGameSave);             // 0x005BFD5E
};
struct CTCInventoryClothing_Retail : CTCInventoryBase_Retail {
    unsigned char _pad_0x000[0x15D];
    bool          LoadedFromSaveGame;                                        // +0x15D
    void OnSerialise(CPersistContext& context, bool inGameSave);
};
void CTCInventoryClothing_Retail::OnSerialise(CPersistContext& context, bool inGameSave)
{
    CTCInventoryBase_Retail::OnSerialise(context, inGameSave);
    if (context.IsSaving())
        context.Transfer("InGameSave", inGameSave, false);
    else
        context.Transfer("InGameSave", LoadedFromSaveGame);
}