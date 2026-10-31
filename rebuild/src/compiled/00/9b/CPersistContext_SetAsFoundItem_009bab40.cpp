// CPersistContext::SetAsFoundItem() -- after a text-load entry was transferred: a single-transfer context marks itself finished
// Leaf: ?SetAsFoundItem@CPersistContext@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BAB40, 12 bytes, thiscall, plain `ret`.
// Name (2026-10-31, hand621 NAME_FIXES): Ego_r.pdb donor order PersistError 0x00B0F490 -> SetAsFoundItem 0x00B0F540 ->
//   WriteString 0x00B0F550 = retail 0x009BAA90 / 0x009BAB40 / 0x009BAB50; the body is `if (SingleTransfer) Finished = true;`
//   on the Ego_r CPersistContext layout (SingleTransfer +0x16, Finished +0x17); callers are the CPersistContext text-load
//   loops (0x00463334, 0x00475026, 0x0047550F, 0x007AE3D0, 0x00756D00), called after each entry. The earlier landed spelling
//   ?Deactivate_2@CTCCreditsUI@@QAEXXZ (a pad[0x16] stand-in) was a propagated catalog label: Ego_r CTCCreditsUI::Deactivate is
//   a separate function (donor 0x007DFEA0). Bytes unchanged.
// Layout: CPersistContext per Ego_r.pdb (the 0x00463334 / 0x007AE3D0 models).
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CPersistContext { public:
    CDefinitionManager* PDefinitionManager;               // +0x00
    CSymbolMap*         PSymbolMap;                       // +0x04
    CStringParser*      PParser;                          // +0x08
    void (__cdecl*      ErrorCallback)(CCharString*);     // +0x0c
    CCharString*        PStringToClean;                   // +0x10
    bool                UseDefaults;                      // +0x14
    bool                UseCRCCheck;                      // +0x15
    bool                SingleTransfer;                   // +0x16
    bool                Finished;                         // +0x17

    void SetAsFoundItem();
};

void CPersistContext::SetAsFoundItem()
{
    if (SingleTransfer)
    {
        Finished = true;
    }
}
