// CPlayerManager::IsThingMainPlayer(const CThing& thing) const -- true for a creature thing owned by the main player
// Leaf: IsThingMainPlayer@CPlayerManager
// Retail: Fable.exe @ 0x004497C0, 32 bytes, `ret 4`; Ego_r.pdb ?IsThingMainPlayer@CPlayerManager@@QBE_NABVCThing@@@Z.
// Body: the thing's type byte (+0x10) is 2 and its PlayerNumber (signed char, +0x90) equals MainPlayer (+0x1c).
// Layout: CPlayerManager MainPlayer at retail +0x1c (Ego_r.pdb +0x20; the Players vector is 12 bytes in STLport);
// CThing PlayerNumber at retail +0x90 (Ego_r.pdb +0xa0; the retail CThing is 0x10 below the PDB there). The byte at +0x10
// is padding in the Ego_r.pdb layout; it is the thing-type tag that thing filters compare with 1 and 2 (e.g. 0x006A7120),
// so it keeps a descriptive placeholder name. Both classes are modelled locally (no retail-accurate shared header).
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingTypeTag;                          // +0x10 (2 = creature)
    unsigned char _pad11[0x90 - 0x11];
    signed char   PlayerNumber;                          // +0x90
};
class CPlayerManager {
public:
    unsigned char _pad00[0x1c];
    long          MainPlayer;                            // +0x1c
    bool IsThingMainPlayer(const CThing& thing) const;
};
bool CPlayerManager::IsThingMainPlayer(const CThing& thing) const
{
    if (thing.ThingTypeTag == 2 && thing.PlayerNumber == MainPlayer)
        return true;
    return false;
}