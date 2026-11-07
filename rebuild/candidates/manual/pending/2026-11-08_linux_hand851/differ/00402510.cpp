// GFMain(HINSTANCE,char*,int) -- initialize file, text and game systems, validate writable directories, then run the game.
// Grade: DIFFER(SRC_FAIL, 79 errors) -- header debt, not a compiler residue: the Codex pass-12 body (RELOCATION_MATCH in its own header world) needs a polymorphic CBankFile (virtual OpenReadOnly at slot 1, called `call [ebp+4]` at 0x00402EA5; FindIndexBySymbolCRC 0x009CC410 as a CBankFile member) under CBankFileAsync / NGameText::CDataBank, and engine/CBankFile.h is HOLD; plus GFMAIN opt-ins in ~12 existing headers (CSystemManager, CDisplayManager, CRenderManager, CAFile, CDefinitionManager, CDefStringTable, CUserProfileManager, CStreamingFontBank, CMainGameComponent, CDiskFileWin32/CCharString(CAFile&)) and 16 new headers (00402510_hdr/). See NOTES.md 'Header debt'.
// Next idea: once CBankFile.h is released from HOLD, apply the Codex polymorphic CBankFile branch as an opt-in, add the GFMAIN members listed in NOTES.md, and regrade the Codex body unchanged; the four file-static helpers (GFUninitialise 0x00401B80, GFHandleSystemInitError 0x00401C00, GFGetBuildNumber2 0x00401F30, GFInitialise 0x004022B0) must stay byte-identical (GFInitialise is already landed separately: one address, one body).
// Leaf: ?GFMain@@YIJPAUHINSTANCE__@@PADH@Z
// Flags: /Gr /GS
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_GFMAIN
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STATBOX
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
#include "engine/CThreadedFile.h"
#include "engine/CFileInstaller.h"
#include "engine/CBankFileManager.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefStringTable.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NGameText_CTextEntry.h"
#include "engine/CSystemRegistry.h"
#include "engine/CGame.h"
#include "engine/CXMVPlayer.h"
#include "engine/NUISystem_CIME.h"
#include "engine/CMainGameComponent.h"
#include "engine/CUserProfileManager.h"
#include "engine/CStreamingFontBank.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
namespace NProfileTimer {void __fastcall StartProfile(const CCharString&,long);void __fastcall EndProfile(long);}
namespace NLocalisation {CCharString __fastcall GetLanguageFolder();CCharString __fastcall GetTextBankName();CCharString __fastcall GetStreamingFontBankName();}
namespace NResourceDirectoryNames {
 CWideString __fastcall GetGraphicBankDir();CWideString __fastcall GetFontBankDir();CWideString __fastcall GetHDDialogueDir();CWideString __fastcall GetHDMiscDir();CWideString __fastcall GetMiscDir();CWideString __fastcall GetShaderBankDir();CWideString __fastcall GetCacheDir();CWideString __fastcall GetMyDocumentsDir();CWideString __fastcall GetUserDataDir();CWideString __fastcall GetSaveDir();
}
namespace NGlobalConsole {void __fastcall Initialise();}
namespace EULA {int __fastcall DoEULAThings();}
CWideString __fastcall operator+(const unsigned short*,const CWideString&);
CWideString __fastcall operator+(const CWideString&,const unsigned short*);
CWideString __fastcall operator+(const CWideString&,const CWideString&);
void __fastcall DoDirtyDisc();void __fastcall GFHardDiskError(const char*);bool __fastcall GFIsSystemErrorFailure(ESystemCode);
unsigned long __fastcall TestDirForValidFilePermissions(const CWideString&);
bool __fastcall GFConfigDetection();bool __fastcall GFFreeConfigDetection();
CWideString __fastcall GetWindowTitle();
extern bool GAllowWADGeneration,GUseCompiledSoundSymbols,GRunFromDVD,GPreventInstallation,GIgnoreTimestampOnInstall,GStartVTuneDisabled,GUseAsyncLoadingMemoryHandler;
extern bool GLeftAlignText,GNoHangulWordWrap,GDisableCapsLock,GUseRetailBanks,GSkipFrontend,GUseCompiledDefs,GForcePrimary,GSoundOn;
extern long GResolutionWidth,GResolutionHeight,GResolutionDepth,GZBufferDepth,GResolutionRefreshRate,GPushBufferSize,GKickoffSize;
extern bool GPresentImmidiate,GAntialiasingOn,GAntialiasing9x,GFullScreen,GAllowBackgroundProcessing,GSkipConfigDetection,GOnlyBuildStaticMaps;
// Fidelity: the following native-only names remain descriptive after the recorded original-PDB searches.
extern bool StartupConsoleEnabled_0137548f,SaveSoundSymbols_0138dd3b,LoadSoundSymbols_013bc9f0;
extern bool PrepareParticles_01375754,RenderConfigurationFlag1_013750a4,RenderConfigurationFlag2_013750aa,UseFixedFontFunction_013750ab;
void __fastcall ClearStartupLatch_009d81e0();
CCharString __fastcall operator+(const CCharString&,const char*);
void __fastcall UninitializeStreamingFontTextureCache();
extern CASoundManager*PSoundManager;extern CDisplayManager*GPDisplayManager;
extern CRenderManager*GPRenderManager;extern CShaderRenderManager*GPShaderRenderManager;
extern CInputManager*GPInputManager;extern CFontManager*GPFontManager;extern CGraphicBankManager*GPGraphicBankManager;
extern unsigned long GTextureBitDepth,GMaxTextureWidth,GMaxTextureHeight;
extern long GMaxDesiredTextureSize,GDesiredTextureDepth,GFrameLimit;
extern CCharString GVersionString;
static void __fastcall GFGetBuildNumber2()
{
 if(CAFile::PathExists(L"full_build_version.txt")){
  CDiskFileWin32 build_file(L"full_build_version.txt",FILE_ACCESS_READ,2);
  CCharString str(build_file,-1);GVersionString=str;
 }
}
static bool __fastcall GFInitialise()
{
 // Layout: native CSystemManager keeps its CASoundManager counted pointer at+0x7c; this member was removed in the donor.
 PSoundManager=reinterpret_cast<CASoundManager**>(GFGetSystemManager())[0x7c/sizeof(void*)];
 GPDisplayManager=GFGetSystemManager()->GetDisplayManager();
 GPRenderManager=&GPDisplayManager->GetRenderManager();GPShaderRenderManager=&CShaderRenderManager::Get();
 GPInputManager=GFGetSystemManager()->GetInputManager();GPFontManager=GFGetSystemManager()->GetFontManager();GPGraphicBankManager=GFGetSystemManager()->GetGraphicBankManager();
 GTextureBitDepth=GDesiredTextureDepth;
 if(GPDisplayManager->IsTextureBitDepthSupported(GDesiredTextureDepth))GTextureBitDepth=GDesiredTextureDepth;
 else if(GPDisplayManager->IsTextureBitDepthSupported(16))GTextureBitDepth=16;
 else if(GPDisplayManager->IsTextureBitDepthSupported(24))GTextureBitDepth=24;
 else if(GPDisplayManager->IsTextureBitDepthSupported(32))GTextureBitDepth=32;
 unsigned long dimension=GPRenderManager->GetMaxTextureWidth();if((unsigned long)GMaxDesiredTextureSize<dimension)dimension=GMaxDesiredTextureSize;GMaxTextureWidth=dimension;
 dimension=GPRenderManager->GetMaxTextureHeight();if((unsigned long)GMaxDesiredTextureSize<dimension)dimension=GMaxDesiredTextureSize;GMaxTextureHeight=dimension;
 CXMVPlayer::GlobalRegisterPlayer();GFGetBuildNumber2();
 if(GGame.Initialise()==true){GFrameLimit=GPDisplayManager->GetRefreshRate()/2;return true;}return false;
}
static void __fastcall GFUninitialise()
{
 GGame.Uninitialise();CUserProfileManager::Release();NBankFileManager::GBankFileManager.Close();
 GFGetSystemManager()->Uninitialise();CThreadedFile::GetManager()->Uninitialise();
 UninitializeStreamingFontTextureCache();NUISystem::CIME::Get().Shutdown();
}
static void __fastcall GFHandleSystemInitError(ESystemCode code)
{
 CCharString name="Fable";
 switch(code){
 case 2:GFGetSystemManager()->DisplayCriticalMessage(name+" was unable to initialise the display. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Use the -fullscreen option to force fullscreen, and/or -forceprimary if you have a secondary 3D accelerator you wish to ignore. Otherwise, make sure you have closed down all other applications and try again.");return;
 case 4:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise a window. You may need to reboot your machine.");return;
 case 16:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise DirectX. You may need to reboot your machine.");return;
 case 5:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to access the hard disc. Lord knows why.");return;
 case 17:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise the network manager.");return;
 case 12:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise the input manager.");return;
 case 6:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise a joystick. Please make sure you have one connected.");return;
 case 10:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise a mouse. Please make sure you have one connected.");return;
 case 13:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise a your 3D card. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Otherwise, make sure you have closed down all other applications and try again.");return;
 case 14:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise debugging information. Lord knows why.");return;
 case 15:GFGetSystemManager()->DisplayCriticalMessage(name+"  was unable to initialise. Lord knows why.");return;
 default:GFGetSystemManager()->DisplayCriticalMessage("Unknown initialisation error. Bugger.");return;
 }
}
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
