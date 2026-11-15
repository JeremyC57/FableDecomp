// Full SetDefaultValues body copied verbatim from the graded row; portable host fixture replaces external engine callees.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <string>
#include <cmath>
typedef std::wstring CWideString;
struct C2DExtentsI { long X,Y; C2DExtentsI(long x=0,long y=0):X(x),Y(y){} };
struct CDisplayModeDesc { C2DExtentsI Dimensions; long BitsPerPixel; CDisplayModeDesc(long x,long y,long d):Dimensions(x,y),BitsPerPixel(d){} };
struct CConfigOptionsDefaultsDef { unsigned long Antialiasing; long ResolutionWidth,ResolutionHeight,BitDepth; float TextureDetail,MaxTextureDetail,ShadowDetail,MaxShadowDetail,MeshDetail,MaxMeshDetail,EffectsDetail,MaxEffectsDetail; };
template<class T> class CDefPointer { T* p; public: CDefPointer(T* x=0):p(x){} bool IsNull()const{return p==0;} T* operator->()const{return p;} };
long g_MaxDetailA,g_MaxDetailB,g_MaxDetailC,g_MaxDetailD,g_DetailA,g_DetailB,g_DetailC,g_DetailD,g_MaxAntialiasing,g_MaxResolutionWidth,g_MaxRefreshRate,g_MemoryA,g_MemoryB;
const long g_refreshRateOptions[4]={60,75,85,100}; // Fable.exe .rdata0122DE30
static CConfigOptionsDefaultsDef* g_definition;
struct CDisplayManager { long width,height,depth,rate; long GetBitDepth(){return depth;} long GetRefreshRate(){return rate;} C2DExtentsI GetRenderTargetDimensions(){return C2DExtentsI(width,height);} };
struct CSystemManager { CDisplayManager display; CDisplayManager* GetDisplayManager(){return &display;} };
CSystemManager g_system; CSystemManager* GFGetSystemManager(){return &g_system;}
static std::string calls;
class CUserProfileManager {
public:
 CWideString ProfileName; bool SubtitlingEnabled,VibrationEnabled,CameraXYInverted,CameraYZInverted,CameraFPBowXYInverted,CameraFPBowYZInverted,CameraPCBowXYInverted,CameraPCBowYZInverted,CameraPCViewHeroXYInverted,CameraPCViewHeroYZInverted,TutorialSystemEnabled,GuildMasterEnabled,HUDTooltipEnabled,UsingHeadPhones,SoundsMuted,ShowingBuddyNames,ApplyVideoValuesOnLoad,ShowTargetingStatus,CameraResetting,VerticalSyncEnabled;
 float Gamma,HUDAlpha,SoundVolume,MusicVolume,DialogueVolume,CameraSensitivity,EffectsDetail,TextureDetail,MeshDetail,ShadowDetail;
 unsigned long Antialiasing,Resolution,RefreshRate,ScreenAspectRatio; long ScoreBoardProxyPort,ScoreBoardServerPort;
 std::vector<CDisplayModeDesc> DisplayModes;std::vector<int> GivenTutorialCategories;std::queue<int> PendingTutorialCategories;std::vector<CWideString> SaveGameNames;
 CDefPointer<const CConfigOptionsDefaultsDef> GetConfigOptionsDefaultsDef()const{calls+='D';return CDefPointer<const CConfigOptionsDefaultsDef>(g_definition);}
 const std::vector<CDisplayModeDesc>& GetDisplayModes()const{return DisplayModes;}
 static void ClampVideoOption(float& out,float value,float fallback,float cap,float second){
  if(value!=-1.0f){out=value;return;}if(fallback!=-1.0f)out=fallback;
  if(cap!=-1.0f){if(out>cap)out=cap;}else if(second!=-1.0f){if(out>second)out=second;}else out=0.0f;
 }
 void ClampAntialiasing(unsigned long& aa,long cap){if(cap!=-1 && aa>(unsigned long)cap)aa=cap;}
 void ClampResolution(unsigned long&,long);void ClampRefreshRate(unsigned long&,long);
 void ResetAssignedInputs(){calls+='R';}void ApplyValues(){calls+='A';}void ApplyVideoValues(){calls+='V';}void Save(){calls+='S';}
 void SetDefaultValues();
};
void CUserProfileManager::ClampResolution(unsigned long& resolution, long maxWidth)
{
    if (resolution >= DisplayModes.size())
        resolution = 0;
    if (maxWidth != -1) {
        if (DisplayModes[resolution].Dimensions.X > maxWidth) {
            unsigned long index = resolution;
            // Unsigned guard (always true) keeps the loop unrotated as in retail: cmp ; jle ; dec ; jmp at 0x004095C6..0x004095D2.
            while (index >= 0 && DisplayModes[index].Dimensions.X > maxWidth)
                --index;
            resolution = index;
        }
    }
}

// 0x004069A0 out of line (no landed row); expanded here at 0x004096CA..0x004096FC. Graded RELOCATION_MATCH against 0x004069A0.
void CUserProfileManager::ClampRefreshRate(unsigned long& refreshRate, long maxRefreshRate)
{
    if (refreshRate >= 4)
        refreshRate = 0;
    if (maxRefreshRate != -1) {
        if (g_refreshRateOptions[refreshRate] > maxRefreshRate) {
            unsigned long index = refreshRate;
            // Unsigned guard (always true) keeps the loop unrotated as in retail: cmp ; jle ; dec ; jmp at 0x004096F0..0x004096FA.
            while (index >= 0 && g_refreshRateOptions[index] > maxRefreshRate)
                --index;
            refreshRate = index;
        }
    }
}

void CUserProfileManager::SetDefaultValues()
{
    Gamma = 1.0f;
    HUDAlpha = 1.0f;
    SubtitlingEnabled = true;
    VibrationEnabled = true;
    CameraXYInverted = false;
    CameraYZInverted = false;
    CameraFPBowXYInverted = false;
    CameraFPBowYZInverted = false;
    CameraPCBowXYInverted = false;
    CameraPCBowYZInverted = false;
    CameraPCViewHeroXYInverted = false;
    CameraPCViewHeroYZInverted = false;
    TutorialSystemEnabled = true;
    GuildMasterEnabled = true;
    HUDTooltipEnabled = true;
    SoundVolume = 0.8f;
    MusicVolume = 0.6f;
    DialogueVolume = 0.9f;
    ShowingBuddyNames = false;
    SoundsMuted = false;
    UsingHeadPhones = false;
    ShowTargetingStatus = true;
    CameraResetting = true;
    CameraSensitivity = 0.5f;

    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (def.IsNull()) {
        ClampVideoOption(EffectsDetail, (float)g_DetailA, -1.0f, (float)g_MaxDetailA, -1.0f);
        ClampVideoOption(TextureDetail, (float)g_DetailD, -1.0f, (float)g_MaxDetailD, -1.0f);
        ClampVideoOption(MeshDetail, (float)g_DetailC, -1.0f, (float)g_MaxDetailC, -1.0f);
        ClampVideoOption(ShadowDetail, (float)g_DetailB, -1.0f, (float)g_MaxDetailB, -1.0f);
    } else {
        ClampVideoOption(EffectsDetail, (float)g_DetailA, def->EffectsDetail, (float)g_MaxDetailA, def->MaxEffectsDetail);
        ClampVideoOption(TextureDetail, (float)g_DetailD, def->TextureDetail, (float)g_MaxDetailD, def->MaxTextureDetail);
        ClampVideoOption(MeshDetail, (float)g_DetailC, def->MeshDetail, (float)g_MaxDetailC, def->MaxMeshDetail);
        ClampVideoOption(ShadowDetail, (float)g_DetailB, def->ShadowDetail, (float)g_MaxDetailB, def->MaxShadowDetail);
        Antialiasing = def->Antialiasing;
    }
    if (def.IsNull())
        Antialiasing = 0;
    ClampAntialiasing(Antialiasing, g_MaxAntialiasing);

    long width = -1;
    long height = -1;
    long bitDepth = GFGetSystemManager()->GetDisplayManager()->GetBitDepth();
    if ((long)g_MemoryA > 0)
        width = g_MemoryA;
    if ((long)g_MemoryB > 0)
        height = g_MemoryB;
    if (!def.IsNull()) {
        if (width == -1)
            width = def->ResolutionWidth;
        if (height == -1)
            height = def->ResolutionHeight;
        bitDepth = def->BitDepth;
    }
    if (width == -1)
        width = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().X;
    if (height == -1)
        height = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().Y;
    const std::vector<CDisplayModeDesc>& modes = GetDisplayModes();
    Resolution = 0;
    for (std::vector<CDisplayModeDesc>::const_iterator it = modes.begin(); it != modes.end(); ++it) {
        if (it->Dimensions.X == width && it->Dimensions.Y == height && it->BitsPerPixel == bitDepth)
            break;
        ++Resolution;
    }
    ClampResolution(Resolution, g_MaxResolutionWidth);

    long refreshRate = GFGetSystemManager()->GetDisplayManager()->GetRefreshRate();
    RefreshRate = 0;
    unsigned long i = 1;
    do {
        if (abs(refreshRate - g_refreshRateOptions[i]) < abs(refreshRate - g_refreshRateOptions[RefreshRate]) && g_refreshRateOptions[i] <= refreshRate)
            RefreshRate = i;
    } while (++i < 4);
    ClampRefreshRate(RefreshRate, g_MaxRefreshRate);

    ScreenAspectRatio = 0;
    VerticalSyncEnabled = true;
    ScoreBoardProxyPort = 0;
    ScoreBoardServerPort = 80;
    ResetAssignedInputs();
    GivenTutorialCategories.clear();
    while (!PendingTutorialCategories.empty())
        PendingTutorialCategories.pop();
    SaveGameNames.clear();
    SaveGameNames.resize(50, L"");
    ApplyValues();
    if (ApplyVideoValuesOnLoad)
        ApplyVideoValues();
    if (ProfileName != L"")
        Save();
}

static bool closef(float a,float b){return std::fabs(a-b)<0.00001f;}
static int fail(int line){std::printf("0040c790_TEST FAIL %d\n",line);return 1;}
#define CHECK(x) do{if(!(x))return fail(__LINE__);}while(0)
int main(){
 CConfigOptionsDefaultsDef def;def.Antialiasing=4;def.ResolutionWidth=1024;def.ResolutionHeight=768;def.BitDepth=32;def.TextureDetail=3;def.MaxTextureDetail=2;def.ShadowDetail=4;def.MaxShadowDetail=3;def.MeshDetail=5;def.MaxMeshDetail=4;def.EffectsDetail=6;def.MaxEffectsDetail=5;
 for(int scenario=0;scenario<8;++scenario){
  CUserProfileManager p; g_definition=(scenario&1)?&def:0;p.ProfileName=(scenario&2)?L"Profile":L"";p.ApplyVideoValuesOnLoad=(scenario&4)!=0;
  p.EffectsDetail=p.TextureDetail=p.MeshDetail=p.ShadowDetail=9;g_DetailA=g_DetailB=g_DetailC=g_DetailD=-1;g_MaxDetailA=g_MaxDetailB=g_MaxDetailC=g_MaxDetailD=-1;g_MaxAntialiasing=-1;g_MaxResolutionWidth=-1;g_MaxRefreshRate=-1;g_MemoryA=g_MemoryB=0;
  g_system.display.width=800;g_system.display.height=600;g_system.display.depth=32;g_system.display.rate=85;
  p.DisplayModes.push_back(CDisplayModeDesc(800,600,32));p.DisplayModes.push_back(CDisplayModeDesc(1024,768,32));p.DisplayModes.push_back(CDisplayModeDesc(1280,1024,32));p.GivenTutorialCategories.push_back(1);p.PendingTutorialCategories.push(1);p.PendingTutorialCategories.push(2);p.SaveGameNames.push_back(L"Old");calls="";p.SetDefaultValues();
  CHECK(p.Gamma==1 && p.HUDAlpha==1 && p.SubtitlingEnabled && p.VibrationEnabled && p.TutorialSystemEnabled && p.GuildMasterEnabled && p.HUDTooltipEnabled);CHECK(!p.CameraXYInverted&&!p.CameraYZInverted&&!p.CameraFPBowXYInverted&&!p.CameraFPBowYZInverted&&!p.CameraPCBowXYInverted&&!p.CameraPCBowYZInverted&&!p.CameraPCViewHeroXYInverted&&!p.CameraPCViewHeroYZInverted);CHECK(closef(p.SoundVolume,0.8f)&&closef(p.MusicVolume,0.6f)&&closef(p.DialogueVolume,0.9f));CHECK(!p.ShowingBuddyNames&&!p.SoundsMuted&&!p.UsingHeadPhones&&p.ShowTargetingStatus&&p.CameraResetting&&p.CameraSensitivity==0.5f);
  CHECK(p.Resolution==((scenario&1)?1u:0u)&&p.RefreshRate==2&&p.Antialiasing==((scenario&1)?4u:0u));CHECK(p.EffectsDetail==((scenario&1)?5.0f:0.0f)&&p.TextureDetail==((scenario&1)?2.0f:0.0f)&&p.MeshDetail==((scenario&1)?4.0f:0.0f)&&p.ShadowDetail==((scenario&1)?3.0f:0.0f));CHECK(p.ScreenAspectRatio==0&&p.VerticalSyncEnabled&&p.ScoreBoardProxyPort==0&&p.ScoreBoardServerPort==80);CHECK(p.GivenTutorialCategories.empty()&&p.PendingTutorialCategories.empty()&&p.SaveGameNames.size()==50);for(unsigned int j=0;j<50;++j)CHECK(p.SaveGameNames[j].empty());std::string expected="DRA";if(scenario&4)expected+='V';if(scenario&2)expected+='S';CHECK(calls==expected);
  // Exercise config overrides and both cap walk-downs using the full body again.
  g_MemoryA=1280;g_MemoryB=1024;g_MaxResolutionWidth=1024;g_MaxRefreshRate=75;g_MaxAntialiasing=2;g_DetailA=7;g_MaxDetailD=1;p.SetDefaultValues();CHECK(p.Resolution==1&&p.RefreshRate==1&&p.Antialiasing==((scenario&1)?2u:0u)&&p.EffectsDetail==7&&p.TextureDetail==((scenario&1)?1.0f:0.0f));
 }
 std::printf("0040c790_TEST PASS\n");return 0;
}