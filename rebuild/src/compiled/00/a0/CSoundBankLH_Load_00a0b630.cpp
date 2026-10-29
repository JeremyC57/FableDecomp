// CSoundBankLH::Load(const CWideString& fileName) -- unless a bank is already loaded and provided the sound system has an engine, convert the name to a full path and load the bank through the engine with a fixed 16-byte descriptor, keeping the handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B630, length 130 bytes (`ret 4`, + int3 padding).
// Vtable 0x0129C9DC (CSoundBankLH, rtti-vtables.tsv) slot 1. Callees: CAFile::ConvertPathToFullPath @ 0x00999110 (`__fastcall`,
// hidden result in ecx, path in edx), CWideString::GetAsCharArray() @ 0x0099AF10, ~CWideString @ 0x0099B510, the
// engine's virtual slot 10 (+0x28). Name: the landed callers 0x0042E17C and 0x00A3E430 call this slot `Load`.
// Leaf: ?Load@CSoundBankLH
// Layout: as the landed constructor 0x00A0BAC0 / sibling 0x00A0B710 -- the sound system at +0x0c (its engine object at
// +0x40), the loaded bank handle at +0x10.
// Shape: the sibling 0x00A0B710 plus the "already loaded" test (`xor eax,eax ; cmp [esi+10h],eax ; jne`, the zero register
// reused for the engine test and the descriptor's zero bytes); the converted name lives in the dead argument slot
// (`lea ecx,[esp+20h]` = the `name` slot); the descriptor is {2, 2} then seven flag bytes (+0x08..+0x0e) of which
// +0x0a and +0x0c..+0x0e are set; the engine pointer and its vtable are re-read after the conversion.
// The descriptor has an inline defaulting constructor ({2, 2, all flags clear}) and the body sets four flags in the
// order +0x0e, +0x0a, +0x0d, +0x0c: that is what puts retail's surviving zero bytes (+8, +9, +0xb) BEFORE the two 2s
// and keeps eax as the zero register (explicit field-by-field stores schedule the 2s first and swap eax/ecx).
class CWideString {
public:
    void* m_pData;
    const unsigned short* GetAsCharArray() const;   // 0x0099AF10
    ~CWideString();                           // 0x0099B510
};
class CAFile {
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
};

struct SBankLoadDesc {
    long a;                                   // +0x0 = 2
    long b;                                   // +0x4 = 2
    bool f0, f1, f2, f3, f4, f5, f6;          // +0x8..+0xe
    SBankLoadDesc() : a(2), b(2), f0(false), f1(false), f2(false), f3(false), f4(false), f5(false), f6(false) { }
};
struct CSoundEngineLH {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void* LoadBank(const unsigned short* name, const SBankLoadDesc* desc);  // slot 10 (+0x28)
};
struct CSoundSystemLH { unsigned char _pad[0x40]; CSoundEngineLH* m_pEngine; };   // engine at +0x40
struct CSoundBankLH {                         // vtable 0x0129C9DC
    virtual void v00();
    long            m_4;                      // +0x04
    long            m_8;                      // +0x08
    CSoundSystemLH* m_pSystem;                // +0x0c
    void*           m_pBank;                  // +0x10 the engine's handle of the loaded bank
    virtual void Load(const CWideString& fileName);   // slot 1
};

void CSoundBankLH::Load(const CWideString& fileName)
{
    if (m_pBank == 0 && m_pSystem->m_pEngine) {
        SBankLoadDesc d;
        d.f6 = true;
        d.f2 = true;
        d.f5 = true;
        d.f4 = true;
        CWideString w = CAFile::ConvertPathToFullPath(fileName);
        CSoundEngineLH* engine = m_pSystem->m_pEngine;
        m_pBank = engine->LoadBank(w.GetAsCharArray(), &d);
    }
}