// 009fd700 CGraphicInfo constructor behaviour test.
// Exact selected constructor, nested header and two inline accessor bodies.
// External bank operations are controlled service boundaries; full bank lifecycle is not exercised.
#include <cstdio>
#include <cstring>
#include <new>
#include <cstdlib>
class CPixelFormat {public: long FormatIndex; CPixelFormat():FormatIndex(-1) {} };
class CBankFile {public: unsigned long GetEntryDataType(unsigned long index) const;};
class CGraphicInfo;
class CGraphicDataBank:public CBankFile {
public: class CGraphicHeader;
    enum ETextureType {TEXTURE_TYPE_OPAQUE=0,TEXTURE_TYPE_ALPHA=1,TEXTURE_TYPE_BOOLEAN_ALPHA=2,TEXTURE_TYPE_INTERPOLATED_ALPHA=3,TEXTURE_TYPE_BUMP=4,TEXTURE_TYPE_BUMP_ALPHA=5,TEXTURE_TYPE_UNCOMPRESSED_OPAQUE=6,TEXTURE_TYPE_UNCOMPRESSED_ALPHA=7,TEXTURE_TYPE_UNCOMPRESSED_BUMP=8,TEXTURE_TYPE_UNCOMPRESSED_BUMP_ALPHA=9,TEXTURE_TYPE_SIGNED=10,TEXTURE_TYPE_COUNT=11};
 ETextureType GetTextureType(const CGraphicHeader& header,bool is_bump) const;
};
class CGraphicDataBank::CGraphicHeader {
public:
 unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;
 CPixelFormat PixelFormat;
 unsigned char TransparencyType,MipmapLevels,IsFlatAnimatedGraphic:1;
 unsigned long FrameDataSize,CompressedMipmapSizes[1];
 CGraphicHeader():Width(0),Height(0),Depth(0),FrameWidth(0),FrameHeight(0),FrameCount(1),PixelFormat(),TransparencyType(0),MipmapLevels(0),IsFlatAnimatedGraphic(false),FrameDataSize(0) {
  for(unsigned long c_mipmap=0;c_mipmap<1;++c_mipmap) CompressedMipmapSizes[c_mipmap]=0;
 }
};

class CGraphicInfoFrame;
class CGraphicInfo {
public:
 CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header);
 bool IsBump() const {return Bank->GetEntryDataType(Index)==2;}
 CGraphicDataBank* GetBank() const {return Bank;}
 CGraphicDataBank::CGraphicHeader Header;
 CGraphicDataBank::ETextureType TextureType;
 CGraphicDataBank* Bank;
 long Index;
 CGraphicInfoFrame* Frames;
};
CGraphicInfo::CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header)
 :Bank(bank),Index(index),Frames(0) {
 Header=header;
 TextureType=GetBank()->GetTextureType(Header,IsBump());
}

static unsigned long entry_type,expected_index;
static const CGraphicDataBank* expected_bank;
static CGraphicDataBank::ETextureType chosen;
static const CGraphicDataBank::CGraphicHeader* expected_header;
static CGraphicInfo* building;
static int step,failed;
static bool same_header(const CGraphicDataBank::CGraphicHeader& a,const CGraphicDataBank::CGraphicHeader& b) {
 return a.Width==b.Width&&a.Height==b.Height&&a.Depth==b.Depth&&a.FrameWidth==b.FrameWidth&&a.FrameHeight==b.FrameHeight&&a.FrameCount==b.FrameCount&&a.PixelFormat.FormatIndex==b.PixelFormat.FormatIndex&&a.TransparencyType==b.TransparencyType&&a.MipmapLevels==b.MipmapLevels&&a.IsFlatAnimatedGraphic==b.IsFlatAnimatedGraphic&&a.FrameDataSize==b.FrameDataSize&&a.CompressedMipmapSizes[0]==b.CompressedMipmapSizes[0];
}
unsigned long CBankFile::GetEntryDataType(unsigned long index) const {
 if(step++!=0||this!=expected_bank||index!=expected_index||building->Bank!=expected_bank||building->Frames!=0||!same_header(building->Header,*expected_header)) failed=1;
 return entry_type;
}
CGraphicDataBank::ETextureType CGraphicDataBank::GetTextureType(const CGraphicHeader& header,bool is_bump) const {
 if(step++!=1||this!=expected_bank||&header!=&building->Header||!same_header(header,*expected_header)||is_bump!=(entry_type==2)) failed=1;
 return chosen;
}
int main() {
 CGraphicDataBank banks[2];
 unsigned long indices[]={0,1,127,65535};
 unsigned long kinds[]={0,1,2,3};
 unsigned count=0;
 for(unsigned owner=0;owner<2;++owner)for(unsigned n=0;n<4;++n)for(unsigned k=0;k<4;++k)for(unsigned t=0;t<11;++t)for(unsigned pattern=0;pattern<3;++pattern) {
  CGraphicDataBank::CGraphicHeader header;
  if(header.Width||header.Height||header.Depth||header.FrameWidth||header.FrameHeight||header.FrameCount!=1||header.PixelFormat.FormatIndex!=-1||header.TransparencyType||header.MipmapLevels||header.IsFlatAnimatedGraphic||header.FrameDataSize||header.CompressedMipmapSizes[0]) return 1;
  header.Width=(unsigned short)(pattern*301+n);header.Height=(unsigned short)(pattern*203+k);header.Depth=(unsigned short)(pattern*71+t);
  header.FrameWidth=(unsigned short)(pattern*19);header.FrameHeight=(unsigned short)(pattern*41);header.FrameCount=(unsigned short)(pattern*7+1);
  header.PixelFormat.FormatIndex=(long)t-1;header.TransparencyType=(unsigned char)(t+pattern);header.MipmapLevels=(unsigned char)(pattern+1);header.IsFlatAnimatedGraphic=(unsigned char)(pattern&1);
  header.FrameDataSize=pattern*65537UL+n;header.CompressedMipmapSizes[0]=pattern*32767UL+k;
  CGraphicDataBank::CGraphicHeader snapshot=header;
  void* mem=::operator new(sizeof(CGraphicInfo));std::memset(mem,0xa5,sizeof(CGraphicInfo));building=(CGraphicInfo*)mem;
  expected_bank=&banks[owner];expected_index=indices[n];entry_type=kinds[k];chosen=(CGraphicDataBank::ETextureType)t;expected_header=&header;step=0;
  CGraphicInfo* result=new(mem) CGraphicInfo(expected_index,&banks[owner],header);
  if(result!=building||step!=2||failed||result->TextureType!=chosen||result->Index!=(long)expected_index||result->Bank!=expected_bank||result->Frames||!same_header(result->Header,header)||!same_header(header,snapshot))return 2;
  result->~CGraphicInfo();::operator delete(mem);++count;
 }
 if(count!=1056)return 3;
 std::puts("009fd700_TEST PASS");return 0;
}