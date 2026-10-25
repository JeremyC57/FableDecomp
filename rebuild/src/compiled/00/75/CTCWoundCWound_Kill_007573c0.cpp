// CTCWound::CWound::Kill(CTCTextureDecal&) -- delete the wound's four texture/bump decals (wound and scar) and clear their ids
// Leaf: ?Kill@CWound@CTCWound@@QAEXAAVCTCTextureDecal@@@Z
// Retail: Fable.exe @ 0x007573C0, 89 bytes, `ret 4`. Ego_r.pdb ?Kill@CWound@CTCWound@@QAEXAAVCTCTextureDecal@@@Z; the only
// caller is CTCWound's update loop 0x007577BB (the node's CWound at node+8, the decal component pushed).
// Callees: CTCTextureDecal::DeleteDecal(long) 0x0070A430 (landed).
// Layout (Ego_r.pdb CTCWound::CWound, 0x44): Desc +0, Duration +0x28, WoundTextureDecalEntryID +0x2C,
// WoundBumpDecalEntryID +0x30, ScarTextureDecalEntryID +0x34, ScarBumpDecalEntryID +0x38, MorphIndex +0x3C, Dead +0x40.
// Shape: four `if (id) { decals.DeleteDecal(id); id = 0; }` blocks; the zero lives in ebx.
class CTCTextureDecal {                             // PDB class (mangled V)
public:
    void DeleteDecal(long id);                      // 0x0070A430
};

class CTCWound {
public:
    class CWound {
    public:
        void Kill(CTCTextureDecal& decals);
    private:
        unsigned char Desc[0x28];                   // +0x00 CEntry
        unsigned long Duration;                     // +0x28
        long          WoundTextureDecalEntryID;     // +0x2C
        long          WoundBumpDecalEntryID;        // +0x30
        long          ScarTextureDecalEntryID;      // +0x34
        long          ScarBumpDecalEntryID;         // +0x38
        long          MorphIndex;                   // +0x3C
        bool          Dead;                         // +0x40
        bool          HadUpdate;                    // +0x41
    };
};

void CTCWound::CWound::Kill(CTCTextureDecal& decals)
{
    if (WoundTextureDecalEntryID) {
        decals.DeleteDecal(WoundTextureDecalEntryID);
        WoundTextureDecalEntryID = 0;
    }
    if (WoundBumpDecalEntryID) {
        decals.DeleteDecal(WoundBumpDecalEntryID);
        WoundBumpDecalEntryID = 0;
    }
    if (ScarTextureDecalEntryID) {
        decals.DeleteDecal(ScarTextureDecalEntryID);
        ScarTextureDecalEntryID = 0;
    }
    if (ScarBumpDecalEntryID) {
        decals.DeleteDecal(ScarBumpDecalEntryID);
        ScarBumpDecalEntryID = 0;
    }
}