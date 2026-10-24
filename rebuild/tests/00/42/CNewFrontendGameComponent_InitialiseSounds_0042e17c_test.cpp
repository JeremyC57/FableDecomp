// Behaviour test for CNewFrontendGameComponent::InitialiseSounds (0x0042E17C): with a sound manager the component owns a new
// bank bound to that manager, loaded from data\sound\frontend.lug, the 0x8000 group is set to volume 1 with no fade and the
// manager is enabled; without a sound manager nothing changes.
#include <cstdio>
#include <stddef.h>
static bool weq(const wchar_t* a, const wchar_t* b) { int i = 0; for (; a[i] && a[i] == b[i]; ++i) {} return a[i] == b[i]; }
class CWideString {
public:
    CWideString(const wchar_t* text) : PText(text) {}
    ~CWideString() {}
    const wchar_t* PText;
};
class CSoundManagerLH;
class CASoundBank {
public:
    CASoundBank() : loaded(0) {}
    virtual ~CASoundBank() {}
    virtual void Load(const CWideString& fileName) { loaded = weq(fileName.PText, L"data\\sound\\frontend.lug") ? 1 : 2; }
    int loaded;
};
class CSoundBankLH : public CASoundBank {
public:
    CSoundBankLH(CSoundManagerLH* manager) : m_pManager(manager) {}
    CSoundManagerLH* m_pManager;
};
template <class T> class CCountedPointer {
public:
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer() { delete Data; }
    void Reset(T* data) { delete Data; Data = data; }
    T* operator->() const { return Data; }
    T* Data; void* Info;
};
class CSoundManagerLH {
public:
    CSoundManagerLH() : mask(0), volume(0), fade(-1), enabled(false), volumeCalls(0) {}
    virtual void SetGroupVolume(unsigned long groupMask, float v, float fadeTime) { mask = groupMask; volume = v; fade = fadeTime; ++volumeCalls; }
    void SetEnabled(bool e) { enabled = e; }
    unsigned long mask; float volume; float fade; bool enabled; int volumeCalls;
};
CSoundManagerLH* g_pSoundSystem = 0;
class CNewFrontendGameComponent {
public:
    CCountedPointer<CASoundBank> PSampleBank;
    void InitialiseSounds();
};
void CNewFrontendGameComponent::InitialiseSounds()
{
    if (g_pSoundSystem) {
        PSampleBank.Reset(new CSoundBankLH(g_pSoundSystem));
        PSampleBank->Load(L"data\\sound\\frontend.lug");
        g_pSoundSystem->SetGroupVolume(0x8000, 1.0f, 0.0f);
        g_pSoundSystem->SetEnabled(true);
    }
}
int main()
{
    int bad = 0;
    CNewFrontendGameComponent c;
    c.InitialiseSounds();
    if (c.PSampleBank.Data != 0) bad = 1;
    CSoundManagerLH sm; g_pSoundSystem = &sm;
    c.InitialiseSounds();
    CSoundBankLH* bank = static_cast<CSoundBankLH*>(c.PSampleBank.Data);
    if (!bank || bank->m_pManager != &sm || bank->loaded != 1) bad = 1;
    if (sm.volumeCalls != 1 || sm.mask != 0x8000 || sm.volume != 1.0f || sm.fade != 0.0f || !sm.enabled) bad = 1;
    if (bad) { std::printf("0042e17c_TEST FAIL\n"); return 1; }
    std::printf("0042e17c_TEST PASS\n");
    return 0;
}