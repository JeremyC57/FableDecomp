// CEngineWorldMap::LoadWaterData() -- when the static-map bank holds the water entry: copy the entry's header record into a buffer, open the entry asynchronously and read its whole data block into a second buffer, wrap both in memory streams and hand them (data, header) to the water renderer's non-patch data loader
// Leaf: ?LoadWaterData@CEngineWorldMap@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B41FA0, length 336 bytes (body 0x14b), plain `ret`.
// NF76 port (2026-11-17): on the shared headers with the sibling 0x00B42750's macro block (engine/CEngineWorldMap.h STATICMAPFILE,
//   CStaticMapBankFile.h CLASS on CBankFileAsync.h / CBankFile.h) plus CBankFile ADDENTRYMULTIPLEFILES (+ FILENAMEINDICES, the
//   0x009CE5E0 set), CBankFileAsync GETASYNCENTRY, CBankFile ENTRYDATASIZE, CMemoryDataInputStream / CDataInputStream retail branches,
//   CArray with STLP_VECTOR_N_OUTOFLINE (the 0x00BEAE50 set), and my overlay opt-ins: CENGINEWORLDMAP_LOADWATERDATA (the private
//   self), CBANKFILE_PEEKENTRYUPDATEDATA (FableWin.pdb inline), CBANKFILEENTRYUPDATEDATA_READINFO (GetInfoSize inline + ReadInfo),
//   CENGINEWATERRENDERER_LOADNONPATCHDATA_DECL. The local CCharString / CByteBuffer / CDataBuffer / CBankFileEntryUpdateData /
//   CMemoryDataInputStream / CBankFile / CBankFileAsync / CStaticMapBankFile / CEngineWaterRenderer / CEngineWorldMap models are gone;
//   CBankFileAsyncEntry has no shared header and stays a local model (Ego_r.pdb members BankFile +4 / Index +8, FableWin.pdb inline
//   GetDataSize, Ego_r.pdb ReadDataNonAsync 0x009D52B0: its body calls CBankFile::ReadEntryData(Index, offset, buffer, size)).
//   0x009D58D0 binds the queued ?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z (q10881).
//   Locals as Ego_d.pdb names them (smap_index, entry, info_size, info_buffer, info_stream, async_file_entry, size, dest, stream).
//   0x01436E54 keeps the landed descriptive g_pEngineWaterRenderer (no PDB data symbol found; as 0x00B42750).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE
#define FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS
#define FABLE_RETAIL_CENGINEWORLDMAP_LOADWATERDATA
#define FABLE_RETAIL_CENGINEWATERRENDERER_LOADNONPATCHDATA_DECL
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_PEEKENTRYUPDATEDATA
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_READINFO
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineWorldMap.h"

class CBankFileAsyncEntry {                       // no shared header (Ego_r.pdb: vptr, BankFile +4, Index +8, PaddedSize +0xC)
public:
    virtual ~CBankFileAsyncEntry();
    unsigned long GetDataSize() const { return BankFile->GetEntryDataSize(Index); }   // FableWin.pdb ?GetDataSize@CBankFileAsyncEntry@@QBEKXZ (inline)
    void ReadDataNonAsync(unsigned long offset, void* buffer, unsigned long size);   // 0x009D52B0 (Ego_r.pdb)
private:
    CBankFileAsync* BankFile;                     // +0x04
    unsigned long Index;                          // +0x08
    unsigned long PaddedSize;                     // +0x0C
};
extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54
extern CCharString WaterStaticMapBankFileName;         // 0x01436EC8 (Ego_r.pdb `dynamic initializer for 'WaterStaticMapBankFileName'`)

void CEngineWorldMap::LoadWaterData()
{
    unsigned long smap_index = StaticMapFile.FindIndexBySymbol(WaterStaticMapBankFileName);
    if (smap_index) {
        const CBankFileEntryUpdateData* entry = StaticMapFile.PeekEntryUpdateData(smap_index);
        unsigned long info_size = entry->GetInfoSize();
        CArray<char> info_buffer(info_size);
        entry->ReadInfo(0, info_buffer.GetAsCArray(), info_size);
        CMemoryDataInputStream info_stream(info_buffer.GetAsCArray(), info_size);
        CCountedPointer<CBankFileAsyncEntry> async_file_entry = StaticMapFile.GetAsyncEntry(smap_index);
        long size = async_file_entry->GetDataSize();
        CArray<unsigned char> dest(size);
        async_file_entry->ReadDataNonAsync(0, dest.GetAsCArray(), size);
        CMemoryDataInputStream stream(dest.GetAsCArray(), size);
        g_pEngineWaterRenderer->LoadNonPatchData(stream, info_stream);
    }
}
