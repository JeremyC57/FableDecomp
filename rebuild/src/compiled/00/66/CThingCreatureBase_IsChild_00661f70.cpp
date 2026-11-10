// CThingCreatureBase::IsChild() const -- true when the creature's definition says child (creature type 1)
// Leaf: ?IsChild@CThingCreatureBase@@QBE_NXZ
// Name fix (agentNF6 2026-11-10): was the free stand-in `bool __fastcall CThingCreatureBase_IsChild(const CThingCreatureBase*)`.
//   Ego_r.pdb ?IsChild@CThingCreatureBase@@QBE_NXZ (const); the landed callers 0x0057BE71 / 0x0057C2E6 / 0x005F4B30 bind it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661F70, 23 bytes: [[this+0xE0]+0xC4] == 1 (the same word GetCreatureType 0x00662000 returns).
// Layout: local model of the two words the body reads (the shared CThingCreatureBase header is the opt-in retail chain).
class CThingCreatureDef { public:                 // Ego_r.pdb CThingCreatureDef (CreatureType donor +0xC8, retail +0xC4)
    char pad[0xC4];
    int CreatureType;                       // +0xC4
};
class CThingCreatureBase { public:
    char pad[0xE0];
    CThingCreatureDef* PDef;                // +0xE0 CDefPointer<const CThingCreatureDef>::Object (Ego_r.pdb PDef)
    bool IsChild() const;
};
bool CThingCreatureBase::IsChild() const
{
    return PDef->CreatureType == 1;
}
