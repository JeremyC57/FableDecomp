// CTCTargetingPlayer::ClearLockedTarget() -- drops the player's lock-on: a temporary CIntelligentPointer set to null is copied into the locked-thing pointer (retail +0xe4) and the locked-onto-enemy flag (retail +0xf4) is cleared
// Leaf: ClearLockedTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802650, length 76 bytes (+4 int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10; the temporary's vtable is the CIntelligentPointer copy 0x01238C6C.
// Label: the manifest "CTCScriptedControl::ClearLocked" is propagated (same body shape, other offsets). The only caller is
// CPlayer::UninitModeZTarget (0x006359A5), which looks up thing component 8 of the player creature, and the neighbours
// 0x00802420..0x00802610 are CTCTargetingPlayer methods. engine/CTCTargetingPlayer.h (PDB) puts PTargetLockedThing at +0xec
// and LockedOntoEnemy at +0xfc; retail touches +0xe4 / +0xf4, i.e. the retail layout sits 8 bytes lower here (the usual
// PDB-vs-retail container width drift), so the class is modelled locally.
// Shape: the "temporary CIntelligentPointer" family (0x0084B1C0) with the temporary in an inner scope so the flag store
// follows its destructor.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CTCTargetingPlayer { public:
    unsigned char _pad[0xe4];
    CBaseIntelligentPointer PTargetLockedThing;   // +0xe4 (PDB +0xec)
    unsigned char _padEC[8];
    bool LockedOntoEnemy;                         // +0xf4 (PDB +0xfc)
    void ClearLockedTarget();
};

void CTCTargetingPlayer::ClearLockedTarget()
{
    {
        CIntelligentPointer pointer;
        pointer.SetPItem(0);
        PTargetLockedThing.SetPItem(pointer.GetPItem());
    }
    LockedOntoEnemy = false;
}