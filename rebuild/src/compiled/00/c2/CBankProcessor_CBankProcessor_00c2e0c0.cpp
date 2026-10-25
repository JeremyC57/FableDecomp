// CBankProcessor::CBankProcessor(CBankData* data, CScriptHost* manager, CLoadLog* log) -- EH-framed constructor of the libpk sound-bank processor: the bank data, the host (CScriptHost, vtable 0x012B25A8) and the load log at +0/+4/+8, the driver map (+0x0c) and the sample map (+0x3c) built out of line, then the two trigger registries (+0x6c/+0x70) and the seven option bytes (+0x74..+0x7a) zeroed
// Leaf: ??0CBankProcessor@@QAE@PAVCBankData@@PAVCScriptHost@@PAVCLoadLog@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E0C0, oracle length 120 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov eax,[a] ; mov edx,[c] ; push ebx ; push esi ; mov esi,ecx ; mov ecx,[b] ; mov [esi+4],ecx ; lea ecx,[esi+0Ch] ; mov [esp+8],esi ; mov [esi],eax ; mov [esi+8],edx ; call CDriverMap::CDriverMap ; xor ebx,ebx ; lea ecx,[esi+3Ch] ; mov [esp+14h],ebx ; call CSampleMap::CSampleMap ; mov ecx,[esp+0Ch] ; mov [esi+6Ch],ebx ; mov [esi+70h],ebx ; mov [esi+74h],bl ; ... ; mov [esi+7Ah],bl ; mov eax,esi ; pop esi ; pop ebx ; mov fs:[0],ecx ; add esp,10h ; ret 0Ch
// Naming (sweep1, 2026-10-25): was landed as CSubPrimitiveList::CSubPrimitiveList (a propagated manifest label);
//   descriptive (no PDB covers libpk). Its only caller, the bank loader 0x00C16CE0, builds it on the stack (0x00C16D06),
//   copies the loader options into +0x6c..+0x7a, runs it (0x00C2EB00, the step sequence that calls the landed
//   CBankProcessor::AddDrivers 0x00C2E480 / AddNumberTriggers 0x00C2E4D0 / AddEvents 0x00C2EA30 / AddSubEvents
//   0x00C2EA70) and destroys it (0x00C2E140). The +0x0c map is the one CBankProcessor::AddDriver 0x00C2E2B0 adds the
//   drivers to (CIndexMap Add 0x00C3AB50); the +0x3c map is the one AddSample 0x00C2E190 adds the samples to (Add
//   0x00C3B0E0) and AddDriver looks the samples up in (Get 0x00C3B160). Agent436 (chunk 436) layout.
// No vtable store: a plain record. The zero register is shared by the EH state store and the
// trailing initialisers, which is why they follow the second constructor.
class CBankData;
class CScriptHost;
class CLoadLog;

class CDriverMap {                              // 0x30 bytes: the index map of drivers (vtable 0x012B5578)
    unsigned char _body[0x30];
public:
    CDriverMap();                               // 0x00C3ABF0
    ~CDriverMap();                              // 0x00C3AC40
};

class CSampleMap {                              // 0x30 bytes: the index map of samples
    unsigned char _body[0x30];
public:
    CSampleMap();                               // 0x00C3B180
    ~CSampleMap();                              // 0x00C3B1D0
};

class CBankProcessor {
public:
    CBankData*     m_pData;                     // +0x00
    CScriptHost* m_pManager;                  // +0x04
    CLoadLog*      m_pLog;                      // +0x08
    CDriverMap     m_drivers;                   // +0x0c
    CSampleMap     m_samples;                   // +0x3c
    void*          m_pCriteriaTriggers;         // +0x6c
    void*          m_pNumberTriggers;           // +0x70
    bool m_option74, m_option75, m_option76, m_option77, m_option78, m_option79, m_option7a;   // +0x74..+0x7a
    CBankProcessor(CBankData* data, CScriptHost* manager, CLoadLog* log);
};

CBankProcessor::CBankProcessor(CBankData* data, CScriptHost* manager, CLoadLog* log)
    : m_pData(data), m_pManager(manager), m_pLog(log)
{
    m_pCriteriaTriggers = 0;
    m_pNumberTriggers = 0;
    m_option74 = false;
    m_option75 = false;
    m_option76 = false;
    m_option77 = false;
    m_option78 = false;
    m_option79 = false;
    m_option7a = false;
}
