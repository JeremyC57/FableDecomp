#include <vector>
#include <string>
#include <stdio.h>
#include <wchar.h>
struct C2DExtentsI {
    long X,Y;
    C2DExtentsI(long x=0,long y=0):X(x),Y(y) {}
    void Initialise(long x,long y){X=x;Y=y;}
};
struct CDisplayModeDesc { C2DExtentsI Dimensions; long BitsPerPixel,RefreshRate; };
typedef std::wstring CWideString;
template<class T> struct CArray : std::vector<T> {};
struct CConfigOptionsDefaultsDef { long MinResolutionWidth,MinResolutionHeight; };
template<class T> struct CDefPointer {
    T* p;
    CDefPointer(T* x=0):p(x){}
    bool IsNull()const{return !p;}
    T* operator->()const{return p;}
};
struct CUserProfileManager;
static int step,failures;
static CConfigOptionsDefaultsDef* defaults;
static std::vector<CDisplayModeDesc> supplied;
static const CUserProfileManager* callback;
struct TestDisplay {
    void EnumerateDisplayModes(CArray<CDisplayModeDesc>& modes,C2DExtentsI min,C2DExtentsI max) {
        if(++step!=2||min.X!=(defaults?defaults->MinResolutionWidth:1024)||
           min.Y!=(defaults?defaults->MinResolutionHeight:768)||max.X!=32767||max.Y!=32767) ++failures;
        modes.assign(supplied.begin(),supplied.end());
    }
    void AddDeviceResetCallback(CUserProfileManager* owner){if(++step!=6)++failures;callback=owner;}
};
struct TestSystem {TestDisplay display;TestDisplay* GetDisplayManager(){return &display;}} systemStub;
static TestSystem* GFGetSystemManager(){return &systemStub;}
struct CUserProfileManager {
    bool UseNewProfileValues,SoundsMuted,ApplyVideoValuesOnLoad,DisplayDialogOnOptionsChanged;
    std::vector<CWideString> SaveGameNames;
    bool DisplayTutorialsDuringCutscenes,VideoChangesAccepted;
    std::vector<CDisplayModeDesc> DisplayModes;
    CWideString ProfileName;
    void* PBackBufferTexture;
    CUserProfileManager();
    CDefPointer<const CConfigOptionsDefaultsDef> GetConfigOptionsDefaultsDef() const {
        if(++step!=1)++failures;
        return CDefPointer<const CConfigOptionsDefaultsDef>(defaults);
    }
    void SetDefaultValues(){if(++step!=3)++failures;}
    void SetDefaultValuesForNewProfile(){if(++step!=4)++failures;}
    CWideString GetDefaultProfileName()const{if(++step!=5)++failures;return CWideString(L"Default");}
};

CUserProfileManager::CUserProfileManager()
    : UseNewProfileValues(false), SoundsMuted(false), ApplyVideoValuesOnLoad(true), DisplayDialogOnOptionsChanged(true),
      SaveGameNames(50), DisplayTutorialsDuringCutscenes(false), VideoChangesAccepted(true)
{
    CArray<CDisplayModeDesc> modes;
    CDefPointer<const CConfigOptionsDefaultsDef> pdefaults = GetConfigOptionsDefaultsDef();
    C2DExtentsI min_size(1024, 768);
    if (!pdefaults.IsNull())
        min_size.Initialise(pdefaults->MinResolutionWidth, pdefaults->MinResolutionHeight);
    GFGetSystemManager()->GetDisplayManager()->EnumerateDisplayModes(modes, min_size, C2DExtentsI(0x7fff, 0x7fff));
    CDisplayModeDesc previous_mode = *modes.begin();
    // FableWin 0187A03B retains the original for-scope compatibility guard.
    if (false) {} else for (CArray<CDisplayModeDesc>::const_iterator it = modes.begin(); it != modes.end(); ++it) {
        if (it->Dimensions.X != previous_mode.Dimensions.X || it->Dimensions.Y != previous_mode.Dimensions.Y || it->BitsPerPixel != previous_mode.BitsPerPixel) {
            DisplayModes.push_back(previous_mode);
            previous_mode = *it;
        } else {
            previous_mode = *it;
        }
    }
    if (!DisplayModes.empty()) {
        if (DisplayModes.back().Dimensions.X != previous_mode.Dimensions.X || DisplayModes.back().Dimensions.Y != previous_mode.Dimensions.Y || DisplayModes.back().BitsPerPixel != previous_mode.BitsPerPixel)
            DisplayModes.push_back(previous_mode);
    }
    SetDefaultValues();
    SetDefaultValuesForNewProfile();
    ProfileName = GetDefaultProfileName();
    PBackBufferTexture = 0;
    GFGetSystemManager()->GetDisplayManager()->AddDeviceResetCallback(this);
}

static bool sameSizeDepth(const CDisplayModeDesc& a,const CDisplayModeDesc& b) {
    return a.Dimensions.X==b.Dimensions.X && a.Dimensions.Y==b.Dimensions.Y && a.BitsPerPixel==b.BitsPerPixel;
}
static bool runCase() {
    step=0;callback=0;
    CUserProfileManager manager;
    if(step!=6||callback!=&manager||failures||manager.PBackBufferTexture||manager.ProfileName!=L"Default") return false;
    if(manager.UseNewProfileValues||manager.SoundsMuted||!manager.ApplyVideoValuesOnLoad||
       !manager.DisplayDialogOnOptionsChanged||manager.DisplayTutorialsDuringCutscenes||!manager.VideoChangesAccepted) return false;
    if(manager.SaveGameNames.size()!=50) return false;
    for(unsigned k=0;k<50;++k) if(!manager.SaveGameNames[k].empty())return false;
    std::vector<CDisplayModeDesc> expected;
    for(size_t i=0;i<supplied.size();++i)
        if(i+1==supplied.size()||!sameSizeDepth(supplied[i],supplied[i+1])) expected.push_back(supplied[i]);
    if(expected.size()==1) expected.clear();
    if(expected.size()!=manager.DisplayModes.size())return false;
    for(size_t i=0;i<expected.size();++i)
        if(!sameSizeDepth(expected[i],manager.DisplayModes[i])||expected[i].RefreshRate!=manager.DisplayModes[i].RefreshRate)return false;
    return true;
}
int main() {
    CConfigOptionsDefaultsDef settings={1280,720};
    CDisplayModeDesc list[4];
    for(int i=0;i<4;++i){list[i].Dimensions.Initialise(i==1?2048:1024,i==2?1024:768);list[i].BitsPerPixel=i==3?16:32;}
    int count=0;
    for(int def=0;def<2;++def)for(int length=1;length<=5;++length){
        defaults=def?&settings:0;
        const int combinations=1<<(length*2);
        for(int pattern=0;pattern<combinations;++pattern){
            supplied.clear();
            for(int i=0;i<length;++i){CDisplayModeDesc d=list[(pattern>>(2*i))&3];d.RefreshRate=50+i;supplied.push_back(d);}
            if(!runCase())return 1;
            ++count;
        }
    }
    if(count!=2728)return 2;
    puts("0040cec0_TEST PASS");return 0;
}