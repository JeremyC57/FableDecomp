CGameDefinitionManager: inspected rebuild/include/engine/_quarantine/CGameDefinitionManager.h. Its emitted struct contains a vptr (four bytes on native ABI), while its generated size assertion requires one byte. The header is internally inconsistent. Sources use declaration-only method views for actual native Get/GetDefGlobalIndexFromName/GetObjectDef; no manager data fields are accessed and no invalid header assertion is suppressed. The fixtures stub these actual external callees.

CTCInventoryBase: actual quarantined header is now included by 008986d0/00898840 and their InventoryMethods derive from it. The methods use no disputed donor fields.

CTCBuyableHouse: clean actual header included in 00895ed0; BuyableHouseMethods inherits it.
