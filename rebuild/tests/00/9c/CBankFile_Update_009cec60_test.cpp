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
#define FABLE_RETAIL_CBANKFILE_TEST_SUPPORT_PASS27
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_STRING_THROW_INLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define private public
#define protected public
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

#include <ctype.h>
#include <stdio.h>
#include <wchar.h>
#undef protected
#undef private

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString() : PStringData(0) {}
CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData)
{
    if (length < 0)
        length = strlen(text);
    PStringData->NoRefs = 1;
    PStringData->Data.PData = new char[length + 1];
    PStringData->Data.StringLength = length;
    PStringData->Data.DataLength = length + 1;
    PStringData->Data.UseFastExtend = false;
    memcpy(PStringData->Data.PData, text, length);
    PStringData->Data.PData[length] = 0;
}
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CCharString::~CCharString()
{
    if (PStringData && --PStringData->NoRefs == 0) {
        delete[] PStringData->Data.PData;
        delete PStringData;
    }
}
static const char* NarrowText(const CCharString& text)
{
    return text.PStringData ? text.PStringData->Data.PeekData() : "";
}
CCharString CCharString::GetAsLowerCase() const
{
    const char* source = NarrowText(*this);
    char lower[128];
    const size_t n = strlen(source);
    if (n >= sizeof(lower))
        abort();
    for (size_t i = 0; i < n; ++i)
        lower[i] = (char)tolower((unsigned char)source[i]);
    lower[n] = 0;
    return CCharString(lower);
}
CCharString __fastcall operator+(const CCharString& left, const char* right)
{
    char result[256];
    if (strlen(NarrowText(left)) + strlen(right) >= sizeof(result))
        abort();
    strcpy(result, NarrowText(left));
    strcat(result, right);
    return CCharString(result);
}
CCharString __fastcall operator+(const CCharString& left, const CCharString& right)
{
    return left + NarrowText(right);
}

CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const wchar_t* text) : PStringData(new CWideStringData)
{
    const size_t n = wcslen(text);
    PStringData->NoRefs = 1;
    PStringData->Data._M_deallocate_block();
    PStringData->Data._M_allocate_block(n + 1);
    memcpy(PStringData->Data._M_start, text, (n + 1) * sizeof(wchar_t));
    PStringData->Data._M_finish += n;
}
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CWideString::~CWideString()
{
    if (PStringData && --PStringData->NoRefs == 0)
        delete PStringData;
}
static const wchar_t* WideText(const CWideString& text)
{
    return text.PStringData ? text.PStringData->Data.c_str() : L"";
}
const CCharString& CCharString::operator=(const CCharString& other)
{
    if (other.PStringData != PStringData) {
        if (other.PStringData)
            ++other.PStringData->NoRefs;
        if (PStringData && --PStringData->NoRefs == 0) {
            delete[] PStringData->Data.PData;
            delete PStringData;
        }
        PStringData = other.PStringData;
    }
    return *this;
}



const CWideString& CWideString::operator=(const CWideString& other)
{
    if (this != &other) {
        if (PStringData && --PStringData->NoRefs == 0)
            delete PStringData;
        PStringData = other.PStringData;
        if (PStringData)
            ++PStringData->NoRefs;
    }
    return *this;
}
const CWideString& CWideString::operator=(const wchar_t* value)
{
    return *this = CWideString(value);
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{
    wchar_t joined[260];
    if (wcslen(WideText(left)) + wcslen(right) >= sizeof(joined) / sizeof(joined[0]))
        abort();
    wcscpy(joined, WideText(left));
    wcscat(joined, right);
    return CWideString(joined);
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{
    return left + WideText(right);
}
CWideString CCharString::ToWideString() const
{
    const char* text = NarrowText(*this);
    wchar_t wide[260];
    if (strlen(text) >= sizeof(wide) / sizeof(wide[0]))
        abort();
    for (size_t i = 0; i <= strlen(text); ++i)
        wide[i] = (unsigned char)text[i];
    return CWideString(wide);
}
void FABLE_FASTCALL CAFile::SplitPath(const CWideString& path, CWideString* drive,
    CWideString* folder, CWideString* name, CWideString* extension)
{
    if (wcscmp(WideText(path), L"C:/Data/Bank.bnk") != 0)
        abort();
    *drive = L"C:";
    *folder = L"/Data/";
    *name = L"Bank";
    *extension = L".bnk";
}
void FABLE_FASTCALL CAFile::SplitPath(const CCharString& path, CCharString* drive,
    CCharString* folder, CCharString* name, CCharString* extension)
{
    if (strcmp(NarrowText(path), "C:/ART/Asset.dat") != 0)
        abort();
    *drive = CCharString("C:");
    *folder = CCharString("/ART/");
    *name = CCharString("Asset");
    *extension = CCharString(".dat");
}

bool g_BankCreationAborted;
unsigned long g_CRCTable[256];
static int cache_opens, cache_closes, cache_checks, begins, ends, deletes, writes, adjustments, disk_checks;
static unsigned long last_deleted;
static bool adjust_result, disk_type, skip_second_file;
const unsigned long checksum_value = 0x98abcd20UL;

CChecksumCacheFile::CChecksumCacheFile(long size) : BufferSize(size), NeedsSaving(false) {}
CChecksumCacheFile::~CChecksumCacheFile() {}
void CChecksumCacheFile::Open(const CWideString& filename)
{
    if (wcscmp(WideText(filename), L"C:/Data/Bank.cks") != 0)
        abort();
    ++cache_opens;
}
void CChecksumCacheFile::Close() { ++cache_closes; }
unsigned long CChecksumCacheFile::GetChecksum(const CWideString& filename)
{
    if (wcscmp(WideText(filename), L"C:/ART/Asset.dat") != 0)
        abort();
    ++cache_checks;
    return checksum_value;
}
CBankCreatorException::CBankCreatorException(const CCharString& entry, const CCharString& text)
    : EntrySymbol(entry), ErrorText(text) {}
CBankCreatorException::CBankCreatorException(const CBankCreatorException& other)
    : EntrySymbol(other.EntrySymbol), ErrorText(other.ErrorText) {}
CBankCreatorException::~CBankCreatorException() {}
CCharString CBankCreatorException::GetText() const { return ErrorText; }

CBankFile::CBankFile() : Size(0), RetailMode(false), ChangeCount(0), FileValid(true),
    ProgressCallback(0), ChecksumCache(1024)
{
    Filename = L"C:/Data/Bank.bnk";
    PackedDataType.PackedInts = 0;
    PackedDataType.Size = 0;
    PackedValid.PackedInts = 0;
    PackedValid.Size = 0;
}
CBankFile::~CBankFile() {}
unsigned long CBankFile::CountEntries() const
{
    unsigned long count = 0;
    for (long i = 0; i < Size; ++i)
        if (RuntimeData[i].Valid)
            ++count;
    return count;
}
void CBankFile::DeleteEntry(unsigned long i)
{
    ++deletes;
    last_deleted = i;
    RuntimeData[i].Valid = false;
    ++ChangeCount;
}
void CBankFile::BeginUpdateEntries() { ++begins; }
void CBankFile::EndUpdateEntries() { ++ends; }
unsigned long CBankFile::AdjustUpdateEntries() { ++adjustments; return adjust_result ? 1 : 0; }
void CBankFile::WriteFileInformation(bool final)
{
    if (final)
        abort();
    ++writes;
}
bool CBankFile::IsDiskFile(unsigned long data_type, unsigned long file)
{
    ++disk_checks;
    if (data_type != 7)
        abort();
    return disk_type && (!skip_second_file || file == 0);
}
unsigned long CPackedUIntArray::operator[](unsigned long index) const
{
    if (index >= Size)
        abort();
    return PackedInts[index];
}
bool CBankFile::OpenReadOnly(const CCharString&, unsigned long) { abort(); }
void CBankFile::OpenReadWrite(const CCharString&, unsigned long, unsigned long) { abort(); }
bool CBankFile::OpenFileReadOnly(const CWideString&, unsigned long) { abort(); }
void CBankFile::OpenFileReadWrite(const CWideString&, unsigned long, unsigned long) { abort(); }
void CBankFile::Close() { abort(); }
void CBankFile::AddEntryMultipleFiles(const CCharString&, unsigned long, const CArray<CCharString>&, const CCountedPointer<CBankStateBlock>) { abort(); }
void CBankFile::NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&) { abort(); }
void CBankFile::NotifyBankOpened() { abort(); }
void CBankFile::NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&) { abort(); }
void CBankFile::NotifyEntryUpdated(unsigned long, unsigned long, const CArray<char>&) { abort(); }
void CBankFile::NotifyEntryRemoved(unsigned long) { abort(); }
void CBankFile::NotifyPostEntriesLoaded() { abort(); }
void CBankFile::PrepareDataForInclusion(unsigned long, const CArray<CCharString>&, unsigned long,
    const CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&) { abort(); }
CDateAndTime CBankFile::GetLastModifiedTime(const CCharString&) { abort(); }
CCharString CBankFile::GetEnumerationType(unsigned long) const { abort(); }

class Callback : public CBankFile::CProgressCallback {
public:
    Callback() : scan_progress(0), update_progress(0), scanned(0), updated(0), update_total(0),
        reports(0), logs(0), abort_scan(false), abort_update(false), throw_on_read(false) { last_status[0] = 0; }
    void SetProgress(unsigned long done, unsigned long total)
    {
        if (!begins) {
            ++scan_progress;
            scanned = done;
            if (abort_scan && done != 0)
                g_BankCreationAborted = true;
        } else {
            ++update_progress;
            updated = done;
            update_total = total;
            if (abort_update && done != 0)
                g_BankCreationAborted = true;
        }
    }
    void SetStatus(const CCharString& value)
    {
        ++reports;
        if (strlen(NarrowText(value)) >= sizeof(last_status))
            abort();
        strcpy(last_status, NarrowText(value));
        if (throw_on_read && begins)
            throw CBankCreatorException(CCharString("test boundary"), CCharString("read failed"));
    }
    void WriteLog(const CCharString& value)
    {
        if (strcmp(NarrowText(value), "Source file has been modified for AssetSymbol") != 0)
            abort();
        ++logs;
    }
    int scan_progress, update_progress;
    unsigned long scanned, updated, update_total;
    int reports, logs;
    bool abort_scan, abort_update, throw_on_read;
    char last_status[128];
};

class TestBank : public CBankFile {
public:
    TestBank() : checksum(0), file0("C:/ART/Asset.dat"), file1("not on disk")
    {
        Add(false, false, false, false);
    }
    unsigned long GetDataFormatVersion() { return 1; }
    void Add(bool valid, bool exists, bool required, bool two_files)
    {
        CRuntimeData runtime;
        runtime.DataType = 7;
        runtime.Valid = valid;
        CBankFileEntryUpdateData& data = entries[Size];
        data.Exists = exists;
        data.RequiresUpdate = required;
        files[Size][0] = file0;
        files[Size][1] = file1;
        data.Filenames.Start = files[Size];
        data.Filenames.Size = two_files ? 2 : 1;
        RuntimeData.push_back(runtime);
        UpdateData.push_back(&data);
        Checksums.push_back(checksum);
        Symbols.push_back(CCharString("AssetSymbol"));
        ++Size;
    }
    unsigned long checksum;
    CCharString file0, file1;
    CCharString files[16][2];
    CBankFileEntryUpdateData entries[16];
};

static void Reset()
{
    cache_opens = cache_closes = cache_checks = begins = ends = deletes = writes = adjustments = disk_checks = 0;
    last_deleted = 0;
    adjust_result = false;
    disk_type = true;
    skip_second_file = true;
    g_BankCreationAborted = false;
}

int main()
{
    for (unsigned long index = 0; index < 256; ++index) {
        unsigned long remainder = index;
        for (int bit = 0; bit < 8; ++bit)
            remainder = (remainder >> 1) ^ ((remainder & 1) ? 0xedb88320UL : 0);
        g_CRCTable[index] = remainder;
    }
    {
        Reset();
        Callback callback;
        TestBank bank;
        bank.ProgressCallback = &callback;
        bank.checksum = CCRC::Calc(0, &checksum_value, sizeof(checksum_value));
        bank.Add(true, true, false, true);
        bank.Add(false, false, false, false);
        bank.Update();
        if (cache_opens != 1 || cache_closes != 1 || cache_checks != 1 || begins || ends || deletes
            || disk_checks != 2 || callback.logs || callback.reports != 1
            || strcmp(callback.last_status, "Scanning files...") || callback.scan_progress != 2)
            return 1;
    }
    {
        Reset();
        Callback callback;
        TestBank bank;
        bank.ProgressCallback = &callback;
        bank.Add(true, true, false, false);
        adjust_result = true;
        bank.Update();
        if (!bank.entries[1].RequiresUpdate || cache_checks != 1 || cache_closes != 1
            || begins != 1 || ends != 1 || adjustments != 1 || callback.logs != 1
            || callback.updated != 1 || callback.update_total != 2
            || strcmp(callback.last_status, "Reading Asset.dat\n in c:/art/"))
            return 2;
    }
    {
        Reset();
        Callback callback;
        TestBank bank;
        bank.ProgressCallback = &callback;
        bank.Add(true, false, false, false);
        bank.Add(true, true, true, false);
        disk_type = false;
        unsigned long packedType[2] = { 7, 7 };
        unsigned long packedValid[2] = { 0, 1 };
        bank.PackedDataType.Size = bank.PackedValid.Size = 2;
        bank.PackedDataType.PackedInts = packedType;
        bank.PackedValid.PackedInts = packedValid;
        bank.RuntimeData[2].DataType = 0;
        bank.Update();
        if (deletes != 1 || last_deleted != 1 || cache_checks || cache_closes != 1 || ends != 1
            || callback.updated != 1 || strcmp(callback.last_status, "Processing C:/ART/Asset.dat..."))
            return 3;
    }
    for (int mode = 0; mode < 2; ++mode) {
        Reset();
        TestBank bank;
        bank.RetailMode = (mode != 0);
        for (int item = 0; item < 11; ++item)
            bank.Add(true, true, true, false);
        bank.Update();
        if (writes != (mode == 0 ? 1 : 0) || begins != 1 || ends != 1 || cache_checks || disk_checks != 11)
            return 4 + mode;
    }
    {
        Reset();
        Callback callback;
        TestBank bank;
        bank.ProgressCallback = &callback;
        callback.abort_scan = true;
        bank.Add(true, true, true, false);
        try { bank.Update(); return 6; }
        catch (const CBankCreatorAbortException&) {
            if (begins || ends || cache_closes || callback.scanned != 1)
                return 7;
        }
    }
    {
        Reset();
        Callback callback;
        TestBank bank;
        bank.ProgressCallback = &callback;
        callback.abort_update = true;
        bank.Add(true, true, true, false);
        try { bank.Update(); return 8; }
        catch (const CBankCreatorAbortException&) {
            if (begins != 1 || ends != 1 || cache_closes || callback.updated != 1)
                return 9;
        }
    }
    {
        Reset();
        Callback callback;
        TestBank bank;
        bank.ProgressCallback = &callback;
        callback.throw_on_read = true;
        bank.Add(true, true, true, false);
        try { bank.Update(); return 10; }
        catch (const CBankCreatorException& error) {
            if (begins != 1 || ends != 1 || deletes != 1 || last_deleted != 1
                || cache_closes || strcmp(NarrowText(error.EntrySymbol), "AssetSymbol")
                || strcmp(NarrowText(error.ErrorText), "read failed"))
                return 11;
        }
    }
    {
        Reset();
        TestBank bank;
        bank.FileValid = false;
        bank.Update();
        if (begins != 1 || ends != 1 || cache_opens != 1 || cache_closes != 1)
            return 12;
    }
    puts("009cec60_TEST PASS");
    return 0;
}