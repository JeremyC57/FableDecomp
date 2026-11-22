#include <stdio.h>
#include <string.h>

// The fixture records compressor input separately from the bytes it writes.
template<class T> class CArray {
    T items[1536];
    unsigned int count;
public:
    CArray(unsigned int n=0):count(n) { memset(items,0,sizeof(items)); }
    unsigned int size() const { return count; }
    void resize(unsigned int n) { count=n; }
    unsigned int GetByteSize() const { return count*sizeof(T); }
    T* GetAsCArray() { return items; }
    T& operator[](unsigned int i) { return items[i]; }
    void clear() { count=0; }
};
class CMemoryDataOutputStream {
    unsigned char data[256];
    unsigned int n;
public:
    CMemoryDataOutputStream():n(0) {}
    unsigned long GetLength() const {return n;}
    const void* PeekData() const {return data;}
    void Clear(){n=0;}
    void Write(const void* p,long len){memcpy(data+n,p,len);n+=len;}
};
static unsigned char observed[256];
static unsigned int source_len,dest_len;
static bool empty_result;
class CCompressorZlib {
public:
    unsigned long InternalCompress(const void* src,unsigned long n,void* dst,unsigned long cap) {
        memcpy(observed,src,n);source_len=n;dest_len=cap;
        if (empty_result) return 0;
        unsigned char* out=(unsigned char*)dst;
        out[0]=0xD3;
        for(unsigned int i=0;i<n;++i)out[i+1]=observed[n-1-i]^0xA5;
        return n+1;
    }
};
class CDefinitionManager {
public:
    CArray<unsigned char> CompressedDefData;
    struct Offsets {
        unsigned long keys[2],values[2],n;
        Offsets():n(0){}
        void Insert(unsigned long k,unsigned long v){keys[n]=k;values[n++]=v;}
    } CompressedOffsets;
    void CompressDefinitionBlock(CMemoryDataOutputStream&,CArray<unsigned short>&,unsigned long);
};
void CDefinitionManager::CompressDefinitionBlock(CMemoryDataOutputStream& uncompressed_buffer,
    CArray<unsigned short>& def_offsets, unsigned long first_compressed_def)
{
    CCompressorZlib compressor;
    CMemoryDataOutputStream buffer_with_offsets;
    unsigned long adjust_offsets = def_offsets.GetByteSize();
    for (unsigned long c_offset = 0; c_offset < def_offsets.size(); ++c_offset)
        def_offsets[c_offset] += adjust_offsets;
    buffer_with_offsets.Write(def_offsets.GetAsCArray(),def_offsets.GetByteSize());
    buffer_with_offsets.Write(uncompressed_buffer.PeekData(),uncompressed_buffer.GetLength());
    CArray<unsigned char> compressed_buffer(buffer_with_offsets.GetLength() + 1024);
    unsigned long compressed_size = compressor.InternalCompress(buffer_with_offsets.PeekData(),
        buffer_with_offsets.GetLength(),compressed_buffer.GetAsCArray(),compressed_buffer.size());
    unsigned long compressed_offset = CompressedDefData.size();
    CompressedDefData.resize(compressed_offset + compressed_size);
    memcpy(&CompressedDefData[compressed_offset],compressed_buffer.GetAsCArray(),compressed_size);
    CompressedOffsets.Insert(first_compressed_def,compressed_offset);
    uncompressed_buffer.Clear();
    def_offsets.clear();
}

int main() {
    for(unsigned int offsets=0;offsets<=4;++offsets)
    for(unsigned int payload=0;payload<=3;++payload)
    for(unsigned int prefix=0;prefix<=2;++prefix)
    for(unsigned int empty=0;empty<=1;++empty) {
        CDefinitionManager m;
        m.CompressedDefData.resize(prefix);
        for(unsigned int i=0;i<prefix;++i) m.CompressedDefData[i]=0x77;
        CArray<unsigned short> table(offsets);
        unsigned char expected[16],input[]={0,0xAB,0xFF};
        unsigned int length=2*offsets+payload;
        for(unsigned int k=0;k<offsets;++k) {
            table[k]=(unsigned short)(65535-k*400);
            unsigned int rebased=(table[k]+2*offsets)&65535;
            expected[2*k]=(unsigned char)rebased;
            expected[2*k+1]=(unsigned char)(rebased>>8);
        }
        memcpy(expected+2*offsets,input,payload);
        CMemoryDataOutputStream stream;
        stream.Write(input,payload);
        empty_result=empty!=0;
        m.CompressDefinitionBlock(stream,table,0x34567899);
        if (source_len!=length || dest_len!=length+1024 ||
            memcmp(observed,expected,length) || stream.GetLength() || table.size()) return 1;
        if (m.CompressedOffsets.n!=1 || m.CompressedOffsets.keys[0]!=0x34567899 ||
            m.CompressedOffsets.values[0]!=prefix) return 2;
        if (m.CompressedDefData.size()!=prefix+(empty ? 0 : length+1)) return 3;
        for(unsigned int j=0;j<prefix;++j)if(m.CompressedDefData[j]!=0x77)return 4;
        if (!empty) {
            if(m.CompressedDefData[prefix]!=0xD3)return 5;
            for(unsigned int t=0;t<length;++t)
                if(m.CompressedDefData[prefix+1+t]!=(unsigned char)(expected[length-1-t]^0xA5))return 6;
        }
    }
    puts("009ae390_TEST PASS");
    return 0;
}