// CTCFireballSpell::SetWeaponState(long state) -- store the weapon state and pick the hand the fireball attaches to: state 1 the left-hand name, state 2 the right-hand name, state 0 whichever hand the cast animation uses
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C8660, length 82 bytes.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 on FireballAttachmentName; the two attachment-name
// strings are global CCharString objects in .bss @ 0x013BAC30 (right hand) and 0x013BAC34 (left hand).
// Layout: PDB CTCFireballSpell (132 bytes) puts WeaponState at +0x68, LeftHandAnim +0x6c, FireballAttachmentName
// +0x70; retail is 4 lower from WeaponState on (+0x64 / +0x68 / +0x6c: the CWillButtonHelperTimer member at +0xc is
// 4 bytes smaller in the shipped build), so the members are modelled locally over the retail offsets.
// Shape: a three-case `switch` lowered to `sub eax,0 ; je / dec eax ; je / dec eax ; jne`; every case ends in the
// assignment, which becomes a tail jump reusing the `state` argument slot for the string's address
// (`mov [esp+4],offset g ; add ecx,6Ch ; jmp operator=`); case 0 selects the address with a ternary.
// Name fix (agentNF6 2026-11-10): 0x013BAC30 is ?PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME@@3VCCharString@@B (const CCharString "WEAPON_FOCUS_01"; Ego_r.pdb / FableWin.pdb public records and their dynamic initializer names).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAC34 SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME (global, const; initializer 0x0121E620), was g_LeftHandAttachmentName.
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;      // 0x013BAC30
extern const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;       // 0x013BAC34

class CTCFireballSpell { public:
    unsigned char _pad[0x64];
    long          WeaponState;                     // +0x64 EWEAPON_STATE
    bool          LeftHandAnim;                    // +0x68
    CCharString   FireballAttachmentName;          // +0x6c
    void SetWeaponState(long state);
};

void CTCFireballSpell::SetWeaponState(long state)
{
    WeaponState = state;
    switch (state) {
    case 0:
        FireballAttachmentName = LeftHandAnim ? SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME : PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;
        break;
    case 1:
        FireballAttachmentName = SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;
        break;
    case 2:
        FireballAttachmentName = PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;
        break;
    }
}