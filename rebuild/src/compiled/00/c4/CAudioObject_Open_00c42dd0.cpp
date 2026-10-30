// CAudioObject::Open(unsigned long caps) -- slot 3 of the libpk audio object: keep the requested caps, clear the buffer / callback / voice members and the parameter block (gain and pitch 1.0, volume 0), copy the audio system's default state callback, mark the object open (0x100000); with EAX caps (0x2000) also flag it (0x120000), make sure the EAX manager exists, allocate the 0x228-byte EAX property block and a software CEaxSource on the manager's context, clear the block and link it to the source and this object; false when any step fails
// Leaf: ?Open@CAudioObject@@UAE_NK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42DD0, 373 bytes, __fastcall `ret 4`, EH frame (handler 0x0121531B: state 0 = the
//   `new CEaxSource` block while its constructor runs; the unwind deletes it through 0x00C33100, a `jmp` to the
//   CPKObject operator delete 0x00C3C370).
// Identity (descriptive; libpk audio, no PDB / RTTI): the landed binding `?Open@CAudioObject@@UAE_NK@Z` (direct caller
//   C2DAudioObject::Open 0x00C449C0, "the base Open is 0x00C42DD0 (it stores the caps at +0x0C)"); slot 3 of the
//   CAudioObject vtable 0x012B5A88 (reference 0x012B5A94); members as the landed CAudioObject rows (m_caps +0xC, m_flags
//   +0x10, m_gain +0x18, m_volume +0x1C, m_pVoice +0x50, m_pCurrentBuffer +0x78, m_pfnQueueCallback +0xD4,
//   m_stateCallback +0xDC / data +0xE0, m_status +0xF0). The catalog label CTCHeroStats_InitializeShadowRenderer is
//   propagated and wrong.
// Callees (landed spellings): the audio system g_audioSystem 0x0143BB58 (`class CAudioSystem`, as C2DAudioObject::Open)
//   running GetPair80 0x00C3D2D0 (`CVtable_012B5870::GetPair80`, the default state callback pair) and 0x00C3DF10
//   (`CTCHeroStats_00c3df10::CreateIndexBuffer`, which builds the 0xF0-byte EAX manager into the system's +0x18 =
//   g_pEAXManager 0x0143BB70 and returns it) -- both landed under placeholder class names, hence the casts;
//   CPKObject::operator new 0x00C3C360 (the 0x228-byte property block and the 0x3C0-byte CEaxSource); the CEaxSource
//   constructor 0x00C61610 (vtable 0x012B9344: context +0x14, a +0x370, b +0x378, c +0x10, result out-pointer).
// Shape: the parameter block (+0x14, ten dwords) is cleared with the memset intrinsic (pointer in eax, stores through
//   edx) followed by its first-word store through the same register, as in C2DAudioObject::Open; the CEaxSource result
//   out-parameter is the `push ecx` frame word; the `new` result is homed in the dead caps argument slot. The caps are
//   re-read into a local right after the clear (retail loads m_caps before the gain / volume / flag stores, with 1.0f in
//   ecx; testing m_caps in the `if` puts 1.0f in eax and the load after the stores); the manager's context goes through
//   an inline accessor (retail loads the global and the member before pushing the result pointer). Grades: 3.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};
class CEaxContext;
class CEaxSource : public CPKObject {               // vtable 0x012B9344, 0x3C0 bytes
public:
    CEaxSource(CEaxContext* context, unsigned long a, unsigned long b, long c, long* pResult);   // 0x00C61610
    virtual ~CEaxSource();                          // 0x00C60B90
    unsigned char _body[0x3bc];
};
class CEAXManager {                                 // 0xF0 bytes, built by 0x00C3DF10 into g_pEAXManager
public:
    unsigned char _pad[0xe4];
    CEaxContext* m_pContext;                        // +0xe4
    CEaxContext* GetContext() const { return m_pContext; }
};
extern CEAXManager* g_pEAXManager;                  // 0x0143BB70

class CAudioSystem { };                             // the static audio system object (ctor 0x00C3D7D0, vtable 0x012B5870)
extern CAudioSystem g_audioSystem;                  // 0x0143BB58
struct CVtable_012B5870 {
    void GetPair80(long* first, long* second) const;   // 0x00C3D2D0
};
struct CIndexBuffer_00c3df10;
struct CTCHeroStats_00c3df10 {
    CIndexBuffer_00c3df10* CreateIndexBuffer();     // 0x00C3DF10
};

struct SEaxSourceProps {                            // 0x228 bytes, zeroed after allocation
    unsigned char _pad[0x1f0];
    CEaxSource*   m_pSource;                        // +0x1f0
    unsigned char _pad1f4[0x21c - 0x1f4];
    void*         m_pOwner;                         // +0x21c the audio object
    unsigned char _pad220[8];
};
struct SAudioParams {                               // +0x14 of the audio object, ten dwords
    long  m_14;                                     // +0x00
    float m_gain;                                   // +0x04 (+0x18) linear gain
    long  m_volume;                                 // +0x08 (+0x1c) hundredths of a decibel
    float m_pitch;                                  // +0x0c (+0x20)
    void* m_records[6];                             // +0x10 (+0x24..+0x38) current parameter records (0 = none)
};

class CVoice;
class CSoundBuffer;
class CAudioObject;
typedef void (__stdcall *TStateCallback)(CAudioObject* object, unsigned long flags, void* data);
class CAudioObject {
public:
    virtual ~CAudioObject();                        // slot 0
    virtual bool Is3D() const;                      // slot 1
    virtual bool IsOpen() const;                    // slot 2
    virtual bool Open(unsigned long caps);          // slot 3
    unsigned long  m_node[2];                       // +0x04
    unsigned long  m_caps;                          // +0x0c
    unsigned long  m_flags;                         // +0x10 state bits, 0x100000 open, 0x20000 EAX
    SAudioParams   m_params;                        // +0x14
    unsigned char  m_pad3c[0x50 - 0x3c];
    CVoice*        m_pVoice;                        // +0x50
    unsigned char  m_pad54[0x78 - 0x54];
    CSoundBuffer*  m_pCurrentBuffer;                // +0x78
    unsigned char  m_pad7c[0xd4 - 0x7c];
    void*          m_pfnQueueCallback;              // +0xd4
    void*          m_queueCallbackData;             // +0xd8
    TStateCallback m_stateCallback;                 // +0xdc
    void*          m_stateCallbackData;             // +0xe0
    long           m_e4;                            // +0xe4
    long           m_e8;                            // +0xe8
    long           m_ec;                            // +0xec
    long           m_status;                        // +0xf0
    SEaxSourceProps* m_pEaxProps;                   // +0xf4
    CEaxSource*    m_pEaxSource;                    // +0xf8
};

bool CAudioObject::Open(unsigned long caps)
{
    m_caps = caps;
    m_pCurrentBuffer = 0;
    m_pfnQueueCallback = 0;
    m_queueCallbackData = 0;
    ((const CVtable_012B5870*)&g_audioSystem)->GetPair80((long*)&m_stateCallback, (long*)&m_stateCallbackData);
    m_e4 = 0;
    m_ec = 0;
    m_pVoice = 0;
    memset(&m_params, 0, sizeof(m_params));
    m_params.m_14 = 0;
    unsigned long requested = m_caps;
    m_params.m_volume = 0;
    m_params.m_gain = 1.0f;
    m_params.m_pitch = 1.0f;
    m_flags = 0x100000;
    m_status = 0;
    if (requested & 0x2000) {
        m_flags = 0x120000;
        if (g_pEAXManager == 0 && ((CTCHeroStats_00c3df10*)&g_audioSystem)->CreateIndexBuffer() == 0)
            return false;
        m_pEaxProps = (SEaxSourceProps*)CPKObject::operator new(sizeof(SEaxSourceProps));
        if (m_pEaxProps == 0)
            return false;
        long result;
        m_pEaxSource = new CEaxSource(g_pEAXManager->GetContext(), 0, 0, 1, &result);
        if (m_pEaxSource == 0)
            return false;
        memset(m_pEaxProps, 0, sizeof(SEaxSourceProps));
        m_pEaxProps->m_pSource = m_pEaxSource;
        m_pEaxProps->m_pOwner = this;
    }
    return true;
}