#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <stdio.h>
class CWideString {
public:
    std::basic_string<unsigned short> Text;
    CWideString(){}
    template<class T> CWideString(const T* text){while(*text)Text+=(unsigned short)*text++;}
    template<class T> CWideString& operator=(const T* text){Text.erase();while(*text)Text+=(unsigned short)*text++;return *this;}
    template<class T> bool operator==(const T* text)const{return Text==CWideString(text).Text;}
};
static CWideString operator+(const CWideString&a,const CWideString&b){CWideString r;r.Text=a.Text+b.Text;return r;}
template<class T> static CWideString operator+(const CWideString&a,const T*b){return a+CWideString(b);}
class CCharString {
public:
    std::string Text;
    CCharString(){}
    CCharString(const char* text):Text(text){}
};
static CCharString operator+(const char*a,const CCharString&b){return CCharString((std::string(a)+b.Text).c_str());}
class CDefString {
public:
    std::string Text;
    CDefString(){}
    CDefString(const char* text):Text(text){}
    CWideString ToWideString()const{return CWideString(Text.c_str());}
    operator CCharString()const{return CCharString(Text.c_str());}
};
class CSoundBankEntry {public:unsigned long ID;CDefString Filename,SymbolFilename,LipSyncBankHandle;bool DVD;};
class CAtmosBankEntry {public:unsigned long ID;CDefString Filename;};
class CSoundDef {
public:
    int References;
    std::vector<CSoundBankEntry> SoundBankEntries;
    std::vector<CAtmosBankEntry> AtmosBankEntries;
    CSoundDef():References(1){}
};
template<class T> class CDefPointer {
    T* Object;
public:
    CDefPointer():Object(0){}
    ~CDefPointer(){if(Object)--const_cast<CSoundDef*>(Object)->References;}
    T* operator->()const{return Object;}
    void Set(T* object){assert(!Object);Object=object;++const_cast<CSoundDef*>(Object)->References;}
};
static CSoundDef localizedDefinition,mainDefinition;
static std::vector<std::string> lookups,events;
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& Get(){static CGameDefinitionManager manager;return manager;}
    bool GetDef(const CCharString&name,CDefPointer<const CSoundDef>*output){lookups.push_back(name.Text);output->Set(name.Text=="LOCAL_SOUND"?&localizedDefinition:&mainDefinition);return true;}
};
namespace NLocalisation { CCharString GetSoundDefName(){events.push_back("localization");return CCharString("LOCAL_SOUND");} }
namespace NResourceDirectoryNames {
    static int dvdCalls=0,hdCalls=0,dialogCalls=0,defCalls=0;
    CWideString GetDVDDialogueDir(){++dialogCalls;return CWideString("dvd-dialog/");}
    CWideString GetDefDir(){++defCalls;return CWideString("defs/");}
    CWideString GetDVDSoundDir(){++dvdCalls;return CWideString("dvd/");}
    CWideString GetHDSoundDir(){++hdCalls;return CWideString("hd/");}
}
namespace NProfileTimer {
    void StartProfile(const CCharString&name,long channel){assert(channel==100);events.push_back("start:"+name.Text);}
    void EndProfile(long channel){assert(channel==100);events.push_back("end");}
}
namespace NProgressDisplay {
    void DisplayProgress(const CCharString&name,float amount,bool a,bool b){assert(amount==-1.0f&&!a&&!b);events.push_back("progress:"+name.Text);}
}
class CAFile {
public:
    static void SplitPath(const CWideString&path,CWideString*,CWideString*,CWideString*name,CWideString*ext){
        std::basic_string<unsigned short> text=path.Text;
        size_t slash=text.find_last_of((unsigned short)'/');if(slash!=text.npos)text=text.substr(slash+1);
        size_t dot=text.find_last_of((unsigned short)'.');
        name->Text=dot==text.npos?text:text.substr(0,dot);ext->Text=dot==text.npos?std::basic_string<unsigned short>():text.substr(dot);
    }
    static CWideString ConvertPathToFullPath(const CWideString&path){return CWideString("full:")+path;}
};
class CASoundBank {
public:
    CWideString Source;
    int SourceCalls;
    CASoundBank():SourceCalls(0){}
    void SetSourceLugPath(const CWideString&path){Source=path;++SourceCalls;events.push_back("source");}
};
class Registration {
public:
    unsigned long ID;CWideString Name,Path,Symbols;CCharString Lip;bool Atmos;
    Registration(unsigned long id,const CWideString&name,const CWideString&path,const CWideString&symbols,const CCharString&lip,bool atmos):ID(id),Name(name),Path(path),Symbols(symbols),Lip(lip),Atmos(atmos){}
};
class CASoundManager {
public:
    std::vector<Registration> Registrations;
    std::map<long,CASoundBank> Banks;
    void RegisterSoundBank(unsigned long id,const CWideString&name,const CWideString&path,const CWideString&symbols,const CCharString&lip){Registrations.push_back(Registration(id,name,path,symbols,lip,false));events.push_back("bank-added");}
    void RegisterAtmosSoundBank(unsigned long id,const CWideString&name,const CWideString&path){Registrations.push_back(Registration(id,name,path,CWideString(),CCharString(),true));events.push_back("atmos");}
    CASoundBank* GetBank(long id){events.push_back("getbank");return &Banks[id];}
};
static CASoundManager* PSoundManager;
class CMainGameComponent { public:CASoundBank* PSampleBank;void InitSound(); };
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

static void Reset(){
    assert(localizedDefinition.References==1&&mainDefinition.References==1);
    localizedDefinition.SoundBankEntries.clear();mainDefinition.SoundBankEntries.clear();mainDefinition.AtmosBankEntries.clear();
    lookups.clear();events.clear();NResourceDirectoryNames::dvdCalls=0;NResourceDirectoryNames::hdCalls=0;NResourceDirectoryNames::dialogCalls=0;NResourceDirectoryNames::defCalls=0;
}
static CSoundBankEntry Entry(unsigned long id,const char*file,bool dvd){CSoundBankEntry e;e.ID=id;e.Filename=CDefString(file);e.SymbolFilename=CDefString("symbols.sym");e.LipSyncBankHandle=CDefString("lip-bank");e.DVD=dvd;return e;}
int main(){
    CMainGameComponent game;game.PSampleBank=(CASoundBank*)1;PSoundManager=0;Reset();game.InitSound();assert(!game.PSampleBank&&events.empty()&&lookups.empty());
    CASoundManager manager;PSoundManager=&manager;Reset();game.InitSound();assert(game.PSampleBank==&manager.Banks[1]&&manager.Registrations.empty());assert(lookups.size()==2&&events.front()=="start:Lut reg" "ister");
    Reset();manager.Registrations.clear();
    const char* names[]={"folder/a.lut","folder/b.lug","folder/c.met","folder/d.wav","folder/e","folder/f.LUT"};
    for(int i=0;i<6;++i){localizedDefinition.SoundBankEntries.push_back(Entry(10+i,names[i],false));mainDefinition.SoundBankEntries.push_back(Entry(20+i,names[i],(i%2)==0));}
    for(int i=0;i<2;++i){CAtmosBankEntry e;e.ID=30+i;e.Filename=CDefString(i?"wind.wav":"rain.lug");mainDefinition.AtmosBankEntries.push_back(e);}
    game.InitSound();assert(manager.Registrations.size()==14);assert(lookups.size()==2&&lookups[0]=="LOCAL_SOUND"&&lookups[1]=="MAIN_SOUND_SETUP");
    const char* localNames[]={"a.lug","b.lug","c.met","d.wav","e","f.LUT"};
    const char* mainNames[]={"a.lug","b.lug","c.met","d.wav","e","f.LUT"};
    const char* mainPaths[]={"full:dvd/a.lut","full:hd/b.met","full:dvd/c.met","full:hd/d.wav","full:dvd/e","full:hd/f.LUT"};
    for(int i=0;i<6;++i){
        const Registration&a=manager.Registrations[i];assert(a.ID==(unsigned long)(10+i)&&a.Name==localNames[i]&&!a.Atmos&&a.Lip.Text=="lip-bank");
        assert(a.Path.Text==(CWideString("full:dvd-dialog/")+CWideString(names[i])).Text&&a.Symbols=="full:defs/symbols.sym");
        const Registration&b=manager.Registrations[6+i];assert(b.ID==(unsigned long)(20+i)&&b.Name==mainNames[i]&&b.Path==mainPaths[i]&&b.Symbols=="full:defs/symbols.sym"&&b.Lip.Text=="lip-bank"&&!b.Atmos);
    }
    assert(manager.Banks[21].Source=="hd/b.lug"&&manager.Banks[22].Source=="hd/c.lug");assert(manager.Banks[20].SourceCalls==0&&manager.Banks[23].SourceCalls==0);
    assert(manager.Registrations[12].Name=="rain.met"&&manager.Registrations[12].Path=="full:hd/rain.met"&&manager.Registrations[12].Atmos);
    assert(manager.Registrations[13].Name=="wind.met"&&manager.Registrations[13].Path=="full:hd/wind.met"&&manager.Registrations[13].Atmos);
    assert(manager.Banks[30].Source=="hd/rain.lug"&&manager.Banks[31].Source=="hd/wind.lug");
    assert(NResourceDirectoryNames::dialogCalls==6&&NResourceDirectoryNames::defCalls==12&&NResourceDirectoryNames::dvdCalls==5&&NResourceDirectoryNames::hdCalls==9);
    assert(localizedDefinition.References==1&&mainDefinition.References==1&&game.PSampleBank==&manager.Banks[1]);
    int starts=0,ends=0,progress=0;for(size_t i=0;i<events.size();++i){if(events[i].find("start:")==0)++starts;if(events[i]=="end")++ends;if(events[i].find("progress:")==0)++progress;}
    assert(starts==9&&ends==9&&progress==26);
    puts("00417a58_TEST PASS");return 0;
}