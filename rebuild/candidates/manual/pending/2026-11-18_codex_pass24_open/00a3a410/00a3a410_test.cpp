#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

typedef std::string CCharString;
struct CWideString {
    CCharString Name;
    CWideString(const char* name):Name(name) {}
    CCharString ToCharString() const { return Name; }
};
struct CAFile {
    static CWideString ConvertPathToFullPath(const CWideString& name) { return name; }
};
struct CFileInformation { int LastWriteTime; };
enum { FILE_ACCESS_READ };
struct CDiskFileWin32 {
    static const char* Bytes;
    static int Time;
    static unsigned ByteReads;
    size_t Length,Position;
    bool Valid;
    CDiskFileWin32(const CWideString& name,int access,int kind)
        :Length(strlen(Bytes)),Position(0),Valid(name.Name=="fixture" && access==FILE_ACCESS_READ && kind==2) {}
    unsigned long GetLength() const { return static_cast<unsigned long>(Length); }
    CFileInformation GetFileInformation() const { CFileInformation info={Time}; return info; }
    void Read(char* dest,size_t size,int)
    {
        ++ByteReads; memcpy(dest,Bytes+Position,size); Position+=size;
    }
    void SetPosition(size_t position) { Position=position; }
};
const char* CDiskFileWin32::Bytes="original";
int CDiskFileWin32::Time=1;
unsigned CDiskFileWin32::ByteReads=0;
struct CBankStateBlock { virtual ~CBankStateBlock() {} };
struct CWADBankStateBlock : CBankStateBlock {};
template<class T> struct CCountedPointer {
    T* Ptr;
    bool Owned;
    explicit CCountedPointer(T* ptr):Ptr(ptr),Owned(true) {}
    template<class U> CCountedPointer(const CCountedPointer<U>& other):Ptr(other.Ptr),Owned(false) {}
    ~CCountedPointer() { if (Owned) delete Ptr; }
};
template<class T> struct CArray : std::vector<T> {
    T* GetAsCArray() { return this->empty()?0:&(*this)[0]; }
    
};
struct CBankFileEntryUpdateData {
    CFileInformation Stored;
    mutable unsigned Reads;
    CBankFileEntryUpdateData():Reads(0) { Stored.LastWriteTime=0; }
    void ReadInfo(unsigned long offset,void* dest,size_t size) const
    {
        if(offset==0 && size==sizeof(Stored)) { ++Reads; memcpy(dest,&Stored,size); }
    }
};
struct CFileDataInputStream {
    CDiskFileWin32& File;
    unsigned long BufferSize;
    CFileDataInputStream(CDiskFileWin32& file,unsigned long size):File(file),BufferSize(size) {}
};
struct CMemoryDataOutputStream {
    CFileInformation Stored;
    size_t Size;
    CMemoryDataOutputStream():Size(0) {}
    void Write(void* info,size_t size) { memcpy(&Stored,info,size); Size=size; }
    size_t GetLength() const { return Size; }
};
struct CMemoryDataInputStream {
    CMemoryDataOutputStream& Data;
    CMemoryDataInputStream(CMemoryDataOutputStream& data):Data(data) {}
};
struct CWADBank {
    CBankFileEntryUpdateData Existing;
    CBankFileEntryUpdateData* UpdateData[2];
    CCharString Contents;
    bool HaveEntry,BadWrite;
    unsigned Adds,Writes;
    CWADBank():HaveEntry(false),BadWrite(false),Adds(0),Writes(0)
    { UpdateData[0]=0; UpdateData[1]=&Existing; }
    unsigned long FindIndexBySymbol(const CCharString& name) const { return HaveEntry&&name=="fixture"?1:0; }
    void AddEntry(const CCharString& symbol,unsigned long type,const CCharString& filename,const CCountedPointer<CBankStateBlock>& state)
    {
        if(symbol=="fixture" && !type && filename.empty() && state.Ptr) { ++Adds; HaveEntry=true; }
    }
    unsigned long GetEntryDataSize(unsigned long) const { return (unsigned long)Contents.size(); }
    CBankFileEntryUpdateData* PeekEntryUpdateData(unsigned long index) { return UpdateData[index]; }
    void ReadEntryData(unsigned long index,unsigned long offset,char* data,unsigned long size)
    {
        if(index==1 && size) memcpy(data,Contents.data()+offset,size);
    }
    void UpdateEntry(unsigned long index,CFileDataInputStream& input,unsigned long size,CMemoryDataInputStream& info,
        size_t infoSize,bool checksum)
    {
        BadWrite=index!=1 || size!=input.File.Length || input.File.Position!=0 || !input.File.Valid ||
            input.BufferSize!=0x4000 || infoSize!=sizeof(CFileInformation) || checksum;
        ++Writes;
        Contents.assign(CDiskFileWin32::Bytes,size);
        Existing.Stored=info.Data.Stored;
    }
    void AddFile(const CWideString&);
};

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

int main()
{
    CWADBank bank;
    CWideString filename("fixture");
    bank.AddFile(filename);
    if(bank.Adds!=1 || bank.Writes!=1 || bank.Contents!="original" || bank.BadWrite)
        return 1;
    bank.AddFile(filename);
    if(bank.Writes!=1 || CDiskFileWin32::ByteReads!=0)
        return 2;
    CDiskFileWin32::Bytes="resized";
    bank.AddFile(filename);
    if(bank.Writes!=2 || bank.Contents!="resized" || CDiskFileWin32::ByteReads!=0 || bank.BadWrite)
        return 3;
    CDiskFileWin32::Time=2;
    CDiskFileWin32::Bytes="edited!";
    bank.AddFile(filename);
    if(bank.Writes!=3 || CDiskFileWin32::ByteReads!=1 || bank.Contents!="edited!")
        return 4;
    CDiskFileWin32::Bytes="ignored";
    bank.AddFile(filename);
    if(bank.Writes!=3 || CDiskFileWin32::ByteReads!=1 || bank.Contents!="edited!" || bank.BadWrite)
        return 5;
    CDiskFileWin32::Time=3;
    CDiskFileWin32::Bytes="edited!";
    bank.AddFile(filename);
    if(bank.Writes!=3 || CDiskFileWin32::ByteReads!=2 || bank.Contents!="edited!" || bank.BadWrite)
        return 6;
    puts("00a3a410_TEST PASS");
    return 0;
}
