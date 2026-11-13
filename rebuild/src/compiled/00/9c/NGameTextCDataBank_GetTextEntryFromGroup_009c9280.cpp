// NGameText::CDataBank::GetTextEntryFromGroup(unsigned long groupId, const CCharString& narrator, bool& found, long* lastId,
//   bool allowPending) const -- collect the text ids of group `groupId` whose narrator is `narrator` (or `narrator` + "_1_5"),
//   pick one at random (re-rolling while it equals *lastId), and return its text entry; when the pick is not loaded and pending
//   text is not allowed, start loading it and return the first loaded candidate after it, else null. `found` reports whether the
//   group existed with matching entries (or was still pending).
// Leaf: ?GetTextEntryFromGroup@CDataBank@NGameText@@QBEPBVCTextEntry@2@KABVCCharString@@AA_NPAJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C9280, length 861 bytes, `ret 14h`, no frame pointer, no EH frame.
// Signature: Ego_r.pdb + FableWin.pdb ?GetTextEntryFromGroup@CDataBank@NGameText@@QBEPBVCTextEntry@2@KABVCCharString@@AA_NPAJ_N@Z.
// Callees: CBankFile::GetEntryDataType 0x009CC210, CDataBank::GetPGroupEntry 0x009C8890 (hidden result), operator+(const
//   CCharString&, const char*) 0x0099F600 (__fastcall, "_1_5" 0x012783C8), CCharString::~CCharString 0x0099EAE0,
//   CBasicString<char>::Compare 0x00411570 (__fastcall), vector<long>::push_back 0x005178A0 (folded COMDAT), GFROR13 0x00497890,
//   CBankFileAsync::AsyncUpdatePending 0x009D68B0, CDataBank::GetTextInfo 0x009C7D00, CDataBankTextEntry::IsLoaded 0x009C7BE0,
//   CBankFileAsyncData::IsFinished 0x009D54E0, CDataBank::BeginLoadingText 0x009C8630, CDataBank::GetPTextEntry 0x009C9070
//   (hidden result), free 0x00BFEA14.
// Shape (from the disassembly): the whole body sits under one `if` so every failure shares the epilogue at 0x009C9568; the group
//   pointer is assigned (`group = GetPGroupEntry(..)`: the inline CIVCountedPointer operator=, conditional store + AddRef, then
//   the temporary's release with its zero store) and dies at the end of the collecting block (bare `dec/jne/Delete`); the loop
//   reads the group's ids through a reference (GetTextEntries, inline; named only by FableWin.pdb) so the ids' start load is shared between the
//   size test and the body; the per-id text info lookup in the loop and the first pick's loaded test are written out where
//   retail expands them (GetTextInfo / IsLoaded are CALLED at the other sites); the narrator index is a named local and both
//   compares index the narrator table with it (Compare's body is visible, so the table base survives the calls); the two
//   successful returns are tail-merged by the compiler (the first pick's loaded test is a named `bool loaded`; with the test inline in the `if` they are allocated apart).
// Headers: engine/NGameText_CDataBank.h + engine/CBankFileAsync.h (FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT: members at their
//   retail offsets, CDataBankTextEntry, GetGroupInfo, GetTextEntries, AsyncUpdatePending), engine/CIVCountedPointer.h
//   (FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE), engine/GFRandom.h (FABLE_RETAIL_GFRANDOM_RESULT_LOCAL), fable_string.h
//   (FABLE_RETAIL_CBASICSTRING_COMPARE_BODY; FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ: retail has no CCharString::operator== call
//   here, it calls CBasicString<char>::Compare 0x00411570 directly at 0x009C93AD and 0x009C93F4, so operator== is expanded inline).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_GFRANDOM_RESULT_LOCAL
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#include "fable_string.h"
#include "engine/GFRandom.h"
#include "engine/NGameText_CDataBank.h"
namespace NGameText {
const CTextEntry* CDataBank::GetTextEntryFromGroup(unsigned long groupId, const CCharString& narrator, bool& found,
                                                   long* lastId, bool allowPending) const
{
    found = false;
    if (groupId > 0 && GetEntryDataType(groupId) == 1) {
        CArray<long> ids;
        {
            CIVCountedPointer<const CGroupEntry> group;
            if (GetGroupInfo(groupId) == 0)
                return 0;
            group = GetPGroupEntry(groupId, allowPending);
            if (group.IsNull()) {
                found = true;
                return 0;
            }
            const CArray<long>& entries = group->GetTextEntries();
            for (unsigned long i = 0; i < entries.size(); ++i) {
                long id = entries[i];
                const CDataBankTextEntry* info = GetEntryDataType(id) == 0 ? static_cast<const CDataBankTextEntry*>(Entries[id]) : 0;
                long narratorIndex = info->GetNarratorIndex();
                if (Narrators[narratorIndex] == narrator || Narrators[narratorIndex] == narrator + "_1_5")
                    ids.push_back(id);
            }
        }
        if (!ids.empty()) {
            found = true;
            long count = ids.size();
            long id = ids[0];
            unsigned long pick = 0;
            if (count > 1) {
                do {
                    pick = GFRandom(count, &Seed);
                    id = ids[pick];
                } while (lastId && id == *lastId);
            }
            const_cast<CDataBank*>(this)->AsyncUpdatePending();
            const CDataBankTextEntry* info = GetTextInfo(id);
            bool loaded = !info->PText.IsNull() && (info->PText->PFileData.IsNull() || info->PText->PFileData->IsFinished());
            if (loaded || allowPending) {
                if (lastId)
                    *lastId = id;
                return GetPTextEntry(id, true).operator->();
            }
            BeginLoadingText(id);
            long index = pick;
            for (long tries = 0; tries < count; ++tries) {
                long other = ids[index];
                const_cast<CDataBank*>(this)->AsyncUpdatePending();
                if (GetTextInfo(other)->IsLoaded()) {
                    if (lastId)
                        *lastId = other;
                    return GetPTextEntry(other, true).operator->();
                }
                index = (index + 1) % count;
            }
        }
    }
    return 0;
}
}