// Grade: DIFFER(658v662) -- frame (0x54) and slot sharing match (one long reused for the count and each index, make_pair + pair<const> copies, inline 64-bit-range ReadLong, malloc vector, STLport map with malloc header); VC7.1 here keeps the entry argument in ebx and the zero in edi, retail reloads [ebp+8] and keeps this in edi / zero in ebx
// CEngineWorldMap::OpenMapFile(unsigned long entry) -- read a static-map bank entry into a buffer, parse its name -> index table into a map<CCharString,long>, then let every loaded map open its retail static data from the same stream
// Leaf: ?OpenMapFile@CEngineWorldMap
#include <stdlib.h>
#include <string.h>
struct CCharString {
    void* Data;
    CCharString(const CCharString& other);                     // 0x0099EC30
    ~CCharString();                                            // 0x0099EAE0
};
struct CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* out, long size);   // 0x00993CA0
};
inline bool IsValidPosition(__int64 position)
{
    return position >= 0 && position <= 0x7fffffff;
}
struct CMemoryDataInputStream : CDataInputStream {
    virtual void v000();
    unsigned long  Position;                                   // +0x04
    unsigned long  _08;
    unsigned char* Cursor;                                     // +0x0c
    unsigned long  _10;
    long           Remaining;                                  // +0x14
    unsigned long  _18;
    CMemoryDataInputStream(void* data, unsigned long size);    // 0x00994930
    ~CMemoryDataInputStream();                                 // 0x00993B10
    CCharString ReadNullTerminatedString();                    // 0x00995D30
    __forceinline void ReadLong(long& value)
    {
        unsigned long end = Position + 4;
        if (IsValidPosition(end)) {
            if (Remaining >= 4) {
                value = *(long*)Cursor;
                Cursor += 4;
                Remaining -= 4;
                Position = end;
            }
            else
                ReadWithSrcChunkOverflow((unsigned char*)&value, 4);
        }
    }
};
struct CBankFile {
    unsigned long GetEntryDataSize(unsigned long entry);       // 0x009CC240
    void ReadEntryData(unsigned long entry, unsigned long offset, void* data, unsigned long size);   // 0x009CC2A0
};
struct CNameNode {
    unsigned char Colour;
    CNameNode* Parent;
    CNameNode* Left;
    CNameNode* Right;
};
struct CNameIndexPair {
    CCharString first;
    long second;
    CNameIndexPair(const CCharString& name, const long& index) : first(name), second(index) { }
};
inline CNameIndexPair make_pair(const CCharString& name, const long& index) { return CNameIndexPair(name, index); }
struct CNamePair {
    CCharString first;
    long second;
    CNamePair(const CNameIndexPair& other) : first(other.first), second(other.second) { }
};
struct CNameMapInsertResult { CNameNode* Node; bool Inserted; };
struct CNameMap {
    CNameNode* _M_header;
    unsigned long _M_node_count;
    CNameMap() : _M_node_count(0)
    {
        _M_header = (CNameNode*)malloc(0x18);
        _M_header->Colour = 0;
        _M_header->Parent = 0;
        _M_header->Left = _M_header;
        _M_header->Right = _M_header;
    }
    ~CNameMap()
    {
        clear();
        free(_M_header);
    }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->Parent);
            _M_header->Left = _M_header;
            _M_header->Parent = 0;
            _M_header->Right = _M_header;
            _M_node_count = 0;
        }
    }
    void _M_erase(CNameNode* node);                            // 0x0043020A
    CNameMapInsertResult insert(const CNamePair& value);       // 0x00431606
};
struct CEngineMap {
    void OpenRetailStaticMap(CBankFile* bank, CDataInputStream* stream, CNameMap* names);   // 0x00B41E50
};
struct CMapVector {
    CEngineMap** Start; CEngineMap** Finish; CEngineMap** End;
    unsigned int size() const { return Finish - Start; }
};
struct CByteVector {
    char* Start; char* Finish; char* End;
    explicit CByteVector(unsigned int n) : Start(0), Finish(0), End(0)
    {
        if (n)
            Start = (char*)malloc(n);
        End = Start + n;
        memset(Start, 0, n);
        Finish = Start + n;
    }
    ~CByteVector() { if (Start) free(Start); }
    unsigned int size() const { return Finish - Start; }
    char* data() { return size() ? Start : (char*)this; }
};
struct CEngineWorldMap {
    unsigned char _pad[0x20];
    CMapVector Maps;                                           // +0x20
    unsigned char _pad2c[8];
    CBankFile BankFile;                                        // +0x34
    void OpenMapFile(unsigned long entry);
};
void CEngineWorldMap::OpenMapFile(unsigned long entry)
{
    unsigned long size = BankFile.GetEntryDataSize(entry);
    CByteVector buffer(size);
    BankFile.ReadEntryData(entry, 0, buffer.data(), size);
    CMemoryDataInputStream stream(buffer.data(), buffer.size());
    CNameMap names;
    long value;
    stream.ReadLong(value);
    for (long i = 0, count = value; i < count; ++i) {
        CCharString name = stream.ReadNullTerminatedString();
        stream.ReadLong(value);
        names.insert(make_pair(name, value));
    }
    for (unsigned int m = 0; m < Maps.size(); ++m)
        if (Maps.Start[m])
            Maps.Start[m]->OpenRetailStaticMap(&BankFile, &stream, &names);
}
