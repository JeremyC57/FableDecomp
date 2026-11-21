// CBankFile::WriteFileInformation(bool) -- make the bank writable, rewrite the "BBBB" header at offset 0 (bank format version, data-format version, 1 or the random validation value, alignment, highest live index, live entry count, aligned end of entry data), record the header size, pad it to the alignment, then write the entry list after the data (is_valid) or, outside retail mode, the recovery file
// Leaf: ?WriteFileInformation@CBankFile@@AAEX_N@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE370, 620 bytes, thiscall, `ret 4`; /GS (cookie 0x0139C8A8, check 0x00BFE9F9; char header[5]
//   from "BBBB" 0x0129B5A4). lib_bank_file.obj.
// Identity: Ego_r.pdb / FableWin.pdb (0x02F9ACC0) / Ego_d.pdb ?WriteFileInformation@CBankFile@@AAEX_N@Z (private). Ego_d.pdb (debug length
//   887): parameter is_valid; locals info_offset, data_version, max_index, rand_val, entry_count (frame -16..-32: declared in that order,
//   before os -48 and header -56), os (CFileDataOutputStream), header, and c_entry in the loop.
// Callees: CFileDataOutputStream ctor 0x00994400 / SetPosition 0x00994450 / Write 0x00994480 / GetPosition 0x00994460 / dtor 0x00994430,
//   CDataOutputStream::WriteULONG 0x00993EA0, CPackedUIntArray::operator[] 0x00A629F0, GetRandomValidationValue 0x009CC0B0, the virtual
//   GetDataFormatVersion (slot 8, pure in CBankFile), WritePadding 0x009CCB20, WriteRecoveryFile 0x009CDF50, InternalWriteEntryList
//   0x009CDA70; MakeWritable (0x009CBF90), IsEntryInitialised, GetEntryDataOffset / GetEntryDataSize (0x009CC270 / 0x009CC240) and Align
//   (0x009CBB90) expanded.
// Ego_d lines (VC10 /Od): 1673 (8 B) MakeWritable(); 1674 (30 B) the stream on *BankFile (operator* + ctor + EH state); 1675 (10 B)
//   SetPosition(0); 1676 (17 B) header = "BBBB"; 1677 (14 B) Write(header, 4); 1679 / 1680 (7 B each) max_index = 0, entry_count = 0;
//   1681 (12 B) info_offset = HeaderSize; 1682 (40 B) the for loop over UpdateData.size(); 1684 (19 B) if (IsEntryInitialised(c_entry));
//   1686 (9 B) entry_count++; 1687 (47 B) info_offset = std::max(info_offset, offset + size) (two calls, the sum temporary, the fastcall
//   max<unsigned long> by reference); 1688 (14 B) the max_index test and store on one line; 1690 (2 B) the loop jump; 1691 (15 B)
//   info_offset = Align(info_offset); 1693 (11 B) rand_val; 1695 (16 B) data_version (virtual call); 1697..1703 the seven WriteULONGs
//   (35 B for the is_valid ternary); 1704 (17 B) HeaderSize = os.GetPosition(); 1705 (36 B) WritePadding(Align(HeaderSize) - HeaderSize);
//   1707 (8 B) if (!is_valid); 1709 (34 B) `if (!RetailMode) WriteRecoveryFile(...)` on one line (14 + 20 B); 1712 (2 B) else; 1714 /
//   1715 (12 B each) SetPosition(info_offset), InternalWriteEntryList(os).
// Inferred: Ego_d lines 1672 (282 B) and 1710 (17 B) have no release bytes (debug-only checks; 1672 is the size of the other bank-file
//   asserts, e.g. GetEntryDataOffset line 329, 274 B); they are left out.
// Inferred: the order of the two zero stores (max_index first, as retail stores [esp+18h] before [esp+14h] at 0x009CE410) and the
//   comparison spelling `max_index < c_entry` (retail `cmp [esp+18h],edi ; jae`, 0x009CE499); /Od sizes are the same either way.
// Header: FABLE_RETAIL_CBANKFILE_WRITEFILEINFORMATION (new, add-only) in engine/CBankFile.h: the Ego_r member types of BankFile /
//   Filename, inline MakeWritable / IsEntryInitialised / Align, and the private writer declarations.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#define FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE
#define FABLE_RETAIL_CBANKFILE_WRITEFILEINFORMATION
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CDataOutputStream.h"
#include "engine/CFileDataOutputStream.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "engine/CBankFile.h"
void CBankFile::WriteFileInformation(bool is_valid)
{
    unsigned long info_offset;
    unsigned long data_version;
    unsigned long max_index;
    unsigned long rand_val;
    unsigned long entry_count;
    MakeWritable();
    CFileDataOutputStream os(*BankFile);
    os.SetPosition(0);
    char header[] = "BBBB";
    os.Write(header, 4);
    max_index = 0;
    entry_count = 0;
    info_offset = HeaderSize;
    for (unsigned long c_entry = 0; c_entry < UpdateData.size(); c_entry++) {
        if (IsEntryInitialised(c_entry)) {
            entry_count++;
            info_offset = std::max(info_offset, GetEntryDataOffset(c_entry) + GetEntryDataSize(c_entry));
            if (max_index < c_entry) max_index = c_entry;
        }
    }
    info_offset = Align(info_offset);
    rand_val = GetRandomValidationValue();
    data_version = GetDataFormatVersion();
    os.WriteULONG(BankFormatVersion);
    os.WriteULONG(data_version);
    os.WriteULONG(is_valid ? 1 : rand_val);
    os.WriteULONG(Alignment);
    os.WriteULONG(max_index);
    os.WriteULONG(entry_count);
    os.WriteULONG(info_offset);
    HeaderSize = os.GetPosition();
    WritePadding(Align(HeaderSize) - HeaderSize);
    if (!is_valid) {
        if (!RetailMode) WriteRecoveryFile(rand_val, max_index, entry_count);
    } else {
        os.SetPosition(info_offset);
        InternalWriteEntryList(os);
    }
}