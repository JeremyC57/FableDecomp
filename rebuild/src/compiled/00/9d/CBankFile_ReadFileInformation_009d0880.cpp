// 009d0880 CBankFile::ReadFileInformation
// Leaf: ?ReadFileInformation@CBankFile@@AAE_N_N@Z
// Flags: /GS
#define FABLE_RETAIL_CBANKFILE_UPDATE_PASS27
#define FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
#define FABLE_RETAIL_CWAVELOADER_PASS22_EXCEPTIONS
#define FABLE_RETAIL_CBANKFILE_READFILEINFORMATION_PASS29
#define FABLE_RETAIL_CBANKFILE_WRITEFILEINFORMATION
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_BANKFILE_FILENAME_COPY_SITE_PASS26
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_BANKFILE_INDEX_UPDATE_PASS26
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_CBANKFILE_READENTRIES_PASS26
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_ALLOCATEBUFFERS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_LOADGAME_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#include <stddef.h>
#include <string.h>
#include "engine/CBankFileDataTypeCount.h"
#include "engine/CVectorMap.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "engine/CBankFile.h"
#include "engine/CFileDataInputStream.h"

CCharString FABLE_FASTCALL GFIntToCharString(long);
CCharString FABLE_FASTCALL operator+(const CCharString&,const char*);
CCharString FABLE_FASTCALL operator+(const CCharString&,const CCharString&);
#include "engine/CBankFileException.h"

bool CBankFile::ReadFileInformation(bool throwOnError)
{
    CCharString name(Filename.ToCharString());
    CFileDataInputStream stream(*BankFile, 0x4000);
    stream.SetPosition(0);
    bool valid_header = false;
    if (!stream.IsEOF()) {
        char correct_header[] = "BBBB";
        char header[4];
        stream.Read(header, 4);
        if (memcmp(header, correct_header, 4) == 0)
            valid_header = true;
    }
    if (!valid_header) {
        if (throwOnError)
            throw CBankFileException(name + " is not a valid resource file!", CBankFileException::BANK_IS_CORRUPT);
        return false;
    }
    unsigned long format = stream.ReadULONG();
    if (format != BankFormatVersion) {
        if (ReadOnly) {
            if (throwOnError) {
                throw CBankFileException(name + " has the wrong bank format! (should be " + GFIntToCharString(BankFormatVersion) +
                                     " not " + GFIntToCharString(format) + ")", CBankFileException::BANK_IS_INCORRECT_VERSION);
            }
            return false;
        }
        WriteBankLog("Bank file version has changed, requiring rebuild");
        return true;
    }
    unsigned long version = stream.ReadULONG();
    if (version != GetDataFormatVersion()) {
        if (ReadOnly) {
            if (throwOnError) {
                throw CBankFileException(name + " has the wrong version number! (should be " + GFIntToCharString(GetDataFormatVersion()) +
                                     " not " + GFIntToCharString(version) + ")", CBankFileException::BANK_IS_INCORRECT_VERSION);
            }
            return false;
        }
        WriteBankLog("Bank data version has changed, requiring rebuild");
        return true;
    }
    unsigned long updateState = stream.ReadULONG();
    unsigned long alignment = stream.ReadULONG();
    if (alignment != Alignment) {
        if (ReadOnly) {
            Alignment = alignment;
        } else {
            WriteBankLog("Data alignment has changed, requiring rebuild");
            return true;
        }
    }
    unsigned long maxIndex = stream.ReadULONG();
    unsigned long entryCount = stream.ReadULONG();
    unsigned long entryListStart = stream.ReadULONG();
    HeaderSize = stream.GetPosition();
    FileSize = BankFile->GetLength();
    if (updateState != 1) {
        if (ReadOnly) {
            if (throwOnError) {
                throw CBankFileException(CBankFileException::BANK_IS_CORRUPT);
            }
            return false;
        }
        if (!ReadRecoveryFile(updateState))
            WriteBankLog("Previous bank update was interrupted, requiring rebuild");
        return true;
    }
    stream.SetPosition(entryListStart);
    return ReadEntryList(stream, maxIndex, entryCount);
}