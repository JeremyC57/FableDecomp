#include <cstdio>

class CPlayerGui
{
public:
    unsigned char m_Pad00[0xF8];
    bool m_IsSkippable;
    unsigned char m_After;
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; T& operator*() const { return *Data; } };
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the source reads the private static PClass through Get()
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void __fastcall Set(T* p) { PClass.Data = p; PClass.Info = 0; }   // ?Set@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIXPAVCPlayerGui@@@Z
};
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CPlayerGui>;

class CGameScriptInterface
{
public:
    virtual void SetCutsceneSkippable(bool isSkippable) const;
};

static void CallWithRawBool(
    const CGameScriptInterface* pInterface,
    unsigned char value)
{
    __asm
    {
        mov ecx, pInterface
        movzx eax, value
        push eax
        call CGameScriptInterface::SetCutsceneSkippable
    }
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_00892280_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGameScriptInterface interfaceObject;
    CPlayerGui first = {};
    CPlayerGui second = {};

    first.m_IsSkippable = true;
    first.m_After = 0x5A;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&first);
    interfaceObject.CGameScriptInterface::SetCutsceneSkippable(false);
    if (first.m_IsSkippable || first.m_After != 0x5A)
        return Fail("false store");

    second.m_IsSkippable = false;
    second.m_After = 0xA5;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&second);
    interfaceObject.CGameScriptInterface::SetCutsceneSkippable(true);
    if (!second.m_IsSkippable || second.m_After != 0xA5)
        return Fail("true store or manager switch");

    CallWithRawBool(&interfaceObject, 0x7E);
    if (*reinterpret_cast<unsigned char*>(&second.m_IsSkippable) != 0x7E)
        return Fail("raw ABI byte");
    if (second.m_After != 0xA5)
        return Fail("adjacent byte");

    std::printf("FSE2_00892280_TEST PASS\n");
    return 0;
}
