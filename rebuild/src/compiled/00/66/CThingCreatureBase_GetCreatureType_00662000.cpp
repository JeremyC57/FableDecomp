// CThingCreatureBase::GetCreatureType() const -- the creature type word of the creature's definition ([[this+0xE0]+0xC4])
// Leaf: ?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ
// Name fix (agentNF6 2026-11-10): was the free stand-in `int __fastcall CountReferences(Outer*)` (catalog label GetMovementMode).
//   Ego_r.pdb / FableWin.pdb ?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ (Codex pass 19 header opt-in
//   FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE cites native 0x00662000); IsChild 0x00661F70 tests the same word against 1.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662000, 13 bytes.
enum ECreatureType { };
class CThingCreatureDef { public:                 // Ego_r.pdb CThingCreatureDef (CreatureType donor +0xC8, retail +0xC4)
    char pad[0xC4];
    ECreatureType CreatureType;             // +0xC4
};
class CThingCreatureBase { public:
    char pad[0xE0];
    CThingCreatureDef* PDef;                // +0xE0 CDefPointer<const CThingCreatureDef>::Object (Ego_r.pdb PDef)
    ECreatureType GetCreatureType() const;
};
ECreatureType CThingCreatureBase::GetCreatureType() const
{
    return PDef->CreatureType;
}
