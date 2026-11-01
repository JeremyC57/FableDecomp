# h24_cstr.py: naming wave 24, family cstr -- the libpk CString (DECISIONS_W23_RESOLVED item 9 and the remaining 00c0dec0 views)
#  0x00C0C0C0 re-authored as CString::operator=(const CString&) ??4CString@@QAEABV0@ABV0@@Z, the spelling 0x00C2E960 binds and the
#    landed CString::AssignCopy 0x00C0C060 / operator=(const char*) 0x00C0C200 rows name (MFC strcore.cpp operator=; was a propagated
#    CCharString::Assign). The nil header 0x0139C8FC is _NullString: retail's own assert text 0x012B20A8.. "stringSrc.GetData() !=
#    _NullString" names it, and the landed CString(const CString&) 0x00C0DAB0 binds ?_NullString@@3PAUCStringData@@A.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('00c0c0c0', 'src', '''// CString::operator=(const CString& stringSrc) -- MFC-style assignment: when either string is locked (negative reference count, the nil
// header excepted) copy the characters through AssignCopy, else release ours and share the source's data (InterlockedIncrement)
// Leaf: ??4CString@@QAEABV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0C0C0, __thiscall `ret 4`. /O2 /Oy.
// Naming (naming wave 24, DECISIONS_W23_RESOLVED item 9): the libpk CString's operator=(const CString&), the spelling its caller
//   0x00C2E960 binds and the landed AssignCopy 0x00C0C060 / operator=(const char*) 0x00C0C200 rows name (MFC strcore.cpp's
//   CString::operator=); was landed as a propagated CCharString::Assign. The nil header 0x0139C8FC is _NullString: retail's own assert
//   text "stringSrc.GetData() != _NullString" names it and the landed CString(const CString&) 0x00C0DAB0 binds that spelling; the assert
//   hook 0x0143BB2C is g_pfnAssertHook (the landed ~CString 0x00C0BA40).
// Callees: CString::AssignCopy 0x00C0C060, CString::Release 0x00C0B910, InterlockedIncrement, CDebugBreak::CDebugBreak (in this TU, as
//   the row landed). Literals "m_pchData != NULL\\n", "stringSrc.GetData() != _NullString\\n".
// Layout: the object is one char pointer; the 12-byte CStringData header (nRefs, nDataLength, nAllocLength) precedes the characters.
// Flags: /O2 /Oy
#include <stddef.h>
struct CStringData { long nRefs; int nDataLength, nAllocLength; };
extern CStringData* _NullString;                    // 0x0139C8FC
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;               // 0x0143BB2C
struct CDebugBreak { CDebugBreak(); };
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#pragma auto_inline(off)
CDebugBreak::CDebugBreak() { DebugBreak(); }
#pragma auto_inline(on)
#define LIB_ASSERT(cond) \\
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\\n"); CDebugBreak token; }
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long*);
class CString {
public:
    char* m_pchData;
    __forceinline CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void Release();                                 // 0x00C0B910
    void AssignCopy(int nSrcLen, const char* lpszSrcData);   // 0x00C0C060
    const CString& operator=(const CString& stringSrc);
};

const CString& CString::operator=(const CString& stringSrc)
{
    if (m_pchData != stringSrc.m_pchData) {
        if ((GetData()->nRefs < 0 && GetData() != _NullString) || stringSrc.GetData()->nRefs < 0) {
            AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
        } else {
            Release();
            LIB_ASSERT(stringSrc.GetData() != _NullString)
            m_pchData = stringSrc.m_pchData;
            InterlockedIncrement(&GetData()->nRefs);
        }
    }
    return *this;
}''')

# 0x00C17260 (DECISIONS_W23_RESOLVED item 8): LoadBank, the same-TU static of its only caller 0x00C172F0, which already defines and binds
#   ?LoadBank@@YGXPAVCLoadLog@@PAXPAUCRequest_00c09110@@@Z with the identical body. libpk has no PDB, so both names are descriptive; the
#   caller's is the one the other landed rows corroborate: 0x00C307C0 is bound as CLoadLog::IsOk (0x00C2E480), 0x00C2EED0 / 0x00C2EF30 as
#   the CBankData ctor / dtor (0x00C17940), 0x00C30F60 takes (.., CBankData*, unsigned long, CLoadLog*) (0x00C16D90), and 0x00C0DEC0 is the
#   CString stream operator<< (DECISIONS_W21). The own row's CInventoryOwner / CRecord0x30 / CLoader / AppendText model came from the
#   propagated AI label. The own row now spells its body as the caller does (one retail address, one name per callee).
he.put('00c17260', 'src', '''// LoadBank(CLoadLog* log, void* file, CRequest_00c09110* request) -- libpk bank load step: read the bank data of the open file (chunk
// 0x2000) into a stack CBankData, logging into `log`; when the log is still good but the meta data does not serialise into the request,
// queue the error "There was a problem serialising the extracted meta data."
// Leaf: ?LoadBank@@YGXPAVCLoadLog@@PAXPAUCRequest_00c09110@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17260, length 138 bytes (+ int3 padding), `ret 8`, EH frame (`push -1 ; push offset ;
//   mov eax,fs:[0]`, state -1/0 around the 0x30-byte CBankData).
// Naming (naming wave 24, DECISIONS_W23_RESOLVED item 8): a same-TU static of its only caller 0x00C172F0 (the libpk bank-file load),
//   which defines the identical body and binds this spelling. libpk has no PDB / RTTI, so the name is descriptive; it is the caller's
//   model the other landed rows corroborate (CLoadLog::IsOk 0x00C307C0 bound by 0x00C2E480, the CBankData ctor / dtor bound by
//   0x00C17940, the CLoadLog* last argument of 0x00C30F60 bound by 0x00C16D90, the CString stream operator<< 0x00C0DEC0 per
//   DECISIONS_W21). Was the descriptive LoadRecordAndReport_00c17260 on a propagated AI model (CInventoryOwner / CRecord0x30 / CLoader /
//   AppendText).
// Shape: the TU-local static gets VC7.1's private convention (the log in esi: the caller does `lea esi,[esp+14h] ; call`); a same-TU
//   caller below (not graded) makes VC7.1 choose it; `__stdcall` gives the callee-pop `ret 8`.
// Callees: CBankData ctor 0x00C2EED0 / dtor 0x00C2EF30, ReadBankData 0x00C30F60 (__fastcall: file ecx, data edx, chunk and log on the
//   stack, `ret 8`), CLoadLog::IsOk 0x00C307C0 (`mov al,[ecx+30h]`), SerialiseBankMetaData 0x00C17820 (__fastcall(data, request)),
//   CLoadLog::AddError 0x00C30840, CString::operator<< 0x00C0DEC0 (__fastcall, the text 0x012B2CD0).
// Flags: /EHsc
class CString { public:                    // libpk trace line (shared buffer)
    char* m_pBuffer;
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
};
class CLoadLog {                           // vtable 0x012B4CC0, 0x34 bytes
public:
    virtual void v00();
    unsigned char _pad04[0x2c];
    bool m_ok;                             // +0x30
    CLoadLog();                            // 0x00C308F0
    ~CLoadLog();                           // 0x00C30940
    CString& AddError();                   // 0x00C30840 (descriptive: queues an error message, clears m_ok)
    bool IsOk() const;                     // 0x00C307C0
};
struct CRequest_00c09110;
class CBankData { public:                  // 0x30 bytes on LoadBank's stack
    long _words[12];
    CBankData();                           // 0x00C2EED0
    ~CBankData();                          // 0x00C2EF30
};
bool __fastcall SerialiseBankMetaData(CBankData* data, CRequest_00c09110* request);   // 0x00C17820 (descriptive)
void __fastcall ReadBankData(void* file, CBankData* data, unsigned long chunk, CLoadLog* log);   // 0x00C30F60 (descriptive)

static __declspec(noinline) void __stdcall LoadBank(CLoadLog* log, void* file, CRequest_00c09110* request)
{
    CBankData data;
    ReadBankData(file, &data, 0x2000, log);
    if (log->IsOk()) {
        if (!SerialiseBankMetaData(&data, request))
            log->AddError() << "There was a problem serialising the extracted meta data.";
    }
}
// Same-TU caller (0x00C172F0 passes its stack CLoadLog in esi), present so the static gets the private convention. Not graded.
void LoadBank_SameTUCaller(void* file, CRequest_00c09110* request)
{
    CLoadLog log;
    LoadBank(&log, file, request);
}''')

# ---- the remaining 00c0dec0 views (DECISIONS_W21_RESOLVED: the libpk error stream is CString) ----
# 0x00C309E0: `add ecx,4 ; jmp ~CString 0x00C0BA40`, called only from the scalar deleting destructor 0x00C30A90: the destructor of a
#   record holding a CString at +4 (class unidentified; the descriptive CClass_00c309e0 kept). Was a Forward_00c309e0 to a stand-in
#   CObject_00c0ba40::Method_00c0ba40.
he.put('00c309e0', 'src', '''// CClass_00c309e0::~CClass_00c309e0() -- destroy the CString member at +4 (the record's only non-trivial member)
// Leaf: ??1CClass_00c309e0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C309E0, 8 bytes: `add ecx,4 ; jmp 0xc0ba40`.
// Naming (naming wave 24, the 00c0dec0 CString views): 0x00C0BA40 is the libpk CString destructor ??1CString@@QAE@XZ (DECISIONS_W21; the
//   landed own row), and the only caller is the scalar deleting destructor 0x00C30A90, so this is the destructor of a record holding a
//   CString at +4 (its class is not identified: libpk has no PDB / RTTI; CClass_00c309e0 is descriptive). Was a tail-forward
//   Forward_00c309e0 to a stand-in CObject_00c0ba40::Method_00c0ba40.
class CString {
public:
    char* m_pchData;
    ~CString();                     // 0x00C0BA40
};

class CClass_00c309e0 {
public:
    unsigned char _pad[0x4];
    CString m_member;               // +0x04
    ~CClass_00c309e0();
};

CClass_00c309e0::~CClass_00c309e0()
{
}''')

# 0x00C3BB50: the bank sample table reader logs into the libpk CLoadLog (0x00C30840 AddError, 0x00C307C0 IsOk, as 0x00C17260 / 0x00C172F0)
#   through the CString stream operators 0x00C0DEC0 (const char*) / 0x00C0DF10 (unsigned long, bound by 0x00C4C080); the segment name
#   is a CString temporary (0x00C0BFE0 / 0x00C0BA40). Was String / Log / Context stand-ins.
A = '00c3bb50'
he.edit(A, 'src', '''struct String{char* data;String(const char*);~String();};
struct Reader{int Find(const String&);int Read(void*,unsigned);};
struct Log{Log& __fastcall Text(const char*);Log& __fastcall Number(unsigned);};
struct Context{Log& CreateErrorText_00c30840();bool Good();};
struct Callback{virtual void Resource(Context*,const void*)=0;};
struct AudioBankTableReader_00c3bb50{Reader* reader;void Load(Context*,Callback*);};
void AudioBankTableReader_00c3bb50::Load(Context* context,Callback* callback){
 if(reader->Find(String("LHAudioBankSampleTable"))<0){context->CreateErrorText_00c30840().Text("Could not find the bank sample table segment");return;}
 unsigned count;if(reader->Read(&count,4)<4){context->CreateErrorText_00c30840().Text("Could not read the number of samples!");return;}''',
'''// Naming (naming wave 24, the 00c0dec0 CString views): the log is the libpk CLoadLog (AddError 0x00C30840 / IsOk 0x00C307C0, as the landed
//   LoadBank 0x00C17260 and 0x00C172F0) written through the CString stream operators 0x00C0DEC0 (const char*) and 0x00C0DF10 (unsigned
//   long, bound ??6CString@@QAIAAV0@K@Z by 0x00C4C080); the segment name is a CString temporary (0x00C0BFE0 / ~CString 0x00C0BA40).
//   Was String / Log::Text / Log::Number / Context stand-ins.
class CString{public:char* m_pchData;CString(const char*);~CString();CString& __fastcall operator<<(const char*);CString& __fastcall operator<<(unsigned long);};
struct Reader{int Find(const CString&);int Read(void*,unsigned);};
class CLoadLog{public:CString& AddError();bool IsOk() const;};
struct Callback{virtual void Resource(CLoadLog*,const void*)=0;};
struct AudioBankTableReader_00c3bb50{Reader* reader;void Load(CLoadLog*,Callback*);};
void AudioBankTableReader_00c3bb50::Load(CLoadLog* context,Callback* callback){
 if(reader->Find(CString("LHAudioBankSampleTable"))<0){context->AddError()<<"Could not find the bank sample table segment";return;}
 unsigned count;if(reader->Read(&count,4)<4){context->AddError()<<"Could not read the number of samples!";return;}''')
he.edit(A, 'src', '''{context->CreateErrorText_00c30840().Text("Could not read in resource ").Number(index);return;}''', '''{context->AddError()<<"Could not read in resource "<<(unsigned long)index;return;}''')
he.edit(A, 'src', 'if(!context->Good())return;', 'if(!context->IsOk())return;')

# 0x00C3BCB0: the criteria loader, the same log and stream (AppendErrorText(CriteriaLog*, text) was the __fastcall operator<<).
A = '00c3bcb0'
he.edit(A, 'src', '''struct CriteriaText{char*data;CriteriaText(const char*);~CriteriaText();};
// Layout: error factory00c30840 allocates12-byte entry and returns its string field+4. Descriptive helper names; original names unresolved.
struct CriteriaLog{char*data;};
struct CriteriaContext{CriteriaLog*CreateErrorText();bool Continue();};
CriteriaLog* __fastcall AppendErrorText(CriteriaLog*,const char*);
struct CriteriaReader{int FindSegment(const CriteriaText&);int Read(void*,int);};
struct CriteriaCallback{virtual void Entry(CriteriaContext*,const char*,unsigned,const unsigned*)=0;};
struct AudioCriteriaLoader_00c3bcb0{CriteriaReader*reader;void LoadCriteria(CriteriaContext*,CriteriaCallback*);};
void AudioCriteriaLoader_00c3bcb0::LoadCriteria(CriteriaContext*context,CriteriaCallback*callback){''', '''// Layout: error factory00c30840 allocates12-byte entry and returns its string field+4. Descriptive helper names; original names unresolved.
// Naming (naming wave 24, the 00c0dec0 CString views): the log is the libpk CLoadLog (AddError 0x00C30840 returns the entry's CString,
//   IsOk 0x00C307C0) and the text goes through CString::operator<<(const char*) 0x00C0DEC0 (__fastcall); the segment name is a CString
//   temporary (0x00C0BFE0 / ~CString 0x00C0BA40). Was CriteriaText / CriteriaLog / CriteriaContext / AppendErrorText stand-ins.
class CString{public:char*m_pchData;CString(const char*);~CString();CString& __fastcall operator<<(const char*);};
class CLoadLog{public:CString& AddError();bool IsOk() const;};
struct CriteriaReader{int FindSegment(const CString&);int Read(void*,int);};
struct CriteriaCallback{virtual void Entry(CLoadLog*,const char*,unsigned,const unsigned*)=0;};
struct AudioCriteriaLoader_00c3bcb0{CriteriaReader*reader;void LoadCriteria(CLoadLog*,CriteriaCallback*);};
void AudioCriteriaLoader_00c3bcb0::LoadCriteria(CLoadLog*context,CriteriaCallback*callback){''')
import re
p, rel, t = he._rw(A, 'src')
t2, n = re.subn(r'AppendErrorText\(context->CreateErrorText\(\),("[^"]*")\);', r'context->AddError()<<\1;', t)
if n: he._w(p, t2); print('ok', A, 'src', n)
else: print('already', A, 'src')
he.edit(A, 'src', 'CriteriaText("LHAudioBankCriteiaInfo")', 'CString("LHAudioBankCriteiaInfo")')
he.edit(A, 'src', 'if(!context->Continue())return;', 'if(!context->IsOk())return;')
