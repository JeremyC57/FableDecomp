// Behaviour test for 00765f90: the row body verbatim over fixture models of the shared-header interfaces it calls.
// CTCObjectAugmentations::GetAugmentationName(long defIndex) -- the display name of an augmentation def (its Name text from the game text bank), or "Missing augmentation name!" when the def has no text
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixture models of the shared-header interfaces: a counted def (Refs counts references, Delete counted), the counted CDefPointer
// handle (copy / assignment add-ref, release on destruction), the definition manager's GetDef<T> (fills the handle with an add-ref,
// returns whether found) and CDefClassBase::GetSubDef<T> (the sub-def table of the def, by a type slot of T).
class CDefPointeeBase { public:
    long Refs; int Deleted; CDefPointeeBase* Sub[8];
    CDefPointeeBase() : Refs(1), Deleted(0) { memset(Sub, 0, sizeof(Sub)); }
    void Delete() { ++Deleted; }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { Release(); }
    CDefPointer& operator=(const CDefPointer& o) { if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->Refs; } return *this; }
    void Release() { if (Object) { if (--Object->Refs == 0) Object->Delete(); Object = 0; } }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    T* Get() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Set(CDefPointeeBase* p) { if (p) ++p->Refs; Release(); Object = p; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const
    {
        CDefPointeeBase* p = Sub[T::Slot];
        if (!p) return false;
        out->Set(p); return true;
    }
};
class CGameDefinitionManager { public:
    CDefPointeeBase* Table[16]; mutable long Calls; mutable long LastIndex;
    static CGameDefinitionManager& Get();
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        ++Calls; LastIndex = index;
        if (index > 0 && index < 16 && Table[index]) { out->Set(Table[index]); return true; }
        return false;
    }
};
static CGameDefinitionManager g_manager;
CGameDefinitionManager& CGameDefinitionManager::Get() { return g_manager; }
static void ResetManager() { memset(&g_manager, 0, sizeof(g_manager)); }
class CThingBaseDef : public CDefClassBase { public: enum { Slot = 0 }; };
#ifndef _MSC_VER
#define __fastcall
#endif
class CAugmentationDef : public CDefClassBase { public: enum { Slot = 1 }; unsigned long Name; float DamageMultiplier; };
class CWideString { public: unsigned long Id; CWideString(unsigned long id) : Id(id) { } };
static int g_entryReleases = 0;
namespace NGameText {
class CTextEntry { public: unsigned long Id; CWideString GetText() const { return CWideString(Id); } };
template <class T> class CEntryPointer { public:
    T* Object; CEntryPointer(T* o) : Object(o) { } ~CEntryPointer() { ++g_entryReleases; }
    T* operator->() const { return Object; }
};
class CDataBank { public:
    mutable CTextEntry Entry; mutable bool LastLoad;
    CEntryPointer<const CTextEntry> GetPTextEntry(unsigned long id, bool load) const { Entry.Id = id * 10; LastLoad = load; return CEntryPointer<const CTextEntry>(&Entry); }
};
}
static NGameText::CDataBank g_bank;
class CMainGameComponent { public:
    static CMainGameComponent* __fastcall Get() { static CMainGameComponent c; return &c; }
    const NGameText::CDataBank& GetTextBank() const { return g_bank; }
};
namespace { CWideString MISSING_AUGMENTATION_TEXT(999); }
class CTCObjectAugmentations { public:
    static CWideString __fastcall GetAugmentationName(long augmentation_def_index);
};
// ---- row body (verbatim) ----
namespace { extern CWideString MISSING_AUGMENTATION_TEXT; }       // 0x013BAD98 L"Missing augmentation name!"

CWideString __fastcall CTCObjectAugmentations::GetAugmentationName(long augmentation_def_index)
{
    CDefPointer<const CThingBaseDef> def;
    CGameDefinitionManager::Get().GetDef(augmentation_def_index, &def);
    CDefPointer<const CAugmentationDef> augmentation;
    def->GetSubDef(&augmentation);
    if (augmentation->Name > 0)
        return CMainGameComponent::Get()->GetTextBank().GetPTextEntry(augmentation->Name, true)->GetText();
    return MISSING_AUGMENTATION_TEXT;
}
// ---- harness ----
int main()
{
    ResetManager();
    CThingBaseDef named, unnamed; CAugmentationDef an, au; an.Name = 7; au.Name = 0; named.Sub[1] = &an; unnamed.Sub[1] = &au;
    g_manager.Table[3] = &named; g_manager.Table[4] = &unnamed;
    CWideString a = CTCObjectAugmentations::GetAugmentationName(3);
    CHECK(a.Id == 70 && g_bank.LastLoad && g_entryReleases == 1 && g_manager.LastIndex == 3);
    CWideString b = CTCObjectAugmentations::GetAugmentationName(4);
    CHECK(b.Id == 999 && g_entryReleases == 1);
    CHECK(named.Refs == 1 && unnamed.Refs == 1 && an.Refs == 1 && au.Refs == 1);
    if (fails == 0) printf("00765f90_TEST PASS\n");
    return fails;
}
