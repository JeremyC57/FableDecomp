// NInventory::CCategory::GetHighlightedItemText(const CGameDefinitionManager&) const -- the highlighted item's description; when that is empty, the TXT_OBJT_EMPTY game text (looked up by its tag) through the owning inventory, if the tag exists
// Leaf: ?GetHighlightedItemText@CCategory@NInventory@@
// Retail: Fable.exe @ 0x005BDA1D, 168 bytes, __fastcall `ret 8` (hidden CWideString result + the unused definition
// manager). Ego_r.pdb `?GetHighlightedItemText@CCategory@NInventory@@QBE?AVCWideString@@ABVCGameDefinitionManager@@@Z`.
// Callees: NInventory::CItem::GetItemDescription() const @ 0x005BD8A0 (hidden result), wcscmp @ 0x00BFE94C (import),
// the STLport `operator==(const wstring&, const wchar_t*)` @ 0x00404280 (cdecl bool), CCharString(const char*, long)
// @ 0x0099EBF0, CCharString::GetCRC() const's out-of-line copy @ 0x00404310, NGameText::NGameText::CDataBank::LookupValue(unsigned
// long) const @ 0x009CC410 on g_pMainGameComponent (0x013B86A0)->TextBank (+0x14), ~CCharString @ 0x0099EAE0,
// CTCInventoryBase::GetInventoryText(long) @ 0x005BD273 on PInventory (+0x18, straight into the result),
// CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510. Literals: L"" 0x0122D70C,
// "TXT_OBJT_EMPTY" 0x01253CB8.
// Layout: retail CCategory 0x2C bytes (ItemsInCategory begin +0x08, PInventory +0x18, HighlightedItemIndex +0x24);
// CItem 0x14 bytes (engine/CItem.h).
// Shape: frame-pointer size TU; CWideString's inline `== L""` is written as two separate tests in the caller that `goto`
// the empty arm (the landed CWorldMap::SetAsUsingWAD form): no data block -> wcscmp(L"", L"") (the literal loaded once
// into eax and pushed twice), else the out-of-line wstring compare; a forced-inline operator== materialises the wcscmp
// bool (170v168), a plain inline one stays a call (140v168). The bank is read into edi before the symbol temporary;
// GetCRC is called out of line on that explicit temporary (its constructor's eax); the tag is a signed `> 0` (`jle`).
// Label: the catalog's NParticleMorphs_GetDisplayText is this method.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
typedef unsigned short wchar_t;                         // VC7.1 without /Zc:wchar_t
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);   // 0x00BFE94C
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
class CCharStringData { public:
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return Calc(0, GetString(), GetLength()); }   // out-of-line copy 0x00404310
};
namespace std {
template <class _CharT> class char_traits { };       // STLport 4.6 class templates (mangle V)
template <class _Tp> class allocator { };
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {                                    // STLport basic_string<wchar_t> (the CWideString data block)
public:
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
};
typedef basic_string<wchar_t> wstring;
template <class _CharT, class _Traits, class _Alloc>
bool __cdecl operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __t);   // 0x00404280 (out of line)
}
class CWideString {
public:
    std::wstring* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
namespace NGameText {
class CDataBank {
public:
public:
    unsigned long LookupValue(unsigned long) const;   // 0x009CC410
};
}
class CMainGameComponent { public:
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                     // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CGameDefinitionManager;
namespace NInventory {
class CTCInventoryBase {
public:
    CWideString GetInventoryText(long textId);          // 0x005BD273
};
class CItemText : public CItem {                        // NInventory::CItem's text accessor
public:
    CWideString GetItemDescription() const;             // 0x005BD8A0
};
class CCategory {                                       // retail layout, 0x2C bytes
public:
    unsigned char PAssociatedThing[0x8];                // +0x00 CIntelligentPointer<CThing>
    CItemText* ItemsInCategory_start;                   // +0x08 vector<NInventory::CItem> (retail 12 bytes)
    CItemText* ItemsInCategory_finish;                  // +0x0C
    CItemText* ItemsInCategory_end_of_storage;          // +0x10
    void* PDef_Object;                                  // +0x14 CDefPointer<const CInventoryCategoryDef>
    CTCInventoryBase* PInventory;                       // +0x18
    long GlobalDefIndex;                                // +0x1C
    long SelectedItemIndex;                             // +0x20
    long HighlightedItemIndex;                          // +0x24
    bool Selected;                                      // +0x28
    bool DisplayItems;                                  // +0x29
    CWideString GetHighlightedItemText(const CGameDefinitionManager& definitions) const;
};
CWideString CCategory::GetHighlightedItemText(const CGameDefinitionManager& definitions) const
{
    CWideString text = ItemsInCategory_start[HighlightedItemIndex].GetItemDescription();
    if (text.PStringData == 0) {
        if (wcscmp(L"", L"") == 0)
            goto empty;
    } else if (*text.PStringData == L"")
        goto empty;
    return text;
empty:
    {
        NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
        long textId = bank->LookupValue(CCharString("TXT_OBJT_EMPTY").GetCRC());
        if (textId > 0)
            return PInventory->GetInventoryText(textId);
    }
    return text;
}
}