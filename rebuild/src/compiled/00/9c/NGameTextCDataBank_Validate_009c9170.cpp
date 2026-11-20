// NGameText::CDataBank::Validate() -- walk every bank entry from index 1: for each group entry, fetch each member text entry (its record, its counted text-entry pointer, and a copy of its text); the checks themselves are compiled out of the release build
// Leaf: ?Validate@CDataBank@NGameText@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C9170, 268 bytes, thiscall. Grade: RELOCATION_MATCH (268/268). No flag, no pragma.
// PDB: Ego_r.pdb 0x00B17BC0 / FableWin.pdb 0x02F6FD20 ?Validate@CDataBank@NGameText@@QAEXXZ (public, non-const).
// Callees: CBankFile::GetEntryDataType 0x009CC210 (twice per text: the row's own type test, and GetTextInfo's, whose result is unused),
//   CDataBank::GetPGroupEntry 0x009C8890, CDataBank::GetPTextEntry 0x009C9070, CWideString copy constructor 0x0099B720 / destructor
//   0x0099B510; the counted-pointer releases expanded.
// Same-TU bodies (verbatim copies of the rows, same define block): GetText 0x009C7A40 and GetTextInfo 0x009C7D00 are expanded in the
//   row (0x009C9217 `lea edx,[ebx+28h]` + the copy constructor; 0x009C91FA the bare GetEntryDataType call); GetPTextEntry 0x009C9070 is
//   called, and with its body in the TU VC7.1 allocates as retail: `this` in ebp, the outer index in ebx (homed at [esp+18h]) and, in the
//   inner loop, the text pointer in ebx across the string calls (0x009C9213). IsLoaded 0x009C7BE0 and
//   BeginLoadingText 0x009C8630 are there because GetPTextEntry's own copy needs them. Declaration-only: GetText DIFFER(267v268),
//   GetTextInfo DIFFER(268v268), GetPTextEntry DIFFER(278v268); IsLoaded / BeginLoadingText byte-neutral for the row.
// Text oracle: Ego_d.pdb (databank.cpp, file id 432) locals this { index { group_entries { c1 { str, pentry, ptext_entry } } } }; line
//   sizes at /Od 221 / 224 / 227 / 228 / 230 / 231 / 232 / 233 / 235 = 50 / 21 / 73 / 40 / 26 / 36 / 19 / 25 / 5, this text 41 / 21 / 43 /
//   31 / 26 / 29 / 19 / 18 / 5 (Ego_d is a VC10 / Dinkumware build: vector size() and the EH-state stores of the counted-pointer and string
//   locals). Ego_d lines 223 (287 bytes) and 226 (291 bytes) are debug checks with no retail code.
// Release spelling: the TU-mate 0x009C9280's FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES + FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
//   (the group temporary: count loaded, pointer copied to ecx, decrement stored, slot-1 Delete, 0x009C91CE..0x009C91E2). The default spelling
//   is DIFFER(269v268); RELEASE_DELETES with the default pointer Release deletes twice.
// Header opt-ins (overlay, add-only): FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY (CTextEntry : CResource with its members, CDataBankTextEntry's
//   BeginLoading / BuildText, CDataBank::Validate).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
#define FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"

namespace NGameText {
// Fidelity: same-TU strong definition of 0x009C7A40 (retail TU-mate)
CWideString CTextEntry::GetText() const
{
    return Text;
}

// Fidelity: same-TU strong definition of 0x009C7BE0 (retail TU-mate)
bool CDataBankTextEntry::IsLoaded() const
{
    if (!PText.IsNull())
    {
        if (PText->PFileData.IsNull() || PText->PFileData->IsFinished())
        {
            return true;
        }
    }
    return false;
}

// Fidelity: same-TU strong definition of 0x009C7D00 (retail TU-mate)
const CDataBankTextEntry* CDataBank::GetTextInfo(unsigned long index) const
{
    if (GetEntryDataType(index) == 0)
    {
        return static_cast<const CDataBankTextEntry*>(Entries[index]);
    }
    return 0;
}

// Fidelity: same-TU strong definition of 0x009C8630 (retail TU-mate)
void CDataBank::BeginLoadingText(unsigned long entry_index) const
{
    const CDataBankTextEntry* info = GetTextInfo(entry_index);
    if (info)
    {
        info->BeginLoading(entry_index, const_cast<CDataBank&>(*this));
    }
}

// Fidelity: same-TU strong definition of 0x009C9070 (retail TU-mate)
CIVCountedPointer<const CTextEntry> CDataBank::GetPTextEntry(unsigned long index, bool wait) const
{
    const CDataBankTextEntry* text_info = GetTextInfo(index);
    if (text_info)
    {
        const_cast<CDataBank*>(this)->AsyncUpdatePending();
        if (text_info->IsLoaded() || wait)
        {
            if (text_info->PText.IsNull())
            {
                BeginLoadingText(index);
                text_info->BuildText();
                const_cast<CDataBank*>(this)->ResourceList.MakeFreeSpace(0);
            }
            else if (!text_info->PText->PFileData.IsNull())
            {
                text_info->BuildText();
            }
            text_info->PText->TouchResource();
            return text_info->PText;
        }
        BeginLoadingText(index);
    }
    return CIVCountedPointer<const CTextEntry>();
}

void CDataBank::Validate()
{
    for (long index = 1; index < Entries.size(); ++index)
    {
        if (GetEntryDataType(index) == 1)
        {
            const CArray<long>& group_entries = GetPGroupEntry(index, true)->GetTextEntries();
            for (long c1 = 0; c1 < group_entries.size(); ++c1)
            {
                const CDataBankTextEntry* ptext_entry = GetTextInfo(group_entries[c1]);
                CIVCountedPointer<const CTextEntry> pentry = GetPTextEntry(group_entries[c1], true);
                CWideString str = pentry->GetText();
            }
        }
    }
}
}