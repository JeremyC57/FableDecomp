// CTCInventoryItem::GetDescription(long defIndex) -- static: cache the item def for defIndex and return its description text (ItemDescription looked up in the main component's text bank), or the empty wide string when the def is missing or has no description
// Leaf: ?GetDescription@CTCInventoryItem@@SI?AVCWideString@@J@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x005D95C0, 112 bytes (+2 int3). Ego_r.pdb `?GetDescription@CTCInventoryItem@@SI?AVCWideString@@J@Z`
// (static __fastcall: the hidden CWideString result in ecx, defIndex in edx; `ret`). Twin: GetDetails 0x005D9630 (+0x38).
// Callees: CTCInventoryItem::CacheDef(long) @ 0x005D94B0 (static `__fastcall` bool: loads the thing def and its
// CInventoryItemDef into the static cache), NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070 on
// CMainGameComponent::Get() (0x013B86A0)->TextBank (+0x14) (a CIVCountedPointer<const CGroupEntry> by hidden pointer, in the
// `push ecx` frame word), NGameText::CGroupEntry::GetText() const @ 0x009C7A40 (straight into this function's result),
// CWideString::CWideString(const CWideString&) @ 0x0099B720 on the empty wide string 0x013BCA24, the entry's slot 1 on release.
// Globals: 0x013B8C18 the cached CInventoryItemDef (engine/CInventoryItemDef.h: ItemDescription +0x34, ItemDetails +0x38).
// Shape: speed TU; the cached def is reached through CDefPointer::operator-> at both uses, so the global load is shared
// (eax) but the ItemDescription field is loaded twice (`test ecx,ecx`/`jbe`, then `mov edx,[eax+34h]`) -- a plain
// `def` local or a bool flag CSEs the field into one load (107v110). The entry temporary's release is the
// DecRef()/GetRefCount() pair (load/dec/store, `test edx,edx`), its dead null store dropped.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BCA24 is CWideString::NullString (?NullString@CWideString@@2V1@A, public static;
//   Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CWideString::NullString'', initializer 0x01223DA0 landed by agentFAM09); was g_emptyWideString.
// NF59 (round 7, queue q10532): moved onto engine/CTCInventoryItem.h (KINDS declares this static, the CACHEDEF opt-in the private
//   static ?CacheDef@CTCInventoryItem@@CI_NJ@Z and s_CachedItemDef), engine/CDefPointer.h and fable_string.h's CWideString (copy ctor
//   0x0099B720, NullString 0x013BCA24), engine/CIVCountedPointer.h (the clearing release) and engine/CIVCountedPointeeBase.h; the local CTCInventoryItem / CDefPointer / CWideString / CIVCountedPointer / CIVCountedPointeeBase models bound CacheDef as public (SI).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CACHEDEF
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CLOTHING_DEFS
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CTCInventoryItem.h"
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR
#include "engine/CIVCountedPointer.h"
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { public:            // NGameText::CGroupEntry
    CWideString GetText() const;                        // 0x009C7A40
};
}
using NGameText::CTextEntry;
namespace NGameText {
class CDataBank { public:                                      // NGameText::CDataBank
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
using NGameText::CDataBank;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
CWideString __fastcall CTCInventoryItem::GetDescription(long defIndex)
{
    if (CacheDef(defIndex)) {
        if (s_CachedItemDef->ItemDescription > 0) {
            CDataBank* bank = CMainGameComponent::Get()->TextBank;
            const unsigned long& textIndex = s_CachedItemDef->ItemDescription;
            return bank->GetPTextEntry(textIndex, true)->GetText();
        }
    }
    return CWideString::NullString;
}