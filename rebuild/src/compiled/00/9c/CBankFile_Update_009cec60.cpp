// CBankFile::Update
// Leaf: ?Update@CBankFile@@UAEXXZ
// Flags: default
#define FABLE_RETAIL_CBANKFILE_UPDATE_PASS27
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#define FABLE_RETAIL_CBANKFILE_WRITEFILEINFORMATION
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#define FABLE_RETAIL_CBANKFILE_SYMBOLINDICES
#define FABLE_RETAIL_CBANKFILE_GETSIZE_PASS26
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCRC_BANK_STATEMENTS_PASS27
#define FABLE_RETAIL_STLP_MAP
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CBankFileEntryUpdateData.h"
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "engine/CBankFile.h"
#include "engine/CBankCreatorException.h"
#include "engine/CCRC.h"

// Set by CBankFile::AbortBankCreation; retail storage at 0x013CA821.
extern bool g_BankCreationAborted;

void CBankFile::Update()
{
    CWideString drive, folder, file, ext;
    CAFile::SplitPath(Filename, &drive, &folder, &file, &ext);
    ChecksumCache.Open(drive + folder + file + L".cks");
    SetBankUpdateStatus("Scanning files...");
    unsigned long entry_count = CountEntries();
    SetBankUpdateProgress(0, entry_count);
    unsigned long total_files_to_update = 0;
    unsigned long scanned_count = 0;

    for (unsigned long index = 0; index < (unsigned long)GetSize(); ++index) {
        if (RuntimeData[index].Valid) {
            CCharString symbol = GetEntrySymbol(index);
            CBankFileEntryUpdateData* update_data = UpdateData[index];
            if (!update_data->Exists) {
                DeleteEntry(index);
                continue;
            }

            unsigned long data_type = GetEntryDataType(index);
            if (!update_data->RequiresUpdate) {
                unsigned long checksum = 0;
                for (unsigned long c_file = 0; c_file < update_data->GetFileCount(); ++c_file) {
                    if (IsDiskFile(data_type, c_file)) {
                        CCharString filename = update_data->GetFilename(c_file);
                        unsigned long file_checksum = ChecksumCache.GetChecksum(filename.ToWideString());
                        checksum = CCRC::Calc(checksum, &file_checksum, sizeof(file_checksum));
                    }
                }
                if (checksum != GetEntryChecksum(index)) {
                    WriteBankLog(CCharString("Source file has been modified for ") + GetEntrySymbol(index));
                    update_data->RequiresUpdate = true;
                }
            }

            if (update_data->RequiresUpdate)
                ++total_files_to_update;
            ++scanned_count;
            SetBankUpdateProgress(scanned_count, entry_count);
            if (g_BankCreationAborted)
                throw CBankCreatorAbortException();
        }
    }

    if (FileValid && ChangeCount == 0 && total_files_to_update == 0 && GetSize() > 0) {
        ChecksumCache.Close();
        return;
    }

    total_files_to_update += AdjustUpdateEntries();
    BeginUpdateEntries();
    SetBankUpdateProgress(0, total_files_to_update);
    unsigned long updated_count = 0;
    unsigned long updated_since_recovery = 0;
    for (unsigned long index = 0; index < (unsigned long)GetSize(); ++index) {
        if (EntryExists(index)) {
            CBankFileEntryUpdateData* update_data = UpdateData[index];
            if (update_data->RequiresUpdate) {
                unsigned long data_type = GetEntryDataType(index);
                try {
                    CCharString status_text;
                    if (IsDiskFile(data_type, 0)) {
                        CCharString path_drive, path_directory, path_filename, path_extension;
                        CAFile::SplitPath(update_data->GetFilename(0), &path_drive, &path_directory, &path_filename, &path_extension);
                        status_text = CCharString("Reading ") + path_filename + path_extension + "\n in " + (path_drive + path_directory).GetAsLowerCase();
                    } else {
                        status_text = CCharString("Processing ") + update_data->GetFilename(0) + "...";
                    }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    // Inline-budget: retail 0x009CF27B calls the callback wrapper here.
                    SetBankUpdateStatus(status_text);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                } catch (CBankCreatorException& error) {
                    CCharString symbol = GetEntrySymbol(index);
                    DeleteEntry(index);
                    EndUpdateEntries();
                    throw CBankCreatorException(symbol, error.GetText());
                }
                ++updated_count;
                if (++updated_since_recovery == 10 && !RetailMode) {
                    WriteFileInformation(false);
                    updated_since_recovery = 0;
                }
                SetBankUpdateProgress(updated_count, total_files_to_update);
            }
            if (g_BankCreationAborted) {
                EndUpdateEntries();
                throw CBankCreatorAbortException();
            }
        }
    }
    EndUpdateEntries();
    ChecksumCache.Close();
}