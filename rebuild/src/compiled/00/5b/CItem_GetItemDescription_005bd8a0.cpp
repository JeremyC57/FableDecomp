// CItem::GetItemDescription() const -- the wide description text of a non-empty slot's item (the inventory def's text index at +0x34, looked up in the game text bank); the empty wide string for an empty slot or a def without text
// Leaf: GetItemDescription
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD8A0, length 145 bytes.
// Callees: CItem::GetInventoryItemDef @ 0x005BD82E (counted def by value), NGameText::CDataBank::GetPGroupEntry(unsigned long,
// bool) @ 0x009C9070 on the bank at CMainGameComponent::Get()(0x013B86A0) +0x14 (CIVCountedPointer<const CGroupEntry> by value),
// CGroupEntry's text getter @ 0x009C7A40 (`return m_text;` at +0x28; manifest label "CBankFile::GetBankHandle" is
// propagated), CWideString::CWideString(const CWideString&) @ 0x0099B720 on the empty wide string 0x013BCA24.
// Model: engine/CItem.h; the counted temporaries and their inlined release (`mov ecx,eax ; add eax,4 ; dec [eax] ;
// cmp [eax],0`) exactly as the landed CItem::GetMaximumNumberOfItems 0x005BD931; both temporaries share the frame word
// [ebp-4]; the text bank is read into a local before the argument pushes. Pragmas: the CItem module's frame-pointer size TU.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BCA24 is CWideString::NullString (?NullString@CWideString@@2V1@A, public static;
//   Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CWideString::NullString'', initializer 0x01223DA0 landed by agentFAM09); was g_emptyWideString.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
public:
    static CWideString NullString;                  // 0x013BCA24 ?NullString@CWideString@@2V1@A
};
struct CIVCountedPointeeVirtual {
    virtual void Destroy(int flags) const;               // slot 0
    virtual void Delete() const;                         // slot 1
};
struct CIVRefCounted {                             // second base: the count at +4 behind the vptr
    mutable unsigned long IVRefCount;
    FABLE_FORCEINLINE void DecRef() const { --IVRefCount; }
    FABLE_FORCEINLINE unsigned long GetRefCount() const { return IVRefCount; }
};
class CIVCountedPointeeBase : public CIVCountedPointeeVirtual, public CIVRefCounted { public:
    FABLE_FORCEINLINE void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer { public:      // 4 bytes
    T* Object;
    FABLE_FORCEINLINE ~CIVCountedPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
struct CInventoryItemDef_Retail : CIVCountedPointeeBase {
    unsigned char _pad[0x34 - 8];
    long          DescriptionTextIndex;            // +0x34
};
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { public:       // NGameText::CGroupEntry
    CWideString GetText() const;                   // 0x009C7A40
};
}
using NGameText::CTextEntry;
namespace NGameText {
class CDataBank { public:                                 // NGameText::CDataBank
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
    CDataBank*    TextBank;                        // +0x14
};
struct CItem_Methods : CItem {
    CIVCountedPointer<CInventoryItemDef_Retail> GetInventoryItemDef() const;   // 0x005BD82E
    CWideString GetItemDescription() const;
};

CWideString CItem_Methods::GetItemDescription() const
{
    if (NumberOfItems == 0)
        return CWideString::NullString;
    long textIndex = GetInventoryItemDef().Object->DescriptionTextIndex;
    if (textIndex > 0) {
        CDataBank* bank = CMainGameComponent::Get()->TextBank;
        return bank->GetPTextEntry(textIndex, true).Object->GetText();
    }
    return CWideString::NullString;
}