// ReadMetadataBank(CLoadLog* log, void* file, CScriptHost* bank, const SBankRequest* settings) -- load the bank data of a .met `file` (PKSerialiseLoad<CBankData>); log "Could not load from file" when there is none, else run the bank processor on it and free the data
// Flags: /EHsc
// Leaf: ?ReadMetadataBank@@YGXPAVCLoadLog@@PAXPAVCScriptHost@@PBUSBankRequest@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17400, length 155 bytes (+ int3 padding), EH-framed (the holder is state 0).
// Callees: PKSerialiseLoad<CBankData> @ 0x00C17940, CLoadLog::AddError @ 0x00C30840, CString::operator<< @ 0x00C0DEC0 ("Could not load from
//   file" @ 0x012B2D44), CBankDataOwner::GetChecked @ 0x00C17740, RunBankProcessor @ 0x00C16CE0, CBankData::~CBankData @ 0x00C2EF30,
//   operator delete @ 0x00BFE9BC.
// NF65 (round 7): ported onto the shared libpk headers by taking the landed 0x00C17640 OpenBankFile TU (libpk/CBankProcessor.h,
//   CScriptHost.h, PKCAutoFile.h): this function is its static ProcessBankEntry, RunProcessor 0x00C16CE0 its RunBankProcessor (q10481 /
//   nf44:117: SBankRequest settings), DeleteTrackNodeCommand 0x00C17400 its ReadMetadataBank. The self name follows that TU (one address,
//   one name; descriptive, libpk has no PDB): was the propagated ProcessEntry. ReadBankData 0x00C30F60 now binds the landed self spelling
//   ?ReadBankData@@YIXPAXPAUCBankData@@KPAVCLoadLog@@@Z (was ?FillEntry@@YIXPAVCBankFile@@...). Three changes from that TU: 0x00C17940 is called as
//   the landed PKSerialiseLoad<CBankData> template (the TU's ReadMetadataData spelling is the rejected nf44:120 claim); the real 0x00C17740
//   CBankDataOwner::GetChecked body is defined in the TU; ReadMetadataBank names the checked pointer before the RunBankProcessor call.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#include "libpk/CBankProcessor.h"
#include "libpk/PKCAutoFile.h"
// Complete genuine deferred-loader context00c16f00. Its original source scope
// is unavailable; logical four-argument static ABI is proved at the call site.
// This context body remains unmatched under the parent GS compilation policy.
#define FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#include "libpk/CScriptHost.h"
#if defined(_MSC_VER)
// RTM selects scalar allocation for POD arrays unless array operators are declared.
// Native PE imports prove array new[]/delete[]; GNU uses its standard declarations.
void* __cdecl operator new[](size_t size);
void __cdecl operator delete[](void* pointer) throw();
#endif
static long HasBankSegments(CSegmentFile* file);
static void __stdcall ReadDeferredBank(CLoadLog* log, void* file, CScriptHost* host, const SBankRequest* request)
{
    CSegmentFile reader(file, request->Chunk);
    if (reader.ErrorCount) {
        log->AddError() << "There were problems decoding the file segment structure";
        return;
    }
    long version = ReadBankInfoCount(&reader);
    if (version < 0) {
        log->AddError() << "Could not extract the version number";
        return;
    }
    if (!HasBankSegments(&reader)) {
        log->AddError() << "This bank is not a normal LUG!";
        return;
    }
    if (version != 4) {
        log->AddError() << "Expecting deferred bank version number " << 4L << "... got " << version;
        return;
    }
    if (reader.OpenSegment(CString("LHAudioBankCompData")) < 0) {
        log->AddError() << "Could not open the composite data segment";
        return;
    }
    fable_u32 compData = reader.GetPosition();
    if (reader.OpenSegment(CString("LHAudioBankLookupTable")) < 0) {
        log->AddError() << "Could not open the lookup table";
        return;
    }
    fable_i32 highestPriority;
    if (reader.Read(&highestPriority, 4) < 4) {
        log->AddError() << "Could not read the highest priority";
        return;
    }
    fable_u32 count;
    if (reader.Read(&count, 4) < 4) {
        log->AddError() << "Could not read the number of samples";
        return;
    }
    if (count == 0)
        return;
    SLookupEntry* table = new SLookupEntry[count];
    if (reader.Read(table, count * sizeof(SLookupEntry)) < (int)(count * sizeof(SLookupEntry))) {
        delete[] table;
        log->AddError() << "Could not read in the lookup table";
        return;
    }
    for (unsigned long i = 0; i < count; ++i)
        host->QueueEvent(table[i].Key, table[i].Offset + compData, table[i].Length);
    host->HighestPriority = highestPriority;
    host->Deferred = 1;
    delete[] table;
}

// Genuine complete file-local helper body recovered from both native203B and
// historicalhand433/766. Ordinary static visibility is tried before any policy.
// Native private convention consumesESI; audit must verify actual emitted ABI.
static long HasBankSegments(CSegmentFile* file)
{
    bool missing = file->OpenSegment(CString("LHAudioBankLookupTable")) < 0
                || file->OpenSegment(CString("LHAudioBankCompData")) < 0;
    return !missing;
}

void __fastcall WideToMultiByte(const unsigned short*,char*,int);
void __fastcall ReadBankData(void*,CBankData*,unsigned long,CLoadLog*);
// 0x00C17940: the retail trace string "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKSerialiseLoad.h" (0x012B2D0C) names the header template;
// the landed self row binds ??$PKSerialiseLoad@UCBankData@@@@YIPAUCBankData@@PAUIRemoteSource@@@Z (IRemoteSource has no shared header).
struct IRemoteSource;
template <class T> T* __fastcall PKSerialiseLoad(IRemoteSource* source);   // 0x00C17940 (T = CBankData)
static void __stdcall RunBankProcessor(CScriptHost* bank,CBankData* data,CLoadLog* log,const SBankRequest* settings)
{
 CBankProcessor processor(data,bank,log);
 processor.m_pCriteriaTriggers=settings->CriteriaTriggers;
 processor.m_pNumberTriggers=settings->NumberTriggers;
 processor.m_bSampleNameTriggers=settings->SampleNameTriggers;
 processor.m_bNamedTriggers=settings->NamedTriggers;
 processor.m_option76=settings->Option76;
 processor.m_option74=settings->Option74;
 processor.m_option75=settings->Option75;
 if(settings->EnableBothExtra){processor.m_option77=true;processor.m_option78=true;}
 processor.Load();
}
static void __stdcall ProcessBankEntry(const SBankRequest* settings,void* file,CLoadLog* log,CScriptHost* bank)
{
 CBankData data;
 ReadBankData(file,&data,settings->Chunk,log);
 if(log->IsOk())RunBankProcessor(bank,&data,log,settings);
}
// Genuine complete nativeC17400metadata path. No made-up ABI forwarding body.
CBankDataOwner::~CBankDataOwner(){if(Pointer){delete Pointer;Pointer=0;}}
// PKCAutoDelete.h's checked accessor 0x00C17740 (the landed self row's body, on engine/CString.h): defined in this TU like retail's
// object, so VC7.1 sees it does not touch Pointer and keeps the owned pointer in edi across the call (0x00C17400 `mov ecx,edi`).
__declspec(noinline) CBankData* CBankDataOwner::GetChecked()
{
    if (Pointer == 0) {
        {
            CString s;
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32L << ") : " << "Should have checked first..." << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(s.m_pchData);
        }
        CDebugBreak();
    }
    return Pointer;
}
static void __stdcall ReadMetadataBank(CLoadLog* log,void* file,CScriptHost* bank,const SBankRequest* settings)
{
 CBankDataOwner data(PKSerialiseLoad<CBankData>(static_cast<IRemoteSource*>(file)));
 if(!data.Pointer){log->AddError()<<"Could not load from file";return;}
 CBankData* checked=data.GetChecked();   // named: retail calls GetChecked before it pushes the log (0x00C17400)
 RunBankProcessor(bank,checked,log,settings);
}
static void __stdcall LoadBankByFormat(PKCFileManager* owner,CLoadLog* log,CScriptHost* bank,const unsigned short* path,const SBankRequest* settings)
{
 PKCAutoFile file(owner->Open(path));
 if(!file.m_pFile){log->AddError()<<"Could not open "<<path;return;}
 char name[256];WideToMultiByte(path,name,256);
 CString filename(name);CString extension=filename.Right(4);
 if(extension.CompareNoCase(".lug")==0)ProcessBankEntry(settings,file.GetChecked(),log,bank);
 else if(extension.CompareNoCase(".lut")==0)ReadDeferredBank(log,file.GetChecked(),bank,settings);
 else if(extension.CompareNoCase(".met")==0)ReadMetadataBank(log,file.GetChecked(),bank,settings);
 else log->AddError()<<"This bank is not a recognised format (.lug, .lut, .met)";
}
bool __fastcall OpenBankFile(PKCFileManager* owner,const unsigned short* path,CScriptHost* bank,const SBankRequest* settings)
{
 CLoadLog log;
 LoadBankByFormat(owner,&log,bank,path,settings);
 char name[256];WideToMultiByte(path,name,256);
 log.SetSourceName(name);
 if(!log.IsOk())return false;
 return true;
}