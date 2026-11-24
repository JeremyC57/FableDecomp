// GFMain(HINSTANCE,char*,int) -- the game entry body: parse-time callbacks, install / language / bank / definition-table setup,
// writable user-data and save directory checks (a localized message box on failure), system initialisation, then GFInitialise,
// the game loop (GGame.Play) and GFUninitialise; finally the registry GFX-reset flag is cleared.
// Grade: RELOCATION_MATCH 3952v3952 (VC7.1 RTM, /Gr /GS as the same object's landed GFConfigDetection 0x004017B0; Codex pass-12
//        body ported onto the current shared headers + overlay opt-ins, see hdr/README.md). What closed it: inline
//        CCharString::GetCRC and CDiskFileWin32::IsOpen, and the one-level CIVCountedPointer destructor for the two CTextEntry
//        pointers. Pick-any: every COMDAT copy EQ to Fable.exe (~CAFile and ~list<CStringParserCommentBoundaries> declaration-only,
//        CCountedPointer ReleaseData Data-first).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00402510, 3952 bytes. Name: Ego_r.pdb ?GFMain@@YIJPAUHINSTANCE__@@PADH@Z.
// The four helpers of this file (GFInitialise / GFUninitialise / GFHandleSystemInitError / GFGetBuildNumber2) are landed rows of
// their own and are called out of line here.
// Leaf: ?GFMain@@YIJPAUHINSTANCE__@@PADH@Z
// Flags: /Gr /GS
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CDISKFILEWIN32_ISOPEN_INLINE
#define FABLE_RETAIL_CAFILE_SETCALLBACKS
#define FABLE_RETAIL_CAFILE_CURRENTPATH
#define FABLE_RETAIL_CAFILE_DTOR_DECL_ONLY
#define FABLE_RETAIL_CTHREADEDFILE
#define FABLE_RETAIL_CTHREADEDFILE_PROCESS_PASS27
#define FABLE_RETAIL_CTHREADEDFILEMANAGER_SETCALLBACKS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER_COMPILEDDEFDIR
#define FABLE_RETAIL_CDEFINITIONMANAGER_STARTUP_SETTERS
#define FABLE_RETAIL_CBANKFILEASYNC_SETENABLEFAILUREHANDLING
#define FABLE_RETAIL_CFILEINSTALLER_SETIGNORETIMESTAMP_INLINE
#define FABLE_RETAIL_CXMVPLAYER_TEXTLAYOUT_SETTERS
#define FABLE_RETAIL_CDEFSTRINGTABLE_LOADTABLE
#define FABLE_RETAIL_CSYSTEMMANAGER_INITIALISE
#define FABLE_RETAIL_CGAME_PLAY
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CSTREAMINGFONTBANK_SETUSEFIXEDFUNCTION
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GENERATEMETFILES
#define FABLE_RETAIL_NGAMETEXT_CDATABANK_CTOR_DECL
#define FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CTBASESINGLETON_RELEASE_BODY
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CSYSTEMMANAGER_MOUSE
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_CAFILE_ERRORCALLBACK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>
#include "rebuild_abi.h"
struct HWND__;typedef HWND__*HWND;struct HINSTANCE__;struct HKEY__;typedef HKEY__*HKEY;
typedef void*HANDLE;struct _SECURITY_ATTRIBUTES;
extern "C" __declspec(dllimport)HANDLE __stdcall CreateFileW(const unsigned short*,unsigned long,unsigned long,_SECURITY_ATTRIBUTES*,unsigned long,unsigned long,HANDLE);
extern "C" __declspec(dllimport)int __stdcall CloseHandle(HANDLE);
extern "C" __declspec(dllimport)int __stdcall MessageBoxW(HWND,const unsigned short*,const unsigned short*,unsigned int);
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define FILE_FLAG_BACKUP_SEMANTICS 0x02000000
#include <stdlib.h>
#include "engine/CSystemManagerInit.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CThreadedFileManager.h"
#include "engine/CFileInstaller.h"
#include "engine/CBankFileManager.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefStringTable.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CSystemRegistry.h"
#include "engine/CGame.h"
#include "engine/CXMVPlayer.h"
#include "engine/NUISystem_CIME.h"
#include "engine/CMainGameComponent.h"
#include "engine/CUserProfileManager.h"
#include "engine/CStreamingFontBank.h"
namespace NProfileTimer {void __fastcall StartProfile(const CCharString&,long);void __fastcall EndProfile(long);}
namespace NLocalisation {CCharString __fastcall GetLanguageFolder();CCharString __fastcall GetTextBankName();CCharString __fastcall GetStreamingFontBankName();}
namespace NResourceDirectoryNames {
 CWideString __fastcall GetGraphicBankDir();CWideString __fastcall GetFontBankDir();CWideString __fastcall GetHDDialogueDir();CWideString __fastcall GetHDMiscDir();CWideString __fastcall GetMiscDir();CWideString __fastcall GetShaderBankDir();CWideString __fastcall GetCacheDir();CWideString __fastcall GetMyDocumentsDir();CWideString __fastcall GetUserDataDir();CWideString __fastcall GetSaveDir();
}
void __fastcall InitialiseConsoleVariables();   // 0x00413C50 (manifest label; registers the startup console variables: "AllowDataGeneration", "UseRetailBanks", "SetLevel" ..)
namespace EULA {int __fastcall DoEULAThings();}
CWideString __fastcall operator+(const unsigned short*,const CWideString&);
CWideString __fastcall operator+(const CWideString&,const unsigned short*);
CWideString __fastcall operator+(const CWideString&,const CWideString&);
void __fastcall DoDirtyDisc();void __fastcall GFHardDiskError(const char*);bool __fastcall GFIsSystemErrorFailure(ESystemCode);
unsigned long __fastcall TestDirForValidFilePermissions(const CWideString&);
bool __fastcall GFConfigDetection();bool __fastcall GFFreeConfigDetection();
CWideString __fastcall GetWindowTitle();
// Data globals: each spelled as the landed rows spell its address (one address, one name); where a PDB proves another spelling
// it is in NAME_FIXES.tsv for the landing respell. GDisableCapsLock (Ego_d.pdb) is the one exception: the landed
// ?g_KeyRedefinerFlag@@3EA is an unsigned char and cannot bind CPersistContext::Transfer(const char*, bool&, bool).
extern bool GAllowDataGeneration,g_bUseCompiledSoundSymbols,g_bRunFromDVD,GPreventInstallation,GIgnoreTimestampOnInstall,GStartVTuneDisabled,GUseAsyncLoadingMemoryHandler;
extern bool g_bLeftAlignText,GNoHangulWordWrap,GDisableCapsLock,g_bRuntimeLevelFiles,g_bStartMainGame,g_bUseCompiledDefs,GForcePrimary,GSoundOn;
extern long GResolutionWidth,GResolutionHeight,GResolutionDepth,GZBufferDepth,GResolutionRefreshRate,GPushBufferSize,GKickoffSize;
extern bool g_PresentationInterval,GAntialiasingOn,GAntialiasing9x,g_bFrontEndFlag_137544A,GAllowBackgroundProcessing,GSkipConfigDetection,g_bCompileFrontendDefinitions;
extern bool g_bFrontEndFlag_137548F,g_bSaveBinarySoundSymbols,g_bLoadBinarySoundSymbols;
extern bool g_bParticlesEnabled,g_FeatureA,g_FeatureF,g_FeatureG;
extern CGame GGame;                       // 0x013B83D0 (Ego_r.pdb dynamic initializer 'GGame'; landed spelling)
extern CDefStringTable GDefStringTable;   // 0x013CA828 (landed spelling)
void __fastcall DisableVTuneCollection();   // 0x009D81E0 (descriptive: clears the byte 0x013964A8 its twin 0x009D81C0 sets; manifest placeholder ClearPendingFlag_009d81e0)
CCharString __fastcall operator+(const CCharString&,const char*);
// The four helpers of this source file are landed rows of their own (out of line in retail: calls at 0x00403389 / 0x004033C7 / 0x004033CF):
bool __fastcall GFInitialise();                                  // 0x004022B0
void __cdecl GFUninitialise();                                    // 0x00401B80
void __stdcall GFHandleSystemInitError(unsigned long errorCode);  // 0x00401C00
long __fastcall GFMain(HINSTANCE__*instance_handle,char*command_line,int show)
{
 NProfileTimer::StartProfile("Parse Command Line",0);
 CSystemManagerInit sys_init;
 CWideString launched_profile_name;
 CAFile::SetErrorCallback(DoDirtyDisc);CThreadedFileManager::SetErrorCallback(DoDirtyDisc);
 CAFile::SetHardDiskErrorCallback(GFHardDiskError);CThreadedFileManager::SetHardDiskErrorCallback(GFHardDiskError);
 CAFile::SetCurrentPath(CAFile::GetProjectPath());
 if(g_bFrontEndFlag_137548F)InitialiseConsoleVariables();
 NProfileTimer::EndProfile(0);
 NProfileTimer::StartProfile("Setup Basic install files",0);
 CDefinitionManager::SetCreatingCompiledDefs(GAllowDataGeneration);
 CDefinitionManager::SetCreateChecksums(GAllowDataGeneration);
 g_bSaveBinarySoundSymbols=GAllowDataGeneration;
 g_bLoadBinarySoundSymbols=g_bUseCompiledSoundSymbols;
 if(g_bRunFromDVD&&!GPreventInstallation)CFileInstaller::Get().SetIgnoreTimeStampWhenCopying(GIgnoreTimestampOnInstall);
 if(GStartVTuneDisabled)DisableVTuneCollection();
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
   context.Transfer("LeftAlignText",g_bLeftAlignText,false);
   context.Transfer("NoHangulWordWrap",GNoHangulWordWrap,false);
   context.Transfer("DisableCapsLock",GDisableCapsLock,false);
   NUISystem::CIME::LoadIMESettings(context);
  }
 }
 CXMVPlayer::SetLeftAlignText(g_bLeftAlignText);CXMVPlayer::SetNoHangulWordWrap(GNoHangulWordWrap);
 NProfileTimer::EndProfile(0);
 NProfileTimer::StartProfile("Setup basic retail banks",0);
 NBankFileManager::GBankFileManager.SetUsingRetailBanks(g_bRuntimeLevelFiles);
 NBankFileManager::GBankFileManager.SetBankAlias("GBANK_MAIN_PC","GBANK_MAIN");
 NBankFileManager::GBankFileManager.SetBankAlias("GBANK_GUI_PC","GBANK_GUI");
 NBankFileManager::GBankFileManager.SetBankAlias("GBANK_FRONT_END_PC","GBANK_FRONT_END");
 NBankFileManager::GBankFileManager.SetBankAlias("PARTICLE_MAIN_PC","PARTICLE_MAIN");
 NBankFileManager::GBankFileManager.SetBankAlias("PARTICLE_FRONTEND_PC","PARTICLE_FRONTEND");
 CWideString compiled_def_dir;
 using namespace NResourceDirectoryNames;
 if(g_bRuntimeLevelFiles){
  if(g_bStartMainGame){
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
  if(g_bRunFromDVD)NBankFileManager::GBankFileManager.OpenIniFile(L"banks_dvd.ini");
  else NBankFileManager::GBankFileManager.OpenIniFile(L"banks.ini");
  NBankFileManager::SetHeaderDir(L"data\\defs\\DevHeaders\\");
  compiled_def_dir=L"Data\\CompiledDefs\\Development\\";
 }
 NProfileTimer::EndProfile(0);
 NProfileTimer::StartProfile("Setup definition manager table",0);
 CDefinitionManager::SetCompiledDefDirectory(compiled_def_dir);
 GDefStringTable.SetTableFilename(compiled_def_dir+L"names.bin");
 CDefinitionManager::SetUsingCompiledDefs(g_bUseCompiledDefs);
 if(g_bUseCompiledDefs)GDefStringTable.LoadTable(!GAllowDataGeneration);
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
 if(GSoundOn==true){sys_init.Flags=21;sys_init.SoundManagerInit.Setting08=32.0f;sys_init.SoundManagerInit.Setting0C=5.0f;sys_init.SoundManagerInit.Scale=1.0f;sys_init.SoundManagerInit.ChannelMask=0x100;sys_init.SoundManagerInit.ResourceMemory=0x10000;}
 sys_init.DisplayManagerInit.Depth=GResolutionDepth;sys_init.DisplayManagerInit.Width=GResolutionWidth;sys_init.DisplayManagerInit.Height=GResolutionHeight;
 sys_init.DisplayManagerInit.ZBufferDepth=(unsigned char)GZBufferDepth;sys_init.DisplayManagerInit.RefreshRate=GResolutionRefreshRate;
 sys_init.DisplayManagerInit.PresentImmediate=g_PresentationInterval;sys_init.DisplayManagerInit.PushBufferSize=GPushBufferSize;sys_init.DisplayManagerInit.KickOffSize=GKickoffSize;
 sys_init.DisplayManagerInit.ShaderResourcePath=GetShaderBankDir();
 if(GAntialiasingOn)sys_init.DisplayManagerInit.MultisampleType=GAntialiasing9x?SURFACE_MULTISAMPLE_9:SURFACE_MULTISAMPLE_4;
 sys_init.DisplayManagerInit.UseRenderManager=true;sys_init.DisplayManagerInit.NoBackBuffers=1;
 sys_init.IsErrorFailureFunc=GFIsSystemErrorFailure;sys_init.AppName=GetWindowTitle();sys_init.ExclusiveMode=g_bFrontEndFlag_137544A;
 sys_init.TempPath=GetCacheDir();sys_init.StreamingFontBankHandle=NLocalisation::GetStreamingFontBankName();
 sys_init.WaitWhileInactive=!GAllowBackgroundProcessing;
 sys_init.InputManagerInit.UseJoystick=true;sys_init.InputManagerInit.UseKeyboard=true;sys_init.InputManagerInit.UseMouse=true;sys_init.SkipConfigDetection=GSkipConfigDetection;
 if(g_bCompileFrontendDefinitions){sys_init.SkipConfigDetection=true;g_bParticlesEnabled=false;}
 else{
  if(!EULA::DoEULAThings())exit(1);
  if(!sys_init.SkipConfigDetection&&!g_bCompileFrontendDefinitions){if(!GFConfigDetection())exit(1);GFFreeConfigDetection();}
 }
 if(g_FeatureA)sys_init.DisplayManagerInit.RenderManagerInitFlags|=1;
 if(!g_FeatureF)sys_init.DisplayManagerInit.RenderManagerInitFlags|=2;
 if(g_FeatureG)CStreamingFontBank::SetUseFixedFunction(true);
 sys_init.IMEMessageFunc=NUISystem::CIME::ProcessWinMessage;sys_init.MessageFilter=NUISystem::CIME::FilterMessage;
 ESystemCode code=GFGetSystemManager()->Initialise(&sys_init);
 NProfileTimer::EndProfile(0);
 NUISystem::CIME::Get().Initialise(GFGetSystemManager()->GetApplicationWinHandle());
 NProfileTimer::StartProfile("End basic init",0);
 if(GAllowDataGeneration)CMainGameComponent::GenerateMetFilesFromLugFiles();
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