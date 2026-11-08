// CSystem::LoadBank(const unsigned short* fileName, const CBankParams_00c08d40& params) -- libpk CSystem.cpp bank factory (CSystem vtable 0x012B1E24 slot 10): under the module lock, create a bank object, wire its two lock pairs from the params, round the system's bank size up to the bank's alignment, load it from the file, and for a .lug / .lut file resolve its handle through the system's loader
// Shape: PKCAutoDelete::GetChecked 0x00C09920 and Release 0x00C099F0 get same-TU `#pragma auto_inline(off)` bodies (the landed row text); their mod/ref lets the failure paths reuse esi for the bank instead of reloading the holder, which also gives retail's holder/name frame slots
// Leaf: ?LoadBank@CSystem@@UAEPAVCUnprovenPKObject_00c099f0@@PBGABUCBankParams_00c08d40@@@Z
// Flags: /EHsc /GS
void* __cdecl operator new(unsigned int size);                 // 0x00BFEA1A
void __cdecl operator delete(void* p);

extern char* g_pchNil;                                         // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;                          // 0x0143BB2C

class CString {                                                // libpk's MFC-style string (one word)
public:
    CString() : m_pchData(g_pchNil) { }
    ~CString();                                                // 0x00C0BA40
    const CString& operator=(const char* lpsz);                // 0x00C0C200
    CString Right(int nCount) const;                           // 0x00C0E0F0
    int CompareNoCase(const char* lpsz) const;                 // 0x00C0BD60
    CString& __fastcall operator<<(const char* text);          // 0x00C0DEC0
    CString& __fastcall operator<<(long value);                // 0x00C0DF00
    char* m_pchData;
};
struct CDebugBreak {
    CDebugBreak();                                             // 0x00C16750
};
void __fastcall WideToMultiByte(const unsigned short* source, char* dest, int destSize);   // 0x00C16760

#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;                     // 0x0143BB14

class CLockPair_00c10100 {                                     // 0x20 bytes
public:
    void Lock();                                               // 0x00C10100
    void Unlock();                                             // 0x00C100F0
    unsigned char m_body[0x20];
};

class CSystem;
class CUnprovenPKObject_00c099f0 {                             // vtable 0x012B25A8, 0xA4 bytes
public:
    CUnprovenPKObject_00c099f0(CSystem* owner);                // 0x00C0EDF0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual ~CUnprovenPKObject_00c099f0();                     // slot 6 (+0x18)
    unsigned long GetAlignment() const;                        // 0x00C0E6E0
    void Lock();                                               // 0x00C0E5E0
    void Unlock();                                             // 0x00C0E5C0
    void*              m_pStreamFile;                          // +0x04 the open file of a .lug / .lut bank
    CSystem*           m_pOwner;                               // +0x08
    CLockPair_00c10100 m_locks;                                // +0x0c
    unsigned char      m_body[0xa4 - 0x2c];
};

template <class T> class PKCAutoDelete {
public:
    PKCAutoDelete(T* object) : m_pObject(object) { }
    ~PKCAutoDelete() { if (m_pObject) delete m_pObject; }
    T* GetChecked();                                           // 0x00C09920
    T* Release();                                              // 0x00C099F0
    T* m_pObject;
};


#pragma auto_inline(off)
template <class T> T* PKCAutoDelete<T>::GetChecked()
{
    if (m_pObject == 0) {
        {
            CString line;
            line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32L << ") : " << "Should have checked first..." << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    return m_pObject;
}
template <class T> T* PKCAutoDelete<T>::Release()
{
    if (m_pObject == 0) {
        {
            CString line;
            line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 42L << ") : " << "Shouls have checked first..." << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    T* object = m_pObject;
    m_pObject = 0;
    return object;
}
#pragma auto_inline(on)

struct CBankParams_00c08d40 {
    long          m_sourceA;                                   // +0x00
    long          m_sourceB;                                   // +0x04
    unsigned char m_8, m_9, m_a, m_pad0;
    unsigned char m_c, m_d, m_e, m_pad1;
};

struct CBankLoadDesc_00c08d40 {
    CLockPair_00c10100* m_pLocksA;
    CLockPair_00c10100* m_pLocksB;
    unsigned long       m_size;
    unsigned char       m_0c, m_0d, m_0e, m_0f, m_10, m_11;
    CBankLoadDesc_00c08d40() : m_pLocksA(0), m_pLocksB(0), m_size(0x1000) { }
};

struct CProcessor_00c09110 {                                   // the bank loader (landed Process_00c172f0 spelling)
    virtual void* Open(const unsigned short* fileName);       // slot 0: open the bank file by name
    bool __fastcall Load(const unsigned short* fileName, CUnprovenPKObject_00c099f0* bank, CBankLoadDesc_00c08d40* desc);   // 0x00C17640
};

class CSystem {                                                // vtable 0x012B1E24
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CUnprovenPKObject_00c099f0* LoadBank(const unsigned short* fileName, const CBankParams_00c08d40& params);   // slot 10
    unsigned char         m_04[0x38];
    CLockPair_00c10100    m_locks;                             // +0x3c
    CProcessor_00c09110*  m_pLoader;                           // +0x5c
    unsigned char         m_60[0x20];
    unsigned long         m_bankSize;                          // +0x80
};

CUnprovenPKObject_00c099f0* CSystem::LoadBank(const unsigned short* fileName, const CBankParams_00c08d40& params)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CUnprovenPKObject_00c099f0* bank = new CUnprovenPKObject_00c099f0(this);
    PKCAutoDelete<CUnprovenPKObject_00c099f0> autoBank(bank);
    if (bank == 0) {
        {
            CString line;
            line << ".\\CSystem.cpp" << "(" << 54L << ") : " << "EMEM" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    CBankLoadDesc_00c08d40 desc;
    if (params.m_sourceB == 1)
        desc.m_pLocksB = &m_locks;
    else if (params.m_sourceB == 2)
        desc.m_pLocksB = &bank->m_locks;
    if (params.m_sourceA == 1)
        desc.m_pLocksA = &m_locks;
    else if (params.m_sourceA == 2)
        desc.m_pLocksA = &bank->m_locks;
    desc.m_0c = params.m_8;
    desc.m_0d = params.m_9;
    desc.m_0e = params.m_a;
    desc.m_0f = params.m_c;
    desc.m_10 = params.m_d;
    desc.m_11 = params.m_e;
    unsigned long alignment = bank->GetAlignment();
    desc.m_size = (m_bankSize + alignment - 1) / alignment * alignment;
    if (desc.m_pLocksA)
        desc.m_pLocksA->Lock();
    if (desc.m_pLocksB)
        desc.m_pLocksB->Lock();
    bank->Lock();
    bool loaded = m_pLoader->Load(fileName, autoBank.GetChecked(), &desc);
    bank->Unlock();
    if (desc.m_pLocksA)
        desc.m_pLocksA->Unlock();
    if (desc.m_pLocksB)
        desc.m_pLocksB->Unlock();
    if (!loaded)
        return 0;
    CString name;
    char buffer[0x100];
    WideToMultiByte(fileName, buffer, 0x100);
    name = buffer;
    if (name.Right(4).CompareNoCase(".lug") == 0 || name.Right(4).CompareNoCase(".lut") == 0) {
        void* file = m_pLoader->Open(fileName);
        if (file == 0)
            return 0;
        bank->m_pStreamFile = file;
    }
    return autoBank.Release();
}