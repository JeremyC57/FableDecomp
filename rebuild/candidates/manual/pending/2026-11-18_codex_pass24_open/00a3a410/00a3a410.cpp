// CWADBank::AddFile(const CWideString& fileName) -- add (or refresh) the WAD bank entry for a file: a new entry is created when the symbol is unknown; an existing one is rewritten only when the size, the last-write time or the bytes changed
// Grade: DIFFER(942v920) with 41 strict instruction hunks.
// Leaf: ?AddFile@CWADBank@@QAEXABVCWideString@@@Z
// Flags: none
// Retail: 0x00A3A410, 920 bytes.

#define FABLE_RETAIL_WAD_ADD_FILE_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_PEEKENTRYUPDATEDATA
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CBANKFILE_READENTRYDATA
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_READINFO
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDATEANDTIME_CTOR
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "engine/CWADBank.h"
#include "engine/CWADBankStateBlock.h"
#include "engine/CFileInformation.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CMemoryDataOutputStream.h"
#include "string.h"

#pragma intrinsic(memcmp)

void CWADBank::AddFile(const CWideString& name)
{
    unsigned long index = FindIndexBySymbol(name.ToCharString());
    bool previously_existed = index != 0;
    if (!previously_existed) {
        CCountedPointer<CWADBankStateBlock> state_block(new CWADBankStateBlock);
        CCharString empty("");
        AddEntry(name.ToCharString(), 0, empty, state_block);
        index = FindIndexBySymbol(name.ToCharString());
    }
    CWideString pathname = CAFile::ConvertPathToFullPath(name);
    CDiskFileWin32 data_file(pathname, FILE_ACCESS_READ, 2);
    bool copy_file = !previously_existed || GetEntryDataSize(index) != static_cast<unsigned long>(data_file.GetLength());
    if (!copy_file) {
            CFileInformation info;
            PeekEntryUpdateData(index)->ReadInfo(0, &info, sizeof(CFileInformation));
        if (data_file.GetFileInformation().LastWriteTime != info.LastWriteTime) {
            CArray<char> data1;
            CArray<char> data2;
            data1.resize(GetEntryDataSize(index));
            ReadEntryData(index, 0, data1.GetAsCArray(), GetEntryDataSize(index));
            data2.resize(data_file.GetLength());
            data_file.Read(data2.GetAsCArray(), data_file.GetLength(), 0);
            data_file.SetPosition(0);
            if (memcmp(data1.GetAsCArray(), data2.GetAsCArray(), data1.size()) != 0)
                copy_file = true;
        }
    }
    if (!copy_file)
        return;
    CFileDataInputStream data_stream(data_file, 0x4000);
    CMemoryDataOutputStream info_stream;
    CFileInformation file_info = data_file.GetFileInformation();
    info_stream.Write(&file_info, sizeof(CFileInformation));
    CMemoryDataInputStream info_ip_stream(info_stream);
    UpdateEntry(index, data_stream, data_file.GetLength(), info_ip_stream, info_stream.GetLength(), false);
}
