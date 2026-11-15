// Full SaveGameState body copied verbatim; engine serialization and I/O boundaries are portable observable fixtures.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#define FABLE_FASTCALL
class CCharString {public:std::string value;CCharString(const char* p=""):value(p){}CCharString(const std::string& p):value(p){}unsigned long GetLength()const{return (unsigned long)value.size();}const char* GetAsCharArray()const{if(value.empty())return "";return value.c_str();}operator const char*()const{return value.c_str();}};
CCharString operator+(const char* p,const CCharString& x){return CCharString(std::string(p)+x.value);}
class CWideString {public:std::string value;CWideString(const char* p=""):value(p){}CCharString ToCharString()const{return CCharString(value);}};
CCharString GFIntToCharString(long x){char b[40];std::sprintf(b,"%ld",x);return CCharString(b);}
struct Record {std::string name,value;Record(const std::string& n,const std::string& v):name(n),value(v){}};
std::vector<Record> records;
static std::string Encode(const CCharString& x){return x.value;}static std::string Encode(const CWideString& x){return x.value;}
static std::string Encode(float x){char b[40];std::sprintf(b,"%.3f",x);return b;}
template<class T>static std::string Encode(const T& x){std::ostringstream out;out<<x;return out.str();}
class CPersistContext {CCharString& out;public:CPersistContext(CCharString& x):out(x){}
 template<class T>void Transfer(const char* name,T& value){records.push_back(Record(name,Encode(value)));out.value+=std::string(name)+"="+Encode(value)+";";}
 template<class T>void TransferVector(const char* name,std::vector<T>& value){std::string joined;for(unsigned int i=0;i<value.size();++i){if(i)joined+=",";joined+=Encode(value[i]);}records.push_back(Record(name,joined));out.value+=std::string(name)+"="+joined+";";}
};
class CMemoryDataOutputStream {std::vector<unsigned char> bytes;public:
 void WriteString(const CCharString& text){Write(text.GetAsCharArray(),(long)text.GetLength());}
 void WriteULONG(unsigned long x){for(int i=0;i<4;++i){bytes.push_back((unsigned char)(x&255));x>>=8;}}
 void ReserveSize(unsigned long n){bytes.reserve(n);}void Write(const void* p,long n){const unsigned char* q=(const unsigned char*)p;for(long i=0;i<n;++i)bytes.push_back(q[i]);}
 const void* PeekData()const{return bytes.empty()?0:&bytes[0];}unsigned long GetLength()const{return (unsigned long)bytes.size();}unsigned long GetPosition()const{return GetLength();}
 void WriteRepeatedByte(unsigned char c,long n){for(long i=0;i<n;++i)bytes.push_back(c);}
};
class CAFile {public:std::vector<unsigned char> bytes;long flag;void Write(const void* p,unsigned long n,long f){flag=f;const unsigned char* q=(const unsigned char*)p;bytes.assign(q,q+n);}};
unsigned long g_CRCTable[256];
class CCRC {public:static unsigned long Calc(unsigned long seed,const void* data,unsigned long byteCount){
 if(byteCount==0)return seed;
 const unsigned char* bytes=static_cast<const unsigned char*>(data);
 for(unsigned long index=0;index<byteCount;++index)
  seed=g_CRCTable[bytes[index]^(seed&0xff)]^(seed>>8);
 return seed;
}};
class CUserProfileManager {public:
 CWideString ProfileName,ScoreBoardUserName,ScoreBoardPassword,ScoreBoardProxyIp;
 bool SubtitlingEnabled,VibrationEnabled,CameraXYInverted,CameraYZInverted,CameraFPBowXYInverted,CameraFPBowYZInverted,CameraPCBowXYInverted,CameraPCBowYZInverted,CameraPCViewHeroXYInverted,CameraPCViewHeroYZInverted,TutorialSystemEnabled,GuildMasterEnabled,HUDTooltipEnabled,UsingHeadPhones,ShowingBuddyNames,CameraResetting,VerticalSyncEnabled,ShowTargetingStatus;
 float Gamma,HUDAlpha,SoundVolume,MusicVolume,DialogueVolume,CameraSensitivity,TextureDetail,ShadowDetail,EffectsDetail,MeshDetail;
 unsigned long Antialiasing,Resolution,RefreshRate,ScreenAspectRatio;long ScoreBoardProxyPort,ScoreBoardServerPort;
 std::vector<int> AssignedInputsPassive,AssignedInputsAggressive,GivenTutorialCategories;std::vector<CWideString> SaveGameNames;
 void SaveGameState(CAFile&);
};
void CUserProfileManager::SaveGameState(CAFile& file)
{
    CCharString text;
    CPersistContext context(text);
    CCharString profileName = ProfileName.ToCharString();
    context.Transfer("ProfileName", profileName);
    context.Transfer("SubtitlingEnabled", SubtitlingEnabled);
    context.Transfer("VibrationEnabled", VibrationEnabled);
    context.Transfer("CameraXYInverted", CameraXYInverted);
    context.Transfer("CameraYZInverted", CameraYZInverted);
    context.Transfer("CameraFPBowXYInverted", CameraFPBowXYInverted);
    context.Transfer("CameraFPBowYZInverted", CameraFPBowYZInverted);
    context.Transfer("CameraPCBowXYInverted", CameraPCBowXYInverted);
    context.Transfer("CameraPCBowYZInverted", CameraPCBowYZInverted);
    context.Transfer("CameraPCViewHeroXYInverted", CameraPCViewHeroXYInverted);
    context.Transfer("CameraPCViewHeroYZInverted", CameraPCViewHeroYZInverted);
    context.Transfer("TutorialSystemEnabled", TutorialSystemEnabled);
    context.Transfer("GuildMasterEnabled", GuildMasterEnabled);
    context.Transfer("HUDTooltipEnabled", HUDTooltipEnabled);
    context.Transfer("Gamma", Gamma);
    context.Transfer("HUDAlpha", HUDAlpha);
    context.Transfer("SoundVolume", SoundVolume);
    context.Transfer("MusicVolume", MusicVolume);
    context.Transfer("DialogueVolume", DialogueVolume);
    context.Transfer("UsingHeadPhones", UsingHeadPhones);
    context.Transfer("ShowingBuddyNames", ShowingBuddyNames);
    context.Transfer("CameraSensitivity", CameraSensitivity);
    context.Transfer("CameraResetting", CameraResetting);
    context.Transfer("Antialiasing", Antialiasing);
    context.Transfer("TextureDetail", TextureDetail);
    context.Transfer("ShadowDetail", ShadowDetail);
    context.Transfer("EffectsDetail", EffectsDetail);
    context.Transfer("MeshDetail", MeshDetail);
    context.Transfer("Resolution", Resolution);
    context.Transfer("RefreshRate", RefreshRate);
    context.Transfer("ScreenAspectRatio", ScreenAspectRatio);
    context.Transfer("VerticalSyncEnabled", VerticalSyncEnabled);
    context.TransferVector("AssignedInputsPassive", AssignedInputsPassive);
    context.TransferVector("AssignedInputsAggressive", AssignedInputsAggressive);
    context.Transfer("ShowTargetingStatus", ShowTargetingStatus);
    context.Transfer("ScoreboardUserName", ScoreBoardUserName);
    context.Transfer("ScoreboardPassword", ScoreBoardPassword);
    context.Transfer("ScoreboardProxyIp", ScoreBoardProxyIp);
    context.Transfer("ScoreboardProxyPort", ScoreBoardProxyPort);
    context.Transfer("ScoreboardServerPort", ScoreBoardServerPort);
    context.TransferVector("GivenTutorialCategories", GivenTutorialCategories);
    for (long i = 0; i < 50; ++i) {
        CCharString saveName;
        saveName = SaveGameNames[i].ToCharString();
        context.Transfer("SaveGameNames" + GFIntToCharString(i + 1), saveName);
    }

    CMemoryDataOutputStream header;
    header.WriteString("FablePro");
    header.WriteULONG(0);
    header.WriteULONG(text.GetLength() + 0x10);
    CMemoryDataOutputStream stream;
    stream.ReserveSize(0x4000);
    stream.Write(header.PeekData(), header.GetLength());
    stream.WriteString(text);
    unsigned long crc = CCRC::Calc(0, header.PeekData(), header.GetLength());
    crc = CCRC::Calc(crc, text.GetAsCharArray(), text.GetLength());
    stream.WriteULONG(crc);
    unsigned long position = stream.GetPosition();
    if (position < 0x4000)
        stream.WriteRepeatedByte(' ', 0x4000 - position);
    file.Write(stream.PeekData(), stream.GetLength(), 0);
}

static unsigned int Read32(const std::vector<unsigned char>& x,unsigned int p){return (unsigned int)x[p]|((unsigned int)x[p+1]<<8)|((unsigned int)x[p+2]<<16)|((unsigned int)x[p+3]<<24);}
static unsigned int ReferenceCRC(unsigned int seed,const unsigned char* p,unsigned int n){for(unsigned int i=0;i<n;++i){seed^=p[i];for(int k=0;k<8;++k)seed=(seed&1)?(seed>>1)^0xedb88320u:seed>>1;}return seed;}
static int fail(int line){std::printf("00409fd0_TEST FAIL %d\n",line);return 1;}
#define CHECK(x) do{if(!(x))return fail(__LINE__);}while(0)
int main(){
 for(unsigned int i=0;i<256;++i){unsigned int c=i;for(int k=0;k<8;++k)c=(c&1)?(c>>1)^0xedb88320u:c>>1;g_CRCTable[i]=c;}
 for(int scenario=0;scenario<2;++scenario){CUserProfileManager p;p.ProfileName=CWideString("Hero");p.ScoreBoardUserName=CWideString("user");p.ScoreBoardPassword=CWideString("password");p.ScoreBoardProxyIp=CWideString("proxy");
 p.SubtitlingEnabled=true;p.VibrationEnabled=false;p.CameraXYInverted=true;p.CameraYZInverted=false;p.CameraFPBowXYInverted=true;p.CameraFPBowYZInverted=false;p.CameraPCBowXYInverted=true;p.CameraPCBowYZInverted=false;p.CameraPCViewHeroXYInverted=true;p.CameraPCViewHeroYZInverted=false;p.TutorialSystemEnabled=true;p.GuildMasterEnabled=false;p.HUDTooltipEnabled=true;p.UsingHeadPhones=false;p.ShowingBuddyNames=true;p.CameraResetting=false;p.VerticalSyncEnabled=true;p.ShowTargetingStatus=false;
 p.Gamma=0.125f;p.HUDAlpha=0.25f;p.SoundVolume=0.375f;p.MusicVolume=0.5f;p.DialogueVolume=0.625f;p.CameraSensitivity=0.75f;p.TextureDetail=0.875f;p.ShadowDetail=1.0f;p.EffectsDetail=1.125f;p.MeshDetail=1.25f;p.Antialiasing=2;p.Resolution=3;p.RefreshRate=1;p.ScreenAspectRatio=4;p.ScoreBoardProxyPort=3128;p.ScoreBoardServerPort=80;
 p.AssignedInputsPassive.push_back(7);p.AssignedInputsPassive.push_back(8);p.AssignedInputsAggressive.push_back(9);p.GivenTutorialCategories.push_back(3);
 for(int j=0;j<50;++j){char name[50];std::sprintf(name,"slot%d",j+1);p.SaveGameNames.push_back(CWideString(name));}if(scenario)p.SaveGameNames[49].value=std::string(18000,'x');
 records.erase(records.begin(),records.end());CAFile file;p.SaveGameState(file);CHECK(file.flag==0&&file.bytes.size()>=0x4000);CHECK(std::memcmp(&file.bytes[0],"FablePro",8)==0&&Read32(file.bytes,8)==0);
 const char* names[]={"ProfileName","SubtitlingEnabled","VibrationEnabled","CameraXYInverted","CameraYZInverted","CameraFPBowXYInverted","CameraFPBowYZInverted","CameraPCBowXYInverted","CameraPCBowYZInverted","CameraPCViewHeroXYInverted","CameraPCViewHeroYZInverted","TutorialSystemEnabled","GuildMasterEnabled","HUDTooltipEnabled","Gamma","HUDAlpha","SoundVolume","MusicVolume","DialogueVolume","UsingHeadPhones","ShowingBuddyNames","CameraSensitivity","CameraResetting","Antialiasing","TextureDetail","ShadowDetail","EffectsDetail","MeshDetail","Resolution","RefreshRate","ScreenAspectRatio","VerticalSyncEnabled","AssignedInputsPassive","AssignedInputsAggressive","ShowTargetingStatus","ScoreboardUserName","ScoreboardPassword","ScoreboardProxyIp","ScoreboardProxyPort","ScoreboardServerPort","GivenTutorialCategories"};
 const char* values[]={"Hero","1","0","1","0","1","0","1","0","1","0","1","0","1","0.125","0.250","0.375","0.500","0.625","0","1","0.750","0","2","0.875","1.000","1.125","1.250","3","1","4","1","7,8","9","0","user","password","proxy","3128","80","3"};
 CHECK(records.size()==91);std::string expected;for(unsigned int k=0;k<41;++k){CHECK(records[k].name==names[k]&&records[k].value==values[k]);expected+=std::string(names[k])+"="+values[k]+";";}
 for(unsigned int k=0;k<50;++k){char name[40];std::sprintf(name,"SaveGameNames%u",k+1);CHECK(records[41+k].name==name&&records[41+k].value==p.SaveGameNames[k].value);expected+=std::string(name)+"="+p.SaveGameNames[k].value+";";}
 unsigned int n=(unsigned int)expected.size();CHECK(Read32(file.bytes,12)==n+16);CHECK(std::memcmp(&file.bytes[16],expected.c_str(),n)==0);CHECK(Read32(file.bytes,16+n)==ReferenceCRC(0,&file.bytes[0],16+n));unsigned int logical=20+n;CHECK(file.bytes.size()==(logical<0x4000?0x4000:logical));for(unsigned int k=logical;k<file.bytes.size();++k)CHECK(file.bytes[k]==' ');
 CHECK(CCRC::Calc(12345,0,0)==12345);const unsigned char check[]="123456789";CHECK(CCRC::Calc(0,check,9)==ReferenceCRC(0,check,9));
 }
 std::printf("00409fd0_TEST PASS\n");return 0;
}