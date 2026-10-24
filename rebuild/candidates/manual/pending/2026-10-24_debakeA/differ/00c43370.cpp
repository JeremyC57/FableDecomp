// Grade: DIFFER(8v21) -- retail keeps `push ; push ; call [eax+30h] ; ret 8` where VC7.1 turns this forwarder into the sibling `jmp [eax+30h]` (GOTCHAS line 41 / FINDINGS F148: bool, named bool local and optimize("g",off) tried; void/long/__stdcall/optimize("y",off) already failed for the F148 family)
// CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(const C3DVector& destination, float radius) const
//   -- forward the query to the navigator object held at +0x50 (its vtable slot 12)
// Retail 0x00C43370, 21 bytes; only caller 0x00C35C84 (a __stdcall dispatcher that picks this or
// 0x00C43980 by the owner's flags). Signature from Ego_r.pdb
// (?IsNewDestinationGoingToResetNavigation@CTCCreatureNavigation@@QBE_NABVC3DVector@@M@Z); the
// label sits in the 0x00C4xxxx library band, so the owning class is the PDB's, the +0x50
// navigator member is modelled from the body.
// Leaf: ?IsNewDestinationGoingToResetNavigation@CTCCreatureNavigation

struct C3DVector { float X, Y, Z; };

class CNavigator
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual bool IsNewDestinationGoingToResetNavigation(const C3DVector& destination, float radius);   // slot 12 (+0x30)
};

class CTCCreatureNavigation
{
public:
    bool IsNewDestinationGoingToResetNavigation(const C3DVector& destination, float radius) const;
    unsigned char _base[0x50];      // +0x00..+0x4F (not touched)
    CNavigator* m_pNavigator;       // +0x50
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(const C3DVector& destination, float radius) const
{
    bool reset = m_pNavigator->IsNewDestinationGoingToResetNavigation(destination, radius);
    return reset;
}
