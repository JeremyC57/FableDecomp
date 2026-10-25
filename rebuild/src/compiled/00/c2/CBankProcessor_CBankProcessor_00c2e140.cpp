// CBankProcessor::~CBankProcessor() -- EH-framed destructor of the libpk sound-bank processor: the sample map (+0x3c, @ 0x00C3B1D0) and then the driver map (+0x0c, @ 0x00C3AC40), in reverse declaration order
// Leaf: ??1CBankProcessor@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E140, oracle length 77 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+3Ch] ; mov dword ptr [esp+10h],0 ; call ~CSampleMap ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+10h],-1 ; call ~CDriverMap ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Naming (sweep1, 2026-10-25): was landed as `SPair_00c2e140::~SPair_00c2e140` (the manifest's Dinkumware
//   `_Dest_val<allocator<pair<CCharString,CScriptInfo>>>` label is propagated); descriptive (no PDB covers libpk). The
//   bank loader 0x00C16CE0 calls it (0x00C16D6D) on the stack object that CBankProcessor::CBankProcessor 0x00C2E0C0
//   built (0x00C16D06), right after running it (0x00C2EB00); the two destroyed members are the maps that constructor
//   builds at +0x0c (0x00C3ABF0) and +0x3c (0x00C3B180). Agent436 (chunk 436).
// Shape: EH state 0 while the driver map is still alive (its unwind funclet calls ~CDriverMap directly, a member
//   with an out-of-line destructor), state -1 before the last member destructor; the rest of the record is trivially
//   destructible.
class CBankData;
class CScriptHost;
class CLoadLog;

class CDriverMap {                              // 0x30 bytes: the index map of drivers
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
    ~CBankProcessor();
};

CBankProcessor::~CBankProcessor()
{
}
