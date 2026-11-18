// NGameText::CDataBank::GetPGroupEntry(unsigned long id, bool allow_pending) const -- the text group an entry id names: flush the pending bank reads, and when the group is not loaded (and pending groups are not allowed) start loading it and return null; otherwise build it (a first load, then a resource-cache trim; or a finished read), touch it in its resource list and return a counted pointer to it
// Leaf: ?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8890, 247 bytes, thiscall (`ret 0Ch`: hidden result, id, allow_pending).
// Name: Ego_r.pdb (0x00B16130) / FableWin.pdb (0x02F6FBE0) carry the Leaf symbol; called from 0x0047EEE4, 0x0048F6BD, 0x0054FBC0, 0x00594FDB,
//   0x00595CC1, 0x005EB180, 0x00639BA0 and others (callers.py).
// agentNF69 (q10858): ported onto engine/NGameText_CDataBank.h. docs/journal/2026-11/EGOD_FINDINGS.md section 5 settles CGroupEntry: a class
//   with public base CResource at offset 0 (Ego_d LF_BCLASS; Ego_r's 28 bytes of base, Entries at +28), so the group's TouchResource (the
//   LRU list walk) is CResource's. hdr/ overlay opt-in FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY (inside FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT):
//   CGroupEntry : CResource (Entries +0x1C, PFileData +0x28, Owner +0x30); CDataBankGroupEntry's PGroup (+0) and its private const
//   BeginLoading 0x009C80F0 / BuildGroup 0x009C8700; CDataBank's second base CResourceBank at +0x164 (RTTI; ResourceList +0x168) and
//   BeginLoadingGroup 0x009C8220. Was a local model of CResource / CResourceList / CIVCountedPointer / CBankFile / CDataBank under renaming
//   macros, with the made-up callee spellings FlushAsyncUpdates / QueueGroupLoad and public non-const BeginLoading / BuildGroup.
// Callees (both PDBs' spellings): CBankFile::GetEntryDataType 0x009CC210, CBankFileAsync::AsyncUpdatePending 0x009D68B0,
//   CBankFileAsyncData::IsFinished 0x009D54E0, CDataBank::BeginLoadingGroup 0x009C8220, CDataBankGroupEntry::BeginLoading 0x009C80F0 /
//   BuildGroup 0x009C8700, CResourceList::MakeFreeSpace 0x009FC4F0, CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90.
// Shape: BeginLoadingGroup is defined here as in its source file (GetGroupInfo, then the entry's BeginLoading with the bank): VC7.1 calls it
//   out of line on the not-loaded path and expands it on the first-load path, as retail does.
//   That helper body, graded on its own (q.py 0x009C8220, leaf ?BeginLoadingGroup@CDataBank@NGameText@@QBEXK@Z), is RELOCATION_MATCH too. CResource::TouchResource (FableWin.pdb, inline)
//   is engine/CResourceList.h's forced-inline body; the result is CIVCountedPointer's converting constructor (AddRef `inc [edi+4]`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_MAKEFREESPACE
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"

namespace NGameText {
void CDataBank::BeginLoadingGroup(unsigned long id) const
{
    const CDataBankGroupEntry* entry = GetGroupInfo(id);
    if (entry)
        entry->BeginLoading(id, const_cast<CDataBank&>(*this));
}

CIVCountedPointer<const CGroupEntry> CDataBank::GetPGroupEntry(unsigned long id, bool allow_pending) const
{
    const CDataBankGroupEntry* entry = GetGroupInfo(id);
    if (entry) {
        const_cast<CDataBank*>(this)->AsyncUpdatePending();
        bool loaded = !entry->PGroup.IsNull() && (entry->PGroup->PFileData.IsNull() || entry->PGroup->PFileData->IsFinished());
        if (!loaded && !allow_pending) {
            BeginLoadingGroup(id);
            return CIVCountedPointer<const CGroupEntry>();
        }
        if (entry->PGroup.IsNull()) {
            BeginLoadingGroup(id);
            entry->BuildGroup();
            const_cast<CDataBank*>(this)->ResourceList.MakeFreeSpace(0);
        }
        else if (!entry->PGroup->PFileData.IsNull())
            entry->BuildGroup();
        entry->PGroup->TouchResource();
        return entry->PGroup;
    }
    return CIVCountedPointer<const CGroupEntry>();
}
}
