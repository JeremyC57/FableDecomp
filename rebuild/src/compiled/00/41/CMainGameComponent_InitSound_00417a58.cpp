// CMainGameComponent::InitSound() -- configure localized, normal and atmosphere sound banks and retain the main sample bank.
// Leaf: ?InitSound@CMainGameComponent@@IAEXXZ
// Flags:
// Retail: 0x00417a58,2097 bytes,thiscall,no arguments.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
#define FABLE_RETAIL_CMAINGAMECOMPONENT_INITSOUND
#define FABLE_RETAIL_CASOUNDMANAGER_BANKREGISTRATION
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_DATA_TEXT_EQ
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefPointer.h"
#include "engine/CSoundDef.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundBank.h"
#include "engine/CAFile.h"
namespace NLocalisation { CCharString FABLE_FASTCALL GetSoundDefName(); }
namespace NResourceDirectoryNames {
    CWideString FABLE_FASTCALL GetDVDDialogueDir();
    CWideString FABLE_FASTCALL GetDefDir();
    CWideString FABLE_FASTCALL GetDVDSoundDir();
    CWideString FABLE_FASTCALL GetHDSoundDir();
}
namespace NProfileTimer {
    void FABLE_FASTCALL StartProfile(const CCharString&,long);
    void FABLE_FASTCALL EndProfile(long);
}
namespace NProgressDisplay {
    void FABLE_FASTCALL DisplayProgress(const CCharString&,float,bool,bool);
}
// Native004196b2 takes a temporary def handle and copies its typed pointer into the output.
template<> bool CDefinitionManager::GetDef<CSoundDef>(const CCharString& name,CDefPointer<const CSoundDef>* output) const
{
    CDefPointer<const CDefClassBase> definition=GetPDefFromInstantiationName(name);
    if(definition.IsNull())return false;
    output->SetDynamicCast(definition);
    return true;
}
void CMainGameComponent::InitSound()
{
    PSampleBank=0;
    if(!PSoundManager)return;
    NProfileTimer::StartProfile("Lut reg" "ister",100);
    CDefPointer<const CSoundDef> localised;
    CGameDefinitionManager::Get().GetDef(NLocalisation::GetSoundDefName(),&localised);
    for(unsigned long index=0;index<localised->SoundBankEntries.size();++index) {
        NProgressDisplay::DisplayProgress("Init Localised Sound Bank Entries",-1.0f,false,false);
        const CSoundBankEntry& entry=localised->SoundBankEntries[index];
        CWideString filename=entry.Filename.ToWideString();
        CWideString name,extension;
        CAFile::SplitPath(filename,0,0,&name,&extension);
        if(extension==L".lut")extension=L".lug";
        filename=name+extension;
        CWideString bankPath=CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetDVDDialogueDir()+entry.Filename.ToWideString());
        CWideString symbolPath=CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetDefDir()+entry.SymbolFilename.ToWideString());
        NProgressDisplay::DisplayProgress("Registering Localised Sound Bank",-1.0f,false,false);
        PSoundManager->RegisterSoundBank(entry.ID,filename,bankPath,symbolPath,entry.LipSyncBankHandle);
    }
    NProfileTimer::EndProfile(100);
    CDefPointer<const CSoundDef> main;
    CGameDefinitionManager::Get().GetDef("MAIN_SOUND_SETUP",&main);
    for(unsigned long index=0;index<main->SoundBankEntries.size();++index) {
        NProgressDisplay::DisplayProgress("Init Sound Bank Entries",-1.0f,false,false);
        const CSoundBankEntry& entry=main->SoundBankEntries[index];
        CWideString filename=entry.Filename.ToWideString();
        NProfileTimer::StartProfile("Normal lug reg" "ister "+entry.Filename,100);
        CWideString name,extension;
        CAFile::SplitPath(filename,0,0,&name,&extension);
        CWideString dataExtension=extension;
        if(extension==L".lug")dataExtension=L".met";
        else if(extension==L".lut")extension=L".lug";
        filename=name+extension;
        CWideString bankPath;
        if(entry.DVD)bankPath=CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetDVDSoundDir()+name+dataExtension);
        else bankPath=CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetHDSoundDir()+name+dataExtension);
        CWideString symbolPath=CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetDefDir()+entry.SymbolFilename.ToWideString());
        NProgressDisplay::DisplayProgress("Registering Sound Bank",-1.0f,false,false);
        PSoundManager->RegisterSoundBank(entry.ID,filename,bankPath,symbolPath,entry.LipSyncBankHandle);
        if(dataExtension==L".met")
            PSoundManager->GetBank(entry.ID)->SetSourceLugPath(NResourceDirectoryNames::GetHDSoundDir()+name+L".lug");
        NProfileTimer::EndProfile(100);
    }
    for(unsigned long index=0;index<main->AtmosBankEntries.size();++index) {
        NProgressDisplay::DisplayProgress("Registering Atmos Sound Bank",-1.0f,false,false);
        const CAtmosBankEntry& entry=main->AtmosBankEntries[index];
        CWideString filename=entry.Filename.ToWideString();
        NProfileTimer::StartProfile("Atmos lug reg" "ister "+entry.Filename,100);
        CWideString name,extension;
        CAFile::SplitPath(filename,0,0,&name,&extension);
        filename=name+L".met";
        PSoundManager->RegisterAtmosSoundBank(entry.ID,filename,CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetHDSoundDir()+filename));
        CWideString hdSource=NResourceDirectoryNames::GetHDSoundDir()+name+L".lug";
        CWideString dvdSource=NResourceDirectoryNames::GetDVDSoundDir()+name+L".lug";
        PSoundManager->GetBank(entry.ID)->SetSourceLugPath(hdSource);
        NProfileTimer::EndProfile(100);
    }
    PSampleBank=PSoundManager->GetBank(1);
}