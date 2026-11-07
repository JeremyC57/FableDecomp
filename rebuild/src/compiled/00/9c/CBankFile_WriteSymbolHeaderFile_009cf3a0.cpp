// CBankFile::WriteSymbolHeaderFile() -- generate public and commented private bank-entry enumerations
// Leaf: ?WriteSymbolHeaderFile@CBankFile@@QAEXXZ
// Retail: Fable.exe 0x009CF3A0, RELOCATION_MATCH (chunk 849 rebase of the held Codex pass 12 source).
// Callees: CBankFileManager::GetDevHeaderFilePath 0x009A7E20 / GetRetailHeaderFilePath 0x009A7ED0 on GBankFileManager 0x013CA79C,
//   CMemoryFile::DumpToFile 0x009E9330, CFileDataOutputStream / CDataOutputStream writers, CCharString formatting, the
//   map<CCharString,CArray<unsigned long> > tree (_M_erase 0x009D3E80), CPackedUIntArray::operator[] const 0x00A629F0.
// Shape: Codex body unchanged; CBankFile is modelled locally (engine/CBankFile.h is HOLD), as the landed 0x00AB8E10 does.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CMEMORYFILE
#define FABLE_RETAIL_CMEMORYFILE_DUMPTOFILE
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/fable_script_text.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CAFile.h"
#include "engine/CMemoryFile.h"
#include "engine/CDataOutputStream.h"
#include "engine/CFileDataOutputStream.h"
#include "engine/CPackedUIntArray.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "fable_retail_banks.h"
class CDateAndTime;
class CBankStateBlock;
class CBankFileEntryStats;
class CBankFileDataTypeCount;
class CDiskFileWin32;
// engine/CBankFile.h is HOLD (flattened layout with opaque members and no virtual table), so the retail class is modelled here, as the
// landed 0x00AB8E10 does: RTTI .?AVCBankFile@@ : CBaseClassNonCopyable, vtable 0x0129B54C (20 slots; GetEnumerationType slot 19 +0x4C),
// members at the offsets engine/CBankFile.h asserts (sizeof 0x110), typed with their Ego_r.pdb types.
class CBankFile : public CBaseClassNonCopyable {
public:
    class CProgressCallback {
    public:
        virtual void NotifyBankUpdateProgress(unsigned long, unsigned long) = 0;
        virtual void NotifyBankUpdateStatus(const CCharString&) = 0;   // slot 1 (+4)
        virtual void NotifyBankLogMessage(const CCharString&) = 0;
        virtual void NotifyBankWarning(const CCharString&) = 0;
        virtual void NotifyBankStats(const CBankFileEntryStats&) = 0;
    };
    class CRuntimeData {
    public:
        unsigned long DataOffset;                                   // +0x00
        unsigned long DataSize;                                     // +0x04
        unsigned char DataType;                                     // +0x08
        bool Valid;                                                 // +0x09
    };
    CBankFile();
    virtual ~CBankFile();
    virtual bool OpenReadOnly(const CCharString&, unsigned long);
    virtual void OpenReadWrite(const CCharString&, unsigned long, unsigned long);
    virtual bool OpenFileReadOnly(const CWideString&, unsigned long);
    virtual void OpenFileReadWrite(const CWideString&, unsigned long, unsigned long);
    virtual void Close();
    virtual void AddEntryMultipleFiles(const CCharString&, unsigned long, const CArray<CCharString>&, const CCountedPointer<CBankStateBlock>);
    virtual void Update();
    void WriteSymbolHeaderFile();
    unsigned long GetEntryDataType(unsigned long index) const { return PackedDataType.GetSize() ? PackedDataType[index-1] : RuntimeData[index].DataType; }
    CCharString GetEntrySymbol(unsigned long index) const { if(Symbols.empty()) return CCharString("(Unknown)"); return Symbols[index]; }
protected:
    void SetBankUpdateStatus(const CCharString& text) { if(ProgressCallback) ProgressCallback->NotifyBankUpdateStatus(text); }
    virtual unsigned long GetDataFormatVersion();
    virtual unsigned long AdjustUpdateEntries();
    virtual void NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&);
    virtual void NotifyBankOpened();
    virtual void NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&);
    virtual void NotifyEntryUpdated(unsigned long, unsigned long, const CArray<char>&);
    virtual void NotifyEntryRemoved(unsigned long);
    virtual void NotifyPostEntriesLoaded();
    virtual void PrepareDataForInclusion(unsigned long, const CArray<CCharString>&, unsigned long, const CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&);
    virtual bool IsDiskFile(unsigned long, unsigned long);
    virtual CDateAndTime GetLastModifiedTime(const CCharString&);
    virtual CCharString GetEnumerationType(unsigned long) const;    // slot 19 (+0x4C)
private:
    unsigned long Size;                                             // +0x04
    CArray<CCharString> Symbols;                                    // +0x08
    CArray<unsigned long> Checksums;                                // +0x14
    CArray<CRuntimeData> RuntimeData;                               // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;                   // +0x2C
    CPackedUIntArray PackedDataOffset;                              // +0x38
    CPackedUIntArray PackedDataSize;                                // +0x48
    CPackedUIntArray PackedDataType;                                // +0x58
    CPackedUIntArray PackedValid;                                   // +0x68
    unsigned long OpenFlags;                                        // +0x78
    CCountedPointer<CDiskFileWin32> BankFile;                       // +0x7C
    CWideString Filename;                                           // +0x84
    CCharString BankHandle;                                         // +0x88
    bool RetailMode;                                                // +0x8C
    bool ReadOnly;                                                  // +0x8D
    bool WrittenToFile;                                             // +0x8E
    bool Solid;                                                     // +0x8F
    unsigned long ChangeCount;                                      // +0x90
    bool FileValid;                                                 // +0x94
    bool UpdatingFlag;                                              // +0x95
    unsigned long Alignment;                                        // +0x98
    unsigned long BankFormatVersion;                                // +0x9C
    unsigned long HeaderSize;                                       // +0xA0
    unsigned long FileInformationSize;                              // +0xA4
    unsigned long FileSize;                                         // +0xA8
    unsigned char SymbolIndices[0xc];                               // +0xAC map<CCharString,unsigned long>
    unsigned char OldSymbolIndices[0xc];                            // +0xB8 map<CCharString,unsigned long>
    unsigned char FilenameIndices[0xc];                             // +0xC4 multimap<CCharString,unsigned long>
    unsigned char SymbolCRCIndices[0x10];                           // +0xD0 CVectorMap<unsigned long,unsigned long>
    CArray<unsigned long> BlockOrder;                               // +0xE0
    CProgressCallback* ProgressCallback;                            // +0xEC
    CCharString HeaderEnumerationType;                              // +0xF0
    bool WarningFlag;                                               // +0xF4
    unsigned char ChecksumCache[0x18];                              // +0xF8 CChecksumCacheFile
};
FABLE_STATIC_ASSERT(sizeof(CBankFile) == 0x110);
void CBankFile::WriteSymbolHeaderFile()
{
 CWideString header_filename;
 if(RetailMode)header_filename=NBankFileManager::GBankFileManager.GetRetailHeaderFilePath(BankHandle);
 else header_filename=NBankFileManager::GBankFileManager.GetDevHeaderFilePath(BankHandle);
 if(header_filename.GetLength()!=0){
  SetBankUpdateStatus("Writing symbol file...");
  CMemoryFile header_file;
  header_file.Open(header_filename,FILE_ACCESS_CREATE,2);
  CFileDataOutputStream outstream(header_file);
  CCharString included_define=BankHandle+"_HEADER_INCLUDED";
  outstream.WriteStringNewLine(CCharString("/* This file was auto-generated by BankCreator */"));
  outstream.WriteStringNewLine("");
  std::map<CCharString,CArray<unsigned long> > enum_lists;
  for(long c_entry=0;c_entry<(long)Size;++c_entry){
   if(RuntimeData[c_entry].Valid&&RuntimeData[c_entry].DataOffset!=0&&!UpdateData[c_entry]->RequiresUpdate){
    CCharString enum_type=GetEnumerationType(GetEntryDataType(c_entry));
    enum_lists[enum_type].push_back(c_entry);
   }
  }
  {
   for(std::map<CCharString,CArray<unsigned long> >::iterator itr=enum_lists.begin();itr!=enum_lists.end();++itr){
    outstream.WriteStringNewLine(CCharString("enum ")+itr->first);
    outstream.WriteStringNewLine("{");
    const CArray<unsigned long>& enum_list=itr->second;
    for(unsigned long i=0;i<enum_list.size();++i){
     unsigned long entry_index=enum_list[i];
     CCharString symbol=GetEntrySymbol(entry_index);
     if(symbol.FindCharPos('[')==-1)outstream.WriteStringNewLine(CCharString("\t")+symbol+" = "+GFIntToCharString(entry_index)+",");
    }
    outstream.WriteStringNewLine("};");
    outstream.WriteStringNewLine("");
   }
  }
  header_file.DumpToFile();
  outstream.WriteStringNewLine("");
  outstream.WriteStringNewLine("/*");
  outstream.WriteStringNewLine("So we can see all the graphic indices (for debugging) the remaining symbols are output here.");
  outstream.WriteStringNewLine("Warning: Do not uncomment!");
  {
   for(std::map<CCharString,CArray<unsigned long> >::iterator itr=enum_lists.begin();itr!=enum_lists.end();++itr){
    outstream.WriteStringNewLine(CCharString("enum ")+itr->first+"_1");
    outstream.WriteStringNewLine("{");
    const CArray<unsigned long>& enum_list=itr->second;
    for(unsigned long i=0;i<enum_list.size();++i){
     unsigned long entry_index=enum_list[i];
     CCharString symbol=GetEntrySymbol(entry_index);
     if(symbol.FindCharPos('[')!=-1)outstream.WriteStringNewLine(CCharString("\t")+symbol+" = "+GFIntToCharString(entry_index)+",");
    }
    outstream.WriteStringNewLine("};");
    outstream.WriteStringNewLine("");
   }
  }
  outstream.WriteStringNewLine("*/");
  header_file.DumpToFile();
  header_file.Close();
 }
}