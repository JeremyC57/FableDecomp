VARIANTS = [
 ("u1", [("    if (!g_pResourceManager->GraphicBanks[0]->EntryExists(sprite.BankIndex))", "    if (!g_pResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->EntryExists(sprite.BankIndex))"),
         ("    const CGraphicInfo* info = g_pResourceManager->GraphicBanks[0]->GetGraphicInfo(sprite.BankIndex);", "    const CGraphicInfo* info = g_pResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->GetGraphicInfo(sprite.BankIndex);")], None, "PeekGraphicBank"),
]
