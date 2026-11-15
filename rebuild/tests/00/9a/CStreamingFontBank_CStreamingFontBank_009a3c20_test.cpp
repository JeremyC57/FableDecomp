// Complete production constructor; typed host input fixture with native32 glyph records/flag words.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <new>
void* operator new[](size_t);
void operator delete[](void*) throw();
static bool bad;static int events[16],nevents,reads,textureCount,decompressions;static unsigned long wantedIndex;static void Event(int x){if(nevents>=16)bad=true;else events[nevents++]=x;}
class CCharString {public:std::string text;CCharString(){}CCharString(const CCharString&v):text(v.text){}CCharString&operator=(const CCharString&v){text=v.text;return *this;}};
class CFontBank {public:unsigned int FontHeight;CCharString Name;void* PManager;};
class CTexture {public:void*p,*q;CTexture():p(0),q(0){++textureCount;Event(8);}};
class CStreamingGlyph {public:unsigned int GlyphIndex:24;unsigned int AdjustmentIndex:8;};
template<class T>class CArray {std::vector<T> a;public:explicit CArray(unsigned int n):a(n){}T*GetAsCArray(){return a.empty()?(T*)this:&a[0];}};
class CBankFile {public:std::vector<unsigned char>data;unsigned long GetEntryDataSize(unsigned long i){Event(4);if(i!=wantedIndex)bad=true;return(unsigned long)data.size();}void ReadEntryData(unsigned long i,unsigned long off,void*out,unsigned long n){Event(5);if(i!=wantedIndex||off!=0||n!=data.size())bad=true;if(n<=data.size())memcpy(out,&data[0],n);}};
class CStreamingFontDataBank:public CBankFile{};static CStreamingFontDataBank*wantedBank;
class CMemoryDataInputStream {const unsigned char*p;unsigned long n,pos;public:CMemoryDataInputStream(const void*d,unsigned long size):p((const unsigned char*)d),n(size),pos(0){Event(6);}~CMemoryDataInputStream(){if(pos!=n)bad=true;Event(9);}void Read(void*out,unsigned long size){++reads;if(size>n-pos){bad=true;memset(out,0,size);return;}memcpy(out,p+pos,size);pos+=size;}unsigned long ReadULONG(){unsigned int x=0;Read(&x,4);return x;}long ReadSLONG(){int x=0;Read(&x,4);return x;}bool ReadEBOOL(){unsigned char x=0;Read(&x,1);return x!=0;}CCharString ReadNullTerminatedString(){CCharString x;while(pos<n&&p[pos])x.text+=(char)p[pos++];if(pos>=n)bad=true;else++pos;return CCharString(x);}};
class CStreamingFontBank:public CFontBank {public:struct GlyphBank {unsigned short FirstGlyph,NoGlyphs;CStreamingGlyph Glyphs[64];};CStreamingFontDataBank*data_bank;GlyphBank*glyphBankCache;unsigned long glyphBankCacheIndex;unsigned int MaxHeight;int Weight;bool Italics;void*GlyphBankMemory;const void*GlyphBanks[1024];unsigned int GlyphFlags[2048];unsigned long fontIndex;CTexture*PTexture;CStreamingFontBank(CStreamingFontDataBank*,unsigned long);};
typedef char GlyphSize[(sizeof(CStreamingGlyph)==4)?1:-1];typedef char BankSize[(sizeof(CStreamingFontBank::GlyphBank)==260)?1:-1];
static CStreamingFontBank* expectedRegistered;static unsigned long expectedSlot;
namespace {struct StreamingFontCacheBufferData {int id;};template<int W,int H>struct StreamingFontCacheBufferData_static:StreamingFontCacheBufferData{};StreamingFontCacheBufferData_static<32,32>g_StreamingFontCacheBufferData;StreamingFontCacheBufferData_static<64,64>g_StreamingFontCacheBufferData2;CStreamingFontBank*streamingFontBankList[8];void InitializeStreamingFontTextureCache(CStreamingFontDataBank*b){Event(1);if(b!=wantedBank||streamingFontBankList[expectedSlot]!=expectedRegistered)bad=true;}void InitializeStreamingFontCacheEntries(StreamingFontCacheBufferData&v){Event(&v==&g_StreamingFontCacheBufferData?2:3);if(&v!=&g_StreamingFontCacheBufferData&&&v!=&g_StreamingFontCacheBufferData2)bad=true;}}
static std::vector<const unsigned char*>compressedInputs;
extern "C" int lzo1x_decompress_safe(const unsigned char*src,unsigned int n,unsigned char*out,unsigned int*outn,void*wrk){if(n!=260||*outn!=260||wrk||!out)bad=true;compressedInputs.push_back(src);++decompressions;memcpy(out,src,260);*outn=260;return 0;}

CStreamingFontBank::CStreamingFontBank(CStreamingFontDataBank* data_bank_, unsigned long index)
{
    data_bank = data_bank_;
    fontIndex = 0;
    while (fontIndex < 8 && streamingFontBankList[fontIndex] != this)
        ++fontIndex;
    if (fontIndex == 8) {
        for (fontIndex = 0; fontIndex < 8; ++fontIndex)
            if (streamingFontBankList[fontIndex] == 0)
                break;
        streamingFontBankList[fontIndex] = this;
    }
    InitializeStreamingFontTextureCache(data_bank);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheBufferData);
    InitializeStreamingFontCacheEntries(g_StreamingFontCacheBufferData2);
    memset(GlyphBanks, 0, sizeof(GlyphBanks));
    memset(GlyphFlags, 0, sizeof(GlyphFlags));
    // CStreamingFontDataBank : CBankFile at offset 0 (RTTI); the shared header is the flattened Ego_r layout.
    CBankFile* file = (CBankFile*)data_bank;
    unsigned long data_size = file->GetEntryDataSize(index);
    CArray<char> data_buffer(data_size);
    ((CBankFile*)data_bank)->ReadEntryData(index, 0, data_buffer.GetAsCArray(), data_size);
    CMemoryDataInputStream stream(data_buffer.GetAsCArray(), data_size);
    glyphBankCache = new GlyphBank;
    glyphBankCacheIndex = 0xffffffff;
    Name = stream.ReadNullTerminatedString();
    FontHeight = stream.ReadULONG();
    Weight = stream.ReadSLONG();
    Italics = stream.ReadEBOOL();
    MaxHeight = stream.ReadULONG();
    unsigned long BankOffsets[0x401];
    stream.Read(BankOffsets, sizeof(BankOffsets));
    GlyphBankMemory = operator new[](BankOffsets[0x400]);
    stream.Read(GlyphBankMemory, BankOffsets[0x400]);
    for (unsigned long bankIndex = 0; bankIndex < 0x400; ++bankIndex) {
        const unsigned long bankSize = BankOffsets[bankIndex + 1] - BankOffsets[bankIndex];
        if (bankSize != 0) {
            GlyphBanks[bankIndex] = (const unsigned char*)GlyphBankMemory + BankOffsets[bankIndex];
            const GlyphBank* bank = (const GlyphBank*)((const unsigned char*)GlyphBanks[bankIndex] + 2);
            if (*(const unsigned short*)GlyphBanks[bankIndex] & 0x8000) {
                unsigned int output_size = sizeof(GlyphBank);
                lzo1x_decompress_safe((const unsigned char*)GlyphBanks[bankIndex] + 2, bankSize - 2, (unsigned char*)glyphBankCache, &output_size, 0);
                bank = glyphBankCache;
                glyphBankCacheIndex = bankIndex;
            }
            for (unsigned long c1 = 0; c1 < bank->NoGlyphs; ++c1) {
                if (bank->Glyphs[c1].GlyphIndex != 0) {
                    const unsigned short ch = (unsigned short)((bankIndex << 6) + c1 + bank->FirstGlyph);
                    GlyphFlags[ch >> 5] |= 1 << (ch & 31);
                }
            }
        }
    }
    PTexture = new CTexture;
}


template<class T>static void Append(std::vector<unsigned char>&out,const T&v){const unsigned char*p=(const unsigned char*)&v;out.insert(out.end(),p,p+sizeof(v));}
static int Fail(int n,const char*s){printf("009a3c20_TEST FAIL %d %s\n",n,s);return 1;}
int main(){int cases=0;const unsigned int bankIds[5]={0,1,15,511,1023};const int counts[4]={0,1,7,64};
 for(int registry=0;registry<3;++registry)for(int slot=0;slot<8;++slot)for(int seed=0;seed<8;++seed)for(int pattern=0;pattern<4;++pattern)for(int packed=0;packed<3;++packed,++cases){
  bad=false;nevents=reads=textureCount=decompressions=0;compressedInputs.clear();CStreamingFontDataBank file;wantedBank=&file;wantedIndex=13+seed;std::vector<unsigned char>memory;unsigned long offsets[1025];unsigned int expectedFlags[2048];memset(expectedFlags,0,sizeof(expectedFlags));bool present[1024];memset(present,0,sizeof(present));CStreamingFontBank::GlyphBank lastPacked;unsigned long lastIndex=0xffffffffUL;int expectedPacked=0;
  for(unsigned int i=0;i<1024;++i){offsets[i]=(unsigned long)memory.size();int which=-1;for(int k=0;k<5;++k)if(bankIds[k]==i)which=k;if(which<0)continue;present[i]=true;CStreamingFontBank::GlyphBank b;memset(&b,0,sizeof(b));b.FirstGlyph=(unsigned short)((seed*11+which*7)&63);b.NoGlyphs=(unsigned short)counts[pattern];for(unsigned int j=0;j<64;++j){b.Glyphs[j].GlyphIndex=((j+seed+which)%3)?(17+j*31):0;b.Glyphs[j].AdjustmentIndex=(unsigned char)(seed*29+j);if(j<b.NoGlyphs&&b.Glyphs[j].GlyphIndex){unsigned int code=(i*64+j+b.FirstGlyph)&65535;expectedFlags[code/32]|=1u<<(code%32);}}bool compressed=packed==1||(packed==2&&(which&1));unsigned short header=(unsigned short)(260|(compressed?0x8000:0));Append(memory,header);const unsigned char*p=(const unsigned char*)&b;memory.insert(memory.end(),p,p+260);if(compressed){lastPacked=b;lastIndex=i;++expectedPacked;}}
  offsets[1024]=(unsigned long)memory.size();char name[32];sprintf(name,"font-bank-%d",seed);file.data.insert(file.data.end(),(const unsigned char*)name,(const unsigned char*)name+strlen(name)+1);unsigned int height=9+seed,maxHeight=33+seed;int weight=-21+seed;unsigned char italics=(unsigned char)(seed%3?255:0);Append(file.data,height);Append(file.data,weight);Append(file.data,italics);Append(file.data,maxHeight);for(int k=0;k<1025;++k)Append(file.data,offsets[k]);file.data.insert(file.data.end(),memory.begin(),memory.end());
  void*storage=::operator new(sizeof(CStreamingFontBank));CStreamingFontBank*address=(CStreamingFontBank*)storage;CStreamingFontBank*before[8];unsigned long chosen=0;
  for(int k=0;k<8;++k)streamingFontBankList[k]=(CStreamingFontBank*)(size_t)(0x1000+k*16);
  if(registry==0){for(int k=0;k<8;++k)streamingFontBankList[k]=0;chosen=0;}else if(registry==1){streamingFontBankList[slot]=0;chosen=(unsigned long)slot;}else{streamingFontBankList[slot]=address;chosen=(unsigned long)slot;}
  memcpy(before,streamingFontBankList,sizeof(before));expectedRegistered=address;expectedSlot=chosen;CStreamingFontBank*b=new(storage)CStreamingFontBank(&file,wantedIndex);
  if(bad||b!=address||b->data_bank!=&file||b->fontIndex!=chosen||b->Name.text!=name||b->FontHeight!=height||b->Weight!=weight||b->Italics!=(italics!=0)||b->MaxHeight!=maxHeight)return Fail(cases,"registration/scalar/name");
  if(nevents!=8||textureCount!=1||reads!=6||decompressions!=expectedPacked)return Fail(cases,"event/read/decompression counts");if(nevents!=8||events[0]!=1||events[1]!=2||events[2]!=3||events[3]!=4||events[4]!=5||events[5]!=6||events[6]!=8||events[7]!=9)return Fail(cases,"external order");
  for(int k=0;k<8;++k)if(streamingFontBankList[k]!=(k==(int)chosen?b:before[k]))return Fail(cases,"table neighbours");
  if(!b->PTexture||b->PTexture->p||b->PTexture->q||!b->glyphBankCache||!b->GlyphBankMemory||b->glyphBankCacheIndex!=lastIndex)return Fail(cases,"owned allocation/cache");
  if(memcmp(b->GlyphFlags,expectedFlags,sizeof(expectedFlags)))return Fail(cases,"all65536glyph presence bits");
  if(memcmp(b->GlyphBankMemory,&memory[0],memory.size()))return Fail(cases,"bank memory read");
  for(unsigned int i=0;i<1024;++i){const void*expected=present[i]?(const unsigned char*)b->GlyphBankMemory+offsets[i]:0;if(b->GlyphBanks[i]!=expected)return Fail(cases,"all1024bank pointers");}
  if(expectedPacked&&memcmp(b->glyphBankCache,&lastPacked,260))return Fail(cases,"last decompressed cache");
  for(unsigned int k=0;k<compressedInputs.size();++k){const unsigned char*p=compressedInputs[k];if(p<(const unsigned char*)b->GlyphBankMemory+2||p>(const unsigned char*)b->GlyphBankMemory+memory.size()-260)return Fail(cases,"decompress source range");}
  delete b->glyphBankCache;::operator delete[](b->GlyphBankMemory);delete b->PTexture;b->~CStreamingFontBank();::operator delete(storage);
 }
 if(cases!=2304)return Fail(cases,"coverage");
 puts("009a3c20_TEST PASS");return 0;}