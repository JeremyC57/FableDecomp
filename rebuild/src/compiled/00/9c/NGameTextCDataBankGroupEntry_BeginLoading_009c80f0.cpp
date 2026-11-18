// NGameText::CDataBankGroupEntry::BeginLoading(unsigned long id, CDataBank& bank) const -- when the group is not loaded, start the asynchronous
// read of the whole bank entry, create the group resource over the read and add it to the bank's resource bank (then trim the resource
// cache); touch the group in its resource list
// Leaf: ?BeginLoading@CDataBankGroupEntry@NGameText@@ABEXKAAVCDataBank@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C80F0, 296 bytes.
// Callees: CBankFileAsync::GetAsyncEntry 0x009D58D0, CBankFile::GetEntryDataSize 0x009CC240, CBankFileAsyncEntry::BeginReadDataAutoAlloc
//   0x009D6100, operator new 0x00BFEA1A, CGroupEntry ctor 0x009C8060, CResourceBank::AddResource 0x009FC380, CResourceList::MakeFreeSpace
//   0x009FC4F0, CResource::TouchResource (inline), operator delete 0x00BFE9BC (count blocks).
// NF72 (round 7, q10906 / q10881 / q10883): Ego_r.pdb 0x00B15DC0 + FableWin.pdb 0x02F75330 ?BeginLoading@CDataBankGroupEntry@NGameText@@ABEXKAAVCDataBank@2@@Z
//   (private const, CDataBank&), declared by engine/NGameText_CDataBank.h GETPGROUPENTRY (NF69) and bound by the ported BeginLoadingGroup /
//   GetPGroupEntry. Ported off the renaming-macro CDataBank / CIVCountedPointer / CResource / CResourceList models and the local counted-pointer,
//   bank and group models onto that header (with the 0x009C8890 macro block): CDataBank : CBankFileAsync, CResourceBank; CGroupEntry : CResource
//   (overlay opt-in NGAMETEXT_CGROUPENTRY_CTOR: the Ego_r / FableWin ctor 0x009C8060). The calls now bind the PDB spellings: GetAsyncEntry
//   (CBankFileAsync, GETASYNCENTRY) and ?BeginReadDataAutoAlloc@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KKW4EThreadedFilePriority@@@Z
//   (Ego_r.pdb 0x00B2A240; CBankFileAsyncEntry has no shared header). PGroup is written through const_cast (the member is not mutable in the header).
//   gDataBankReadPriority (0x01396314) is the landed descriptive name (no PDB data symbol found), now typed EThreadedFilePriority.
//   Without 0x009C8890's FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES: Reset releases with one slot-1 call, as retail does here.
#define FABLE_RETAIL_DEF_CLASS_BASE
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
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_NGAMETEXT_CGROUPENTRY_CTOR
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/EThreadedFilePriority.h"

class CBankFileAsyncEntry {
public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
extern EThreadedFilePriority gDataBankReadPriority;        // 0x01396314 (landed descriptive name)

namespace NGameText {
void CDataBankGroupEntry::BeginLoading(unsigned long id, CDataBank& bank) const
{
    if (PGroup.IsNull()) {
        CCountedPointer<CBankFileAsyncEntry> entry = bank.GetAsyncEntry(id);
        CCountedPointer<CBankFileAsyncData> data = entry->BeginReadDataAutoAlloc(0, bank.GetEntryDataSize(id), gDataBankReadPriority);
        const_cast<CDataBankGroupEntry*>(this)->PGroup.Reset(new CGroupEntry(data, this));
        bank.AddResource(*PGroup);
        bank.ResourceList.MakeFreeSpace(0);
    }
    PGroup->TouchResource();
}
}
