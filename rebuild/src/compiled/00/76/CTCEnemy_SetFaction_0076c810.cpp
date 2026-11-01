// CTCEnemy::SetFaction(const CCharString& faction) -- look the named faction up in the world's faction manager and keep it
// Leaf: ?SetFaction@CTCEnemy@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076C810, 30 bytes, thiscall, `ret 4`.
// Identity (naming wave 25): Ego_r.pdb ?SetFaction@CTCEnemy@@QAEXABVCCharString@@@Z and the catalog name. Callees:
//   ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (DECISIONS_W24_RESOLVED item 7), ?GetPFaction@CFactionManager@@QBEPAVCFaction@@
//   ABVCCharString@@@Z 0x006B93D0 (Ego_r.pdb; bound by 0x00771540). Layout: CWorld +0x54 scoped_ptr<CFactionManager> PFactionManager
//   (Ego_r.pdb / engine/CWorld.h); CTCEnemy +0x18 CFaction* PFaction (Ego_r.pdb +0x28: the two STLport lists EnemyThings / AllyThings
//   are 4 bytes each in retail). Was a free SetFaction over an Enemy_GetObj stand-in and Sub::resolve.
class CCharString;
class CFaction;
class CFactionManager { public:
    CFaction* GetPFaction(const CCharString& name) const;   // 0x006B93D0
};
class CWorld { public:
    unsigned char _pad00[0x54];
    CFactionManager* PFactionManager;                    // +0x54 scoped_ptr<CFactionManager> (its one pointer, as the landed CWorld models)
};
class CTCBase {
protected:
    const CWorld& PeekWorld() const;                     // 0x00686A80
};
class CTCEnemy : public CTCBase { public:
    void*         __vftable;                             // +0x00 (CTCBase)
    unsigned char _pad04[0x18 - 0x04];
    CFaction*     PFaction;                              // +0x18
    void SetFaction(const CCharString& faction);
};
void CTCEnemy::SetFaction(const CCharString& faction)
{
    CFactionManager* manager = PeekWorld().PFactionManager;   // the manager in a local: the argument push follows the PeekWorld call
    PFaction = manager->GetPFaction(faction);
}
