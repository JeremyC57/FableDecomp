// Standalone C++03 fixture for the recovered NPC name lookup.
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <vector>

typedef std::wstring CWideString;

template<class T> struct CIVCountedPointer {
    T* Object;
    explicit CIVCountedPointer(T* object) : Object(object) {}
    T* operator->() const { return Object; }
};

namespace NGameText {
struct CTextEntry {
    CWideString Value;
    explicit CTextEntry(const wchar_t* value) : Value(value) {}
    CWideString GetText() const { return Value; }
};
struct CGroupEntry { std::vector<unsigned long> Entries; };
struct CDataBank {
    CTextEntry Default, GroupA, GroupB;
    CGroupEntry Group;
    mutable int TextQueries, GroupQueries;

    CDataBank() : Default(L"Stewart"), GroupA(L"Norris"), GroupB(L"Robin"), TextQueries(0), GroupQueries(0) {
        Group.Entries.push_back(3);
        Group.Entries.push_back(4);
    }

    bool EntryExists(unsigned long tag) const { return tag == 1 || tag == 2; }
    unsigned long GetEntryDataType(unsigned long tag) const { return tag == 1 ? 0 : 1; }

    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long tag, bool) const {
        ++TextQueries;
        const CTextEntry* value = &Default;
        if (tag == 3) value = &GroupA;
        if (tag == 4) value = &GroupB;
        return CIVCountedPointer<const CTextEntry>(value);
    }

    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long, bool) const {
        ++GroupQueries;
        return CIVCountedPointer<const CGroupEntry>(&Group);
    }
};
}

struct CMainGameComponent {
    NGameText::CDataBank Bank;
    static CMainGameComponent* Get() {
        static CMainGameComponent component;
        return &component;
    }
    const NGameText::CDataBank& GetTextBank() const { return Bank; }
};

struct CThingManager {
    unsigned long Key;
    CWideString Value;
    int Writes;
    CThingManager() : Key(0), Writes(0) {}
    bool DoesNPCNameEntryExistForThing(unsigned long key) const { return key == Key; }
    CWideString GetNameForThing_OnlyCalledFromTCHasName(unsigned long key) const {
        return key == Key ? Value : CWideString();
    }
    void SetNameForThing(unsigned long key, const CWideString& value) {
        Key = key;
        Value = value;
        ++Writes;
    }
};

struct CThing {
    unsigned long UID;
    unsigned long GetUniqueID() const { return UID; }
};

struct CHasNameDef { unsigned long DefaultNameTag; };

struct CTCHasName {
    CThing* ParentThing;
    CHasNameDef* PDef;
    CThingManager* Manager;
    CThingManager& GetThingManager() const { return *Manager; }
    CWideString GetNPCShortName();
};

// Recovered method body, with game dependencies supplied by the fixture.
CWideString CTCHasName::GetNPCShortName()
{
    if (GetThingManager().DoesNPCNameEntryExistForThing(ParentThing->GetUniqueID())) {
        CWideString name = GetThingManager().GetNameForThing_OnlyCalledFromTCHasName(ParentThing->GetUniqueID());
        return name;
    } else {
        const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
        CWideString default_name_converted;
        if (text_bank.EntryExists(PDef->DefaultNameTag)) {
            if (text_bank.GetEntryDataType(PDef->DefaultNameTag) == 0) {
                CIVCountedPointer<const NGameText::CTextEntry> ptext_entry = text_bank.GetPTextEntry(PDef->DefaultNameTag, true);
                default_name_converted = ptext_entry->GetText();
            } else {
                CIVCountedPointer<const NGameText::CGroupEntry> pgroup_entry = text_bank.GetPGroupEntry(PDef->DefaultNameTag, true);
                default_name_converted = text_bank.GetPTextEntry(pgroup_entry->Entries[rand() % pgroup_entry->Entries.size()], true)->GetText();
            }
            GetThingManager().SetNameForThing(ParentThing->GetUniqueID(), default_name_converted);
        }
        return default_name_converted;
    }
}

int main()
{
    CThing thing = { 42 };
    CHasNameDef def = { 0 };
    CThingManager manager;
    CTCHasName owner = { &thing, &def, &manager };
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();

    bool ok = owner.GetNPCShortName().empty() && manager.Writes == 0 && bank.TextQueries == 0;
    def.DefaultNameTag = 1;
    ok = ok && owner.GetNPCShortName() == L"Stewart" && manager.Writes == 1 && bank.TextQueries == 1;
    def.DefaultNameTag = 2;
    ok = ok && owner.GetNPCShortName() == L"Stewart" && manager.Writes == 1 && bank.GroupQueries == 0;
    thing.UID = 43;
    const CWideString chosen = owner.GetNPCShortName();
    ok = ok && (chosen == L"Norris" || chosen == L"Robin") && manager.Writes == 2 && manager.Key == 43;
    ok = ok && owner.GetNPCShortName() == chosen && bank.TextQueries == 2 && bank.GroupQueries == 1;
    std::puts(ok ? "007f4990_TEST PASS" : "007f4990_TEST FAIL");
    return ok ? 0 : 1;
}