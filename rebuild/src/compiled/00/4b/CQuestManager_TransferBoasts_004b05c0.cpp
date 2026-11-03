// CQuestManager::TransferBoasts(CPersistContext& context) -- save or restore the accepted boasts: the count, then each boast's script name, id and failed / completed flags under numbered names (load rebuilds them into TempBoasts)
// Leaf: ?TransferBoasts@CQuestManager@@QAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B05C0, 1126 bytes, `ret 4`. Callees: CPersistContext::Transfer<int> 0x004B8D70
//   (??$Transfer@H@CPersistContext@@QAEXPBDAAHABH@Z, the catalog's Transfer<ENumAcceptedBoasts> is a respelling), Transfer<CCharString> /
//   <long> / <bool>, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690, CBoast copy ctor 0x004B73A0.
// Lever (chunk 728's): the three-argument CPersistContext::Transfer<T> body in the TU (VC7.1 hoists the loop default stores), the
//   CPersistTraits<T> value-initialising ctor (GetDefaultVal's construct / copy / destroy), SetScriptName by value, scalars declared before the loop.
// Grade: RELOCATION_MATCH (chunk 732, attempts b1..b11; mirror = repo + chunk 728 round 2 pending + this chunk's hdr/).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#include "fable_string.h"
#include "engine/CQuestManager.h"
#include "engine/CPersistContext.h"
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
template<class T> inline void CPersistContext::Transfer(const char* name,T& value) { Transfer(name,value,GetDefaultVal<T>()); }
template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { CPersistTraits<T> traits; return traits.DefaultVal; }
template<> inline void CPersistTraits<bool>::TransferBinaryOut(CDataOutputStream& stream,bool value) { stream.WriteEBOOL(value); }
template<> inline void CPersistTraits<CCharString>::TransferBinaryOut(CDataOutputStream& stream,CCharString value) { stream.WriteNullTerminatedString(value); }
template<class T> void CPersistContext::Transfer(const char* name,T& value,const T& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
    case MODE_LOAD_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830 ?GFIntToCharString@@YI?AVCCharString@@J@Z
CCharString __fastcall operator+(const char* left, const CCharString& right);  // 0x0099F690 ??H@YI?AVCCharString@@PBDABV0@@Z

void CQuestManager::TransferBoasts(CPersistContext& context)
{
    if (context.IsSaving()) {
        int num_accepted_boasts = AcceptedBoasts.size();
        context.Transfer("NumAcceptedBoasts", num_accepted_boasts);
        int i = 0;
        long id;
        bool failed;
        bool completed;
        for (std::list<CBoast>::iterator it = AcceptedBoasts.begin(); it != AcceptedBoasts.end(); ++it) {
            CCharString script_name = (*it).GetScriptName();
            id = (*it).GetID();
            failed = (*it).IsFailed();
            completed = (*it).IsCompleted();
            context.Transfer("BoastScriptName" + GFIntToCharString(i), script_name);
            context.Transfer("BoastId" + GFIntToCharString(i), id);
            context.Transfer("BoastFailed" + GFIntToCharString(i), failed);
            context.Transfer("BoastCompleted" + GFIntToCharString(i), completed);
            ++i;
        }
    } else {
        TempBoasts.clear();
        int num_accepted_boasts;
        context.Transfer("NumAcceptedBoasts", num_accepted_boasts);
        for (int i = 0; i != num_accepted_boasts; ++i) {
            CCharString script_name;
            long id;
            bool failed;
            bool completed;
            context.Transfer("BoastScriptName" + GFIntToCharString(i), script_name);
            context.Transfer("BoastId" + GFIntToCharString(i), id);
            context.Transfer("BoastFailed" + GFIntToCharString(i), failed);
            context.Transfer("BoastCompleted" + GFIntToCharString(i), completed);
            CBoast boast;
            boast.SetScriptName(script_name);
            boast.SetID(id);
            if (failed)
                boast.SetAsFailed();
            if (completed)
                boast.SetAsCompleted();
            TempBoasts.push_back(boast);
        }
    }
}