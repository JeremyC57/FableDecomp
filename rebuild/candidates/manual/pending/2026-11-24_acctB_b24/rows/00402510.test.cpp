// Production GFMain body copied verbatim; collaborators expose startup decisions and calls.
// Native-only 32-bit sound configuration words use unsigned int in this host-portable fixture.
#include <stdio.h>
#include <assert.h>
#include <string>
#include <vector>
#include <string.h>
#define __fastcall
struct HINSTANCE__{};struct HWND__{};typedef HWND__*HWND;typedef void*HANDLE;
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define FILE_FLAG_BACKUP_SEMANTICS 0x02000000
struct _SECURITY_ATTRIBUTES;struct tagMSG;
class CWideString{public:std::wstring text;CWideString(){}CWideString(const wchar_t*s):text(s){}CWideString(const std::wstring&s):text(s){}const wchar_t*GetAsCharArray()const{return text.c_str();}bool operator!=(const wchar_t*s)const{return text!=s;}};
class CCharString{public:std::string text;CCharString(){}CCharString(const char*s):text(s){}CCharString(const std::string&s):text(s){}CWideString ToWideString()const{std::wstring out;for(unsigned i=0;i<text.size();++i)out+=(wchar_t)text[i];return CWideString(out);}void LoadFromFile(class CAFile&,void*,long);unsigned long GetCRC()const{assert(text=="TEXT_GUI_WINLOGO_ERROR_CAPTION");return 300;}};
CWideString operator+(const CWideString&a,const CWideString&b){return CWideString(a.text+b.text);}CWideString operator+(const CWideString&a,const wchar_t*b){return CWideString(a.text+b);}CWideString operator+(const wchar_t*a,const CWideString&b){return CWideString(a+b.text);}
struct BankOpen{std::wstring path;bool resident;};
static unsigned mode=0;static bool setting_file,open_file,user_exists,saves_exists,eula_ok,config_ok,game_init;static unsigned error_path,error_crc,system_code;
static unsigned file_reads,parser_count,persist_count,ime_loads,ime_inits,ime_shutdowns,console_count,latch_count,installer_sets,installer_releases,table_loads,close_count,message_count,config_count,free_config_count,eula_count,met_count,game_plays,game_uninits,initialise_count,handle_error_count,registry_count,registry_writes,profile_sets;
static bool text_entry_exists,caption_entry_exists,table_readonly,alias_retail;static std::wstring message_text,message_caption,ini_file,header_dir,table_filename,compiled_directory;
static std::vector<std::string>stages;static std::vector<BankOpen>banks;static std::vector<std::string>aliases;static std::vector<std::wstring>permission_paths;
static bool class_left,class_wrap,font_fixed,async_failure;
void __fastcall DoDirtyDisc(){}void __fastcall GFHardDiskError(const char*){}
enum ESystemCode{SYS_OK=1};bool __fastcall GFIsSystemErrorFailure(ESystemCode){return false;}
bool GAllowWADGeneration,GUseCompiledSoundSymbols,GRunFromDVD,GPreventInstallation,GIgnoreTimestampOnInstall,GStartVTuneDisabled,GUseAsyncLoadingMemoryHandler;
bool GLeftAlignText,GNoHangulWordWrap,GDisableCapsLock,GUseRetailBanks,GSkipFrontend,GUseCompiledDefs,GForcePrimary,GSoundOn;
long GResolutionWidth,GResolutionHeight,GResolutionDepth,GZBufferDepth,GResolutionRefreshRate,GPushBufferSize,GKickoffSize;
bool GPresentImmidiate,GAntialiasingOn,GAntialiasing9x,GFullScreen,GAllowBackgroundProcessing,GSkipConfigDetection,GOnlyBuildStaticMaps;
bool StartupConsoleEnabled_0137548f,SaveSoundSymbols_0138dd3b,LoadSoundSymbols_013bc9f0,PrepareParticles_01375754,RenderConfigurationFlag1_013750a4,RenderConfigurationFlag2_013750aa,UseFixedFontFunction_013750ab;
namespace NProfileTimer{void StartProfile(const CCharString&s,long n){assert(n==0);stages.push_back(s.text);}void EndProfile(long n){assert(n==0);}}
namespace NLocalisation{CCharString GetLanguageFolder(){return CCharString("english");}CCharString GetTextBankName(){return CCharString("TEXT_BANK");}CCharString GetStreamingFontBankName(){return CCharString("FONT_BANK");}}
namespace NResourceDirectoryNames{CWideString GetGraphicBankDir(){return L"graphics/";}CWideString GetFontBankDir(){return L"fonts/";}CWideString GetHDDialogueDir(){return L"dialogue/";}CWideString GetHDMiscDir(){return L"hdmisc/";}CWideString GetMiscDir(){return L"misc/";}CWideString GetShaderBankDir(){return L"shader/";}CWideString GetCacheDir(){return L"cache/";}CWideString GetMyDocumentsDir(){return L"docs/";}CWideString GetUserDataDir(){return L"user/";}CWideString GetSaveDir(){return L"saves/";}}
namespace NGlobalConsole{void Initialise(){++console_count;}}
namespace EULA{int DoEULAThings(){++eula_count;return eula_ok;}}
void ClearStartupLatch_009d81e0(){++latch_count;}
class CAFile{public:static void(*error)();static void(*hard)(const char*);static void SetErrorCallback(void(*f)()){error=f;}static void SetHardDiskErrorCallback(void(*f)(const char*)){hard=f;}static CWideString GetProjectPath(){return L"project/";}static void SetCurrentPath(const CWideString&s){assert(s.text==L"project/");}static bool PathExists(const CWideString&s){assert(s.text==L"Data\\lang\\english\\lang_settings.txt");return setting_file;}};void(*CAFile::error)()=0;void(*CAFile::hard)(const char*)=0;
enum FileAccessType{FILE_ACCESS_READ=1};class CDiskFileWin32:public CAFile{public:CDiskFileWin32(const CWideString&s,FileAccessType a,unsigned long n){assert(s.text==L"Data\\lang\\english\\lang_settings.txt"&&a==FILE_ACCESS_READ&&n==2);}bool IsOpen()const{return open_file;}};
void CCharString::LoadFromFile(CAFile&,void*p,long n){assert(p==0&&n==-1);++file_reads;text="settings";}
class CStringParser{public:CStringParser(const CCharString&s,int n,const CCharString&def,int x){assert(s.text=="settings"&&n==0&&def.text==""&&x==0);++parser_count;}};
class CPersistContext{public:CPersistContext(CStringParser&,int a,bool b,int c,int d,int e){assert(a==0&&b&&c==0&&d==0&&e==0);++persist_count;}void Transfer(const char*name,bool&value,bool def){assert(!def);if(strcmp(name,"LeftAlignText")==0)value=(mode&1)!=0;else if(strcmp(name,"NoHangulWordWrap")==0)value=(mode&2)!=0;else{assert(strcmp(name,"DisableCapsLock")==0);value=(mode&4)!=0;}}};
class CThreadedFileManager{public:static void(*error)();static void(*hard)(const char*);static void SetErrorCallback(void(*f)()){error=f;}static void SetHardDiskErrorCallback(void(*f)(const char*)){hard=f;}};void(*CThreadedFileManager::error)()=0;void(*CThreadedFileManager::hard)(const char*)=0;
class CFileInstaller{public:static CFileInstaller&Get(){static CFileInstaller x;return x;}void SetIgnoreTimeStampWhenCopying(bool b){assert(b==GIgnoreTimestampOnInstall);++installer_sets;}static void Release(){++installer_releases;}};
class CBankFileAsync{public:static void SetEnableFailureHandling(bool b){async_failure=b;}};
class CBankFileManager{public:void SetUsingRetailBanks(bool b){alias_retail=b;}void SetBankAlias(const CCharString&a,const CCharString&b){aliases.push_back(a.text+"="+b.text);}void OpenRetailBank(const CWideString&s,bool r){BankOpen x;x.path=s.text;x.resident=r;banks.push_back(x);}void OpenIniFile(const CWideString&s){ini_file=s.text;}};
namespace NBankFileManager{CBankFileManager GBankFileManager;void SetHeaderDir(const CWideString&s){header_dir=s.text;}}
class CDefinitionManager{public:static bool create,checks,use;static void SetCreatingCompiledDefs(bool b){create=b;}static void SetCreateChecksums(bool b){checks=b;}static void SetUsingCompiledDefs(bool b){use=b;}static void SetCompiledDefDirectory(const CWideString&s){compiled_directory=s.text;}};bool CDefinitionManager::create=false,CDefinitionManager::checks=false,CDefinitionManager::use=false;
class CDefStringTable{public:void SetTableFilename(const CWideString&s){table_filename=s.text;}void LoadTable(bool b){table_readonly=b;++table_loads;}}GDefStringTable;
template<class T>class CIVCountedPointer{T*p;public:CIVCountedPointer(T*x=0):p(x){}bool IsNull()const{return p==0;}T*operator->()const{return p;}};
template<class T>class CCountedPointer{T*p;CCountedPointer(const CCountedPointer&);public:CCountedPointer():p(0){}~CCountedPointer(){delete p;}void Reset(T*x){delete p;p=x;}T*operator->()const{return p;}};
namespace NGameText{class CTextEntry{public:bool caption;CWideString GetText()const{return caption?L"caption":L"error";}};class CDataBank:public CBankFileAsync{public:void OpenReadOnly(const CCharString&s,unsigned long n){assert(s.text=="TEXT_BANK"&&n==0x100);}unsigned long FindIndexBySymbolCRC(unsigned long c)const{if(c==300)return caption_entry_exists?2:0;assert(c==error_crc);return text_entry_exists?1:0;}CIVCountedPointer<const CTextEntry>GetPTextEntry(unsigned long n,bool b)const{assert(b&&(n==1||n==2));static CTextEntry a,c;a.caption=false;c.caption=true;return CIVCountedPointer<const CTextEntry>(n==1?&a:&c);}};}
class CXMVPlayer{public:static void SetLeftAlignText(bool b){class_left=b;}static void SetNoHangulWordWrap(bool b){class_wrap=b;}};
class CStreamingFontBank{public:static void SetUseFixedFunction(bool b){font_fixed=b;}};
namespace NUISystem{class CIME{public:static CIME&Get(){static CIME x;return x;}static void LoadIMESettings(CPersistContext&){++ime_loads;}static bool ProcessWinMessage(HWND,unsigned int,unsigned int,long,long&){return false;}static bool FilterMessage(tagMSG*){return false;}void Initialise(HWND h){assert(h==(HWND)8);++ime_inits;}};}
enum ESurfaceMultisampleType{SURFACE_MULTISAMPLE_NONE=0,SURFACE_MULTISAMPLE_4=4,SURFACE_MULTISAMPLE_9=9};
class CInputManagerInit{public:bool UseJoystick,UseKeyboard,UseMouse;CInputManagerInit():UseJoystick(false),UseKeyboard(false),UseMouse(false){}};
class CDisplayManagerInit{public:long Depth,Width,Height,RefreshRate,PushBufferSize,KickOffSize;unsigned char ZBufferDepth,NoBackBuffers;bool PresentImmediate,UseRenderManager;CWideString ShaderResourcePath;ESurfaceMultisampleType MultisampleType;unsigned long RenderManagerInitFlags;CDisplayManagerInit():Depth(0),Width(0),Height(0),RefreshRate(0),PushBufferSize(0),KickOffSize(0),ZBufferDepth(0),NoBackBuffers(0),PresentImmediate(false),UseRenderManager(false),MultisampleType(SURFACE_MULTISAMPLE_NONE),RenderManagerInitFlags(0){}};
class CSystemManagerInit{public:HINSTANCE__*InstanceHandle;char*CommandLine;int WindowsShow;CWideString AppName,TempPath;bool ExclusiveMode,PreferPrimaryDisplayDevice,WaitWhileInactive,SkipConfigDetection;unsigned long Flags;CCharString StreamingFontBankHandle;unsigned int Body_38[8];CInputManagerInit InputManagerInit;CDisplayManagerInit DisplayManagerInit;bool(*IsErrorFailureFunc)(ESystemCode);bool(*IMEMessageFunc)(HWND,unsigned int,unsigned int,long,long&);bool(*MessageFilter)(tagMSG*);CSystemManagerInit():InstanceHandle(0),CommandLine(0),WindowsShow(0),ExclusiveMode(false),PreferPrimaryDisplayDevice(false),WaitWhileInactive(false),SkipConfigDetection(false),Flags(0),IsErrorFailureFunc(0),IMEMessageFunc(0),MessageFilter(0){memset(Body_38,0,sizeof(Body_38));}};
static CSystemManagerInit captured;
class CSystemManager{public:HWND GetApplicationWinHandle()const{return (HWND)8;}ESystemCode Initialise(const CSystemManagerInit*s){captured=*s;++initialise_count;return (ESystemCode)system_code;}};
CSystemManager*GFGetSystemManager(){static CSystemManager x;return &x;}
class CUserProfileManager{public:static CUserProfileManager&Get(){static CUserProfileManager x;return x;}void SetProfileName(const CWideString&,bool){++profile_sets;}};
class CMainGameComponent{public:static void GenerateMetFilesFromLugFiles(){++met_count;}};
class CSystemRegistry{public:static CWideString GAME_REG_KEY,GFX_RESET_KEY_NAME;CSystemRegistry(const CWideString&s,long n){assert(s.text==L"game"&&n==0);++registry_count;}void WriteBOOL(const CWideString&s,long n){assert(s.text==L"gfx"&&n==0);++registry_writes;}};CWideString CSystemRegistry::GAME_REG_KEY=L"game";CWideString CSystemRegistry::GFX_RESET_KEY_NAME=L"gfx";
class CGame{public:void Play(){++game_plays;}}GGame;
bool GFInitialise(){return game_init;}void GFUninitialise(){++game_uninits;}void GFHandleSystemInitError(ESystemCode c){assert(c==(ESystemCode)system_code);++handle_error_count;}
CWideString GetWindowTitle(){return L"Fable test";}bool GFConfigDetection(){++config_count;return config_ok;}bool GFFreeConfigDetection(){++free_config_count;return true;}
HANDLE CreateFileW(const wchar_t*p,unsigned long a,unsigned long b,_SECURITY_ATTRIBUTES*s,unsigned long c,unsigned long d,HANDLE h){assert(a==1&&b==1&&s==0&&c==3&&d==FILE_FLAG_BACKUP_SEMANTICS&&h==0);bool found=std::wstring(p)==L"user/"?user_exists:saves_exists;assert(std::wstring(p)==L"user/"||std::wstring(p)==L"saves/");return found?(HANDLE)1:INVALID_HANDLE_VALUE;}
int CloseHandle(HANDLE h){assert(h==(HANDLE)1);++close_count;return 1;}
unsigned long TestDirForValidFilePermissions(const CWideString&s){permission_paths.push_back(s.text);unsigned id=s.text==L"user/"?1:s.text==L"saves/"?2:3;return id==error_path?error_crc:0;}
int MessageBoxW(HWND h,const wchar_t*text,const wchar_t*caption,unsigned int flags){assert(h==(HWND)8&&flags==0);++message_count;message_text=text;message_caption=caption;return 0;}
struct ExitResult{int code;ExitResult(int n):code(n){}};void fixture_exit(int n){throw ExitResult(n);}
#define exit fixture_exit

long __fastcall GFMain(HINSTANCE__*instance_handle,char*command_line,int show)
{
 NProfileTimer::StartProfile("Parse Command Line",0);
 CSystemManagerInit sys_init;
 CWideString launched_profile_name;
 CAFile::SetErrorCallback(DoDirtyDisc);CThreadedFileManager::SetErrorCallback(DoDirtyDisc);
 CAFile::SetHardDiskErrorCallback(GFHardDiskError);CThreadedFileManager::SetHardDiskErrorCallback(GFHardDiskError);
 CAFile::SetCurrentPath(CAFile::GetProjectPath());
 if(StartupConsoleEnabled_0137548f)NGlobalConsole::Initialise();
 NProfileTimer::EndProfile(0);
 NProfileTimer::StartProfile("Setup Basic install files",0);
 CDefinitionManager::SetCreatingCompiledDefs(GAllowWADGeneration);
 CDefinitionManager::SetCreateChecksums(GAllowWADGeneration);
 SaveSoundSymbols_0138dd3b=GAllowWADGeneration;
 LoadSoundSymbols_013bc9f0=GUseCompiledSoundSymbols;
 if(GRunFromDVD&&!GPreventInstallation)CFileInstaller::Get().SetIgnoreTimeStampWhenCopying(GIgnoreTimestampOnInstall);
 if(GStartVTuneDisabled)ClearStartupLatch_009d81e0();
 CBankFileAsync::SetEnableFailureHandling(GUseAsyncLoadingMemoryHandler);
 CFileInstaller::Release();
 NProfileTimer::EndProfile(0);
 NProfileTimer::StartProfile("Setup Language",0);
 CWideString lang_settings_filename=L"Data\\lang\\"+NLocalisation::GetLanguageFolder().ToWideString()+L"\\lang_settings.txt";
 if(CAFile::PathExists(lang_settings_filename)){
  CDiskFileWin32 file(lang_settings_filename,FILE_ACCESS_READ,2);
  if(file.IsOpen()){
   CCharString script;script.LoadFromFile(file,0,-1);
   CStringParser parser(script,0,CCharString(""),0);
   CPersistContext context(parser,0,true,0,0,0);
   context.Transfer("LeftAlignText",GLeftAlignText,false);
   context.Transfer("NoHangulWordWrap",GNoHangulWordWrap,false);
   context.Transfer("DisableCapsLock",GDisableCapsLock,false);
   NUISystem::CIME::LoadIMESettings(context);
  }
 }
 CXMVPlayer::SetLeftAlignText(GLeftAlignText);CXMVPlayer::SetNoHangulWordWrap(GNoHangulWordWrap);
 NProfileTimer::EndProfile(0);
 NProfileTimer::StartProfile("Setup basic retail banks",0);
 NBankFileManager::GBankFileManager.SetUsingRetailBanks(GUseRetailBanks);
 NBankFileManager::GBankFileManager.SetBankAlias("GBANK_MAIN_PC","GBANK_MAIN");
 NBankFileManager::GBankFileManager.SetBankAlias("GBANK_GUI_PC","GBANK_GUI");
 NBankFileManager::GBankFileManager.SetBankAlias("GBANK_FRONT_END_PC","GBANK_FRONT_END");
 NBankFileManager::GBankFileManager.SetBankAlias("PARTICLE_MAIN_PC","PARTICLE_MAIN");
 NBankFileManager::GBankFileManager.SetBankAlias("PARTICLE_FRONTEND_PC","PARTICLE_FRONTEND");
 CWideString compiled_def_dir;
 using namespace NResourceDirectoryNames;
 if(GUseRetailBanks){
  if(GSkipFrontend){
   CWideString platform_dir=L"pc\\";
   NBankFileManager::GBankFileManager.OpenRetailBank(GetGraphicBankDir()+L"graphics.big",true);
   NBankFileManager::GBankFileManager.OpenRetailBank(GetGraphicBankDir()+platform_dir+L"textures.big",true);
   NBankFileManager::GBankFileManager.OpenRetailBank(GetHDDialogueDir()+L"dialogue.big",false);
   NBankFileManager::GBankFileManager.OpenRetailBank(GetHDMiscDir()+platform_dir+L"effects.big",false);
   NBankFileManager::GBankFileManager.OpenRetailBank(GetHDDialogueDir()+L"text.big",false);
   NBankFileManager::GBankFileManager.OpenRetailBank(GetMiscDir()+L"temp.big",false);
   NBankFileManager::GBankFileManager.OpenRetailBank(GetShaderBankDir()+platform_dir+L"shaders.big",false);
  }
  NBankFileManager::GBankFileManager.OpenRetailBank(GetFontBankDir()+L"fonts.big",true);
  NBankFileManager::GBankFileManager.OpenRetailBank(GetHDDialogueDir()+L"text.big",false);
  NBankFileManager::SetHeaderDir(L"data\\defs\\RetailHeaders\\");
  compiled_def_dir=L"Data\\CompiledDefs\\";
 }else{
  if(GRunFromDVD)NBankFileManager::GBankFileManager.OpenIniFile(L"banks_dvd.ini");
  else NBankFileManager::GBankFileManager.OpenIniFile(L"banks.ini");
  NBankFileManager::SetHeaderDir(L"data\\defs\\DevHeaders\\");
  compiled_def_dir=L"Data\\CompiledDefs\\Development\\";
 }
 NProfileTimer::EndProfile(0);
 NProfileTimer::StartProfile("Setup definition manager table",0);
 CDefinitionManager::SetCompiledDefDirectory(compiled_def_dir);
 GDefStringTable.SetTableFilename(compiled_def_dir+L"names.bin");
 CDefinitionManager::SetUsingCompiledDefs(GUseCompiledDefs);
 if(GUseCompiledDefs)GDefStringTable.LoadTable(!GAllowWADGeneration);
 NProfileTimer::EndProfile(0);
 HANDLE pfabledirhandle=CreateFileW(GetUserDataDir().GetAsCharArray(),1,1,0,3,FILE_FLAG_BACKUP_SEMANTICS,0);
 unsigned long errorCRC;
 if(pfabledirhandle!=INVALID_HANDLE_VALUE){
  CloseHandle(pfabledirhandle);errorCRC=TestDirForValidFilePermissions(GetUserDataDir());
  HANDLE pfablesavesdirhandle=CreateFileW(GetSaveDir().GetAsCharArray(),1,1,0,3,FILE_FLAG_BACKUP_SEMANTICS,0);
  if(pfablesavesdirhandle!=INVALID_HANDLE_VALUE){CloseHandle(pfablesavesdirhandle);errorCRC=TestDirForValidFilePermissions(GetSaveDir().GetAsCharArray());}
 }else errorCRC=TestDirForValidFilePermissions(GetMyDocumentsDir());
 if(errorCRC>0){
  CCountedPointer<NGameText::CDataBank>pTextBank;pTextBank.Reset(new NGameText::CDataBank);
  pTextBank->OpenReadOnly(NLocalisation::GetTextBankName(),0x100);
  CWideString msg;unsigned long errorIndex=pTextBank->FindIndexBySymbolCRC(errorCRC);
  if(errorIndex>0){CIVCountedPointer<const NGameText::CTextEntry>ptext_entry=pTextBank->GetPTextEntry(errorIndex,true);if(!ptext_entry.IsNull())msg=ptext_entry->GetText();}
  CWideString caption;CCharString captionSymbol="TEXT_GUI_WINLOGO_ERROR_CAPTION";
  unsigned long captionIndex=pTextBank->FindIndexBySymbolCRC(captionSymbol.GetCRC());
  if(captionIndex>0){CIVCountedPointer<const NGameText::CTextEntry>ptext_entry=pTextBank->GetPTextEntry(captionIndex,true);if(!ptext_entry.IsNull())caption=ptext_entry->GetText();}
  MessageBoxW(GFGetSystemManager()->GetApplicationWinHandle(),msg.GetAsCharArray(),caption.GetAsCharArray(),0);
  return 1;
 }
 NProfileTimer::StartProfile("Setup library",0);
 sys_init.InstanceHandle=instance_handle;sys_init.CommandLine=command_line;sys_init.WindowsShow=show;sys_init.Flags=5;sys_init.PreferPrimaryDisplayDevice=GForcePrimary;
 if(GSoundOn==true){sys_init.Flags=21;reinterpret_cast<float*>(sys_init.Body_38)[2]=32.0f;reinterpret_cast<float*>(sys_init.Body_38)[3]=5.0f;reinterpret_cast<float*>(sys_init.Body_38)[4]=1.0f;sys_init.Body_38[5]=0x100;sys_init.Body_38[7]=0x10000;}
 sys_init.DisplayManagerInit.Depth=GResolutionDepth;sys_init.DisplayManagerInit.Width=GResolutionWidth;sys_init.DisplayManagerInit.Height=GResolutionHeight;
 sys_init.DisplayManagerInit.ZBufferDepth=(unsigned char)GZBufferDepth;sys_init.DisplayManagerInit.RefreshRate=GResolutionRefreshRate;
 sys_init.DisplayManagerInit.PresentImmediate=GPresentImmidiate;sys_init.DisplayManagerInit.PushBufferSize=GPushBufferSize;sys_init.DisplayManagerInit.KickOffSize=GKickoffSize;
 sys_init.DisplayManagerInit.ShaderResourcePath=GetShaderBankDir();
 if(GAntialiasingOn)sys_init.DisplayManagerInit.MultisampleType=GAntialiasing9x?SURFACE_MULTISAMPLE_9:SURFACE_MULTISAMPLE_4;
 sys_init.DisplayManagerInit.UseRenderManager=true;sys_init.DisplayManagerInit.NoBackBuffers=1;
 sys_init.IsErrorFailureFunc=GFIsSystemErrorFailure;sys_init.AppName=GetWindowTitle();sys_init.ExclusiveMode=GFullScreen;
 sys_init.TempPath=GetCacheDir();sys_init.StreamingFontBankHandle=NLocalisation::GetStreamingFontBankName();
 sys_init.WaitWhileInactive=!GAllowBackgroundProcessing;
 sys_init.InputManagerInit.UseJoystick=true;sys_init.InputManagerInit.UseKeyboard=true;sys_init.InputManagerInit.UseMouse=true;sys_init.SkipConfigDetection=GSkipConfigDetection;
 if(GOnlyBuildStaticMaps){sys_init.SkipConfigDetection=true;PrepareParticles_01375754=false;}
 else{
  if(!EULA::DoEULAThings())exit(1);
  if(!sys_init.SkipConfigDetection&&!GOnlyBuildStaticMaps){if(!GFConfigDetection())exit(1);GFFreeConfigDetection();}
 }
 if(RenderConfigurationFlag1_013750a4)sys_init.DisplayManagerInit.RenderManagerInitFlags|=1;
 if(!RenderConfigurationFlag2_013750aa)sys_init.DisplayManagerInit.RenderManagerInitFlags|=2;
 if(UseFixedFontFunction_013750ab)CStreamingFontBank::SetUseFixedFunction(true);
 sys_init.IMEMessageFunc=NUISystem::CIME::ProcessWinMessage;sys_init.MessageFilter=NUISystem::CIME::FilterMessage;
 ESystemCode code=GFGetSystemManager()->Initialise(&sys_init);
 NProfileTimer::EndProfile(0);
 NUISystem::CIME::Get().Initialise(GFGetSystemManager()->GetApplicationWinHandle());
 NProfileTimer::StartProfile("End basic init",0);
 if(GAllowWADGeneration)CMainGameComponent::GenerateMetFilesFromLugFiles();
 if(code==SYS_OK){
  if(GFInitialise()){
   if(launched_profile_name!=L"")CUserProfileManager::Get().SetProfileName(launched_profile_name,true);
   NProfileTimer::EndProfile(0);GGame.Play();GFUninitialise();
  }
 }else GFHandleSystemInitError(code);
 NProfileTimer::EndProfile(0);
 {CSystemRegistry registry(CSystemRegistry::GAME_REG_KEY,0);registry.WriteBOOL(CSystemRegistry::GFX_RESET_KEY_NAME,0);}
 return 0;
}


#undef exit
static void reset(unsigned n){mode=n;setting_file=(n&1)!=0;open_file=(n&2)!=0;user_exists=(n&4)!=0;saves_exists=(n&8)!=0;eula_ok=(n%7)!=0;config_ok=(n%11)!=0;game_init=(n%5)!=0;system_code=n%13==0?4:1;error_path=n%17==0?1:n%19==0?2:n%23==0?3:0;error_crc=200;text_entry_exists=(n&1)!=0;caption_entry_exists=(n&2)!=0;
file_reads=parser_count=persist_count=ime_loads=ime_inits=ime_shutdowns=console_count=latch_count=installer_sets=installer_releases=table_loads=close_count=message_count=config_count=free_config_count=eula_count=met_count=game_plays=game_uninits=initialise_count=handle_error_count=registry_count=registry_writes=profile_sets=0;stages.clear();banks.clear();aliases.clear();permission_paths.clear();message_text.erase();message_caption.erase();ini_file.erase();font_fixed=false;class_left=class_wrap=false;table_readonly=false;
GAllowWADGeneration=(n&1)!=0;GUseCompiledSoundSymbols=(n&2)!=0;GRunFromDVD=(n&4)!=0;GPreventInstallation=(n&8)!=0;GIgnoreTimestampOnInstall=(n&16)!=0;GStartVTuneDisabled=(n&32)!=0;GUseAsyncLoadingMemoryHandler=(n&64)!=0;GLeftAlignText=GNoHangulWordWrap=GDisableCapsLock=false;
GUseRetailBanks=(n&16)!=0;GSkipFrontend=(n&32)!=0;GUseCompiledDefs=(n&64)!=0;GForcePrimary=(n&128)!=0;GSoundOn=(n&256)!=0;GResolutionWidth=800;GResolutionHeight=600;GResolutionDepth=32;GZBufferDepth=24;GResolutionRefreshRate=60;GPushBufferSize=123;GKickoffSize=45;GPresentImmidiate=(n&1)!=0;GAntialiasingOn=(n&2)!=0;GAntialiasing9x=(n&4)!=0;GFullScreen=(n&8)!=0;GAllowBackgroundProcessing=(n&16)!=0;GSkipConfigDetection=(n&32)!=0;GOnlyBuildStaticMaps=(n&64)!=0;StartupConsoleEnabled_0137548f=(n&128)!=0;PrepareParticles_01375754=true;RenderConfigurationFlag1_013750a4=(n&256)!=0;RenderConfigurationFlag2_013750aa=(n&512)!=0;UseFixedFontFunction_013750ab=(n&1024)!=0;SaveSoundSymbols_0138dd3b=false;LoadSoundSymbols_013bc9f0=false;}
int main(){for(unsigned n=0;n<2048;++n){reset(n);HINSTANCE__ instance;char cmd[]="-test";long result=-1;bool exited=false;try{result=GFMain(&instance,cmd,3);}catch(const ExitResult&e){assert(e.code==1);exited=true;}
assert(CAFile::error==DoDirtyDisc&&CThreadedFileManager::error==DoDirtyDisc&&CAFile::hard==GFHardDiskError&&CThreadedFileManager::hard==GFHardDiskError);assert(console_count==(unsigned)StartupConsoleEnabled_0137548f);assert(latch_count==(unsigned)GStartVTuneDisabled);assert(installer_sets==(unsigned)(GRunFromDVD&&!GPreventInstallation)&&installer_releases==1);assert(async_failure==GUseAsyncLoadingMemoryHandler);assert(CDefinitionManager::create==GAllowWADGeneration&&CDefinitionManager::checks==GAllowWADGeneration);assert(SaveSoundSymbols_0138dd3b==GAllowWADGeneration&&LoadSoundSymbols_013bc9f0==GUseCompiledSoundSymbols);bool loaded=setting_file&&open_file;assert(file_reads==(unsigned)loaded&&parser_count==file_reads&&persist_count==file_reads&&ime_loads==file_reads);assert(class_left==(loaded&&(n&1))&&class_wrap==(loaded&&(n&2)));assert(aliases.size()==5);assert(aliases[0]=="GBANK_MAIN_PC=GBANK_MAIN"&&aliases[1]=="GBANK_GUI_PC=GBANK_GUI"&&aliases[2]=="GBANK_FRONT_END_PC=GBANK_FRONT_END"&&aliases[3]=="PARTICLE_MAIN_PC=PARTICLE_MAIN"&&aliases[4]=="PARTICLE_FRONTEND_PC=PARTICLE_FRONTEND");assert(alias_retail==GUseRetailBanks);
if(GUseRetailBanks){assert(banks.size()==(GSkipFrontend?9:2));unsigned k=0;if(GSkipFrontend){assert(banks[k].path==L"graphics/graphics.big"&&banks[k++].resident);assert(banks[k].path==L"graphics/pc\\textures.big"&&banks[k++].resident);assert(banks[k].path==L"dialogue/dialogue.big"&&!banks[k++].resident);assert(banks[k].path==L"hdmisc/pc\\effects.big"&&!banks[k++].resident);assert(banks[k].path==L"dialogue/text.big"&&!banks[k++].resident);assert(banks[k].path==L"misc/temp.big"&&!banks[k++].resident);assert(banks[k].path==L"shader/pc\\shaders.big"&&!banks[k++].resident);}assert(banks[k].path==L"fonts/fonts.big"&&banks[k++].resident);assert(banks[k].path==L"dialogue/text.big"&&!banks[k].resident);assert(header_dir==L"data\\defs\\RetailHeaders\\"&&compiled_directory==L"Data\\CompiledDefs\\");}else{assert(banks.empty());assert(ini_file==(GRunFromDVD?L"banks_dvd.ini":L"banks.ini"));assert(header_dir==L"data\\defs\\DevHeaders\\"&&compiled_directory==L"Data\\CompiledDefs\\Development\\");}assert(table_filename==compiled_directory+L"names.bin");assert(table_loads==(unsigned)GUseCompiledDefs);if(GUseCompiledDefs)assert(table_readonly==!GAllowWADGeneration);
unsigned last_path=user_exists?(saves_exists?2:1):3;bool permission_error=last_path==error_path;assert(permission_paths.size()==(user_exists&&saves_exists?2:1));assert(close_count==(unsigned)user_exists+(unsigned)(user_exists&&saves_exists));if(permission_error){assert(!exited&&result==1&&message_count==1&&initialise_count==0&&registry_count==0);assert(message_text==(text_entry_exists?L"error":L"")&&message_caption==(caption_entry_exists?L"caption":L""));continue;}
assert(message_count==0);bool expected_exit=!GOnlyBuildStaticMaps&&(!eula_ok||(!GSkipConfigDetection&&!config_ok));assert(exited==expected_exit);assert(eula_count==(unsigned)!GOnlyBuildStaticMaps);assert(config_count==(unsigned)(!GOnlyBuildStaticMaps&&eula_ok&&!GSkipConfigDetection));assert(free_config_count==(unsigned)(config_count&&config_ok));assert(PrepareParticles_01375754==!GOnlyBuildStaticMaps);if(exited){assert(initialise_count==0&&registry_count==0);continue;}
assert(result==0&&initialise_count==1&&ime_inits==1&&registry_count==1&&registry_writes==1&&profile_sets==0);assert(captured.InstanceHandle==&instance&&captured.CommandLine==cmd&&captured.WindowsShow==3);assert(captured.Flags==(GSoundOn?21:5)&&captured.PreferPrimaryDisplayDevice==GForcePrimary);if(GSoundOn){assert(reinterpret_cast<float*>(captured.Body_38)[2]==32.0f&&reinterpret_cast<float*>(captured.Body_38)[3]==5.0f&&reinterpret_cast<float*>(captured.Body_38)[4]==1.0f);assert(captured.Body_38[5]==256&&captured.Body_38[7]==65536);}assert(captured.DisplayManagerInit.Depth==32&&captured.DisplayManagerInit.Width==800&&captured.DisplayManagerInit.Height==600&&captured.DisplayManagerInit.ZBufferDepth==24&&captured.DisplayManagerInit.RefreshRate==60);assert(captured.DisplayManagerInit.MultisampleType==(GAntialiasingOn?(GAntialiasing9x?SURFACE_MULTISAMPLE_9:SURFACE_MULTISAMPLE_4):SURFACE_MULTISAMPLE_NONE));assert(captured.DisplayManagerInit.UseRenderManager&&captured.DisplayManagerInit.NoBackBuffers==1);assert(captured.DisplayManagerInit.RenderManagerInitFlags==((RenderConfigurationFlag1_013750a4?1u:0u)|(!RenderConfigurationFlag2_013750aa?2u:0u)));assert(captured.DisplayManagerInit.ShaderResourcePath.text==L"shader/"&&captured.TempPath.text==L"cache/"&&captured.AppName.text==L"Fable test");assert(captured.StreamingFontBankHandle.text=="FONT_BANK");assert(captured.InputManagerInit.UseJoystick&&captured.InputManagerInit.UseKeyboard&&captured.InputManagerInit.UseMouse);assert(captured.SkipConfigDetection==(GSkipConfigDetection||GOnlyBuildStaticMaps));assert(captured.WaitWhileInactive==!GAllowBackgroundProcessing&&captured.ExclusiveMode==GFullScreen);assert(captured.IMEMessageFunc==NUISystem::CIME::ProcessWinMessage&&captured.MessageFilter==NUISystem::CIME::FilterMessage&&captured.IsErrorFailureFunc==GFIsSystemErrorFailure);assert(font_fixed==UseFixedFontFunction_013750ab);assert(met_count==(unsigned)GAllowWADGeneration);assert(game_plays==(unsigned)(system_code==1&&game_init)&&game_uninits==game_plays&&handle_error_count==(unsigned)(system_code!=1));assert(stages.size()==7&&stages.front()=="Parse Command Line"&&stages.back()=="End basic init");}
puts("00402510_TEST PASS");return 0;}
