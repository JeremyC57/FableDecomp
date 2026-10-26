// CTCInventoryBase::SetPDef(long globalDefIndex) -- remembers the definition's global index and resolves the inventory definition pointer through the game definition manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDB4D, length 27 bytes.
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0, CDefinitionManager::GetDef<CInventoryDef>(long,
//          CDefPointer<const CInventoryDef>*) const 0x0048591C.
// Catalog label "CTCInGameMenu::SetPDef" is propagated (same bytes, other offsets): the body sits among
// the CTCInventoryBase methods (GetCategoryName 0x005BDAC5, GetCategoryNameWithIndex 0x005BDB68), is
// called from ten inventory-flavoured components, and matches Ego_r ?SetPDef@CTCInventoryBase@NInventory@@IAEXJ@Z.
// No engine header for CTCInventoryBase: PDB PDef (CDefPointer<const CInventoryDef>) +0x70 and
// GlobalDefIndex +0x74 sit 8 bytes lower in retail (+0x68 / +0x6c: the two vectors ahead of them are
// 12-byte retail vectors).
// Shape: identical to CTCInGameMenu::SetPDef (0x0047D5F2): arguments pushed before the
// GetDefinitionManager call, the index store scheduled before the call, `call ; ret 4`.
class CInventoryDef;

struct CInventoryDefPointer {                   // CDefPointer<const CInventoryDef>
    const CInventoryDef* Object;
};

class CGameDefinitionManager { public:
    bool GetDef(long globalDefIndex, CInventoryDefPointer* pDef) const;      // 0x0048591C
};

struct CTCInventoryBase {
    unsigned char        _pad_0x00[0x68];
    CInventoryDefPointer PDef;                  // +0x68 (PDB +0x70)
    long                 GlobalDefIndex;        // +0x6c (PDB +0x74)
    const CGameDefinitionManager& GetDefinitionManager() const;              // CTCBase, 0x006869D0
    void SetPDef(long globalDefIndex);
};

void CTCInventoryBase::SetPDef(long globalDefIndex)
{
    GlobalDefIndex = globalDefIndex;
    GetDefinitionManager().GetDef(globalDefIndex, &PDef);
}