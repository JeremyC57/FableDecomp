#include <cstdio>

class CPlayerGui
{
public:
    unsigned char m_Pad00[0x2BB];
    unsigned char m_Before;
    bool m_AreScreenMessagesEnabled;
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
    virtual void SetScreenMessagesEnabled(bool enabled) const;
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
        call CGameScriptInterface::SetScreenMessagesEnabled
    }
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_008922d0_TEST FAIL: %s\n", pMessage);
    return 1;
}

int main()
{
    CGameScriptInterface interfaceObject;
    CPlayerGui first = {};
    CPlayerGui second = {};

    first.m_Before = 0x31;
    first.m_AreScreenMessagesEnabled = true;
    first.m_After = 0x13;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&first);
    interfaceObject.CGameScriptInterface::
        SetScreenMessagesEnabled(false);
    if (first.m_AreScreenMessagesEnabled ||
        first.m_Before != 0x31 || first.m_After != 0x13)
        return Fail("false store");

    second.m_Before = 0xC7;
    second.m_AreScreenMessagesEnabled = false;
    second.m_After = 0x7C;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&second);
    interfaceObject.CGameScriptInterface::
        SetScreenMessagesEnabled(true);
    if (!second.m_AreScreenMessagesEnabled ||
        second.m_Before != 0xC7 || second.m_After != 0x7C)
        return Fail("true store or manager switch");

    CallWithRawBool(&interfaceObject, 0x96);
    if (*reinterpret_cast<unsigned char*>(
            &second.m_AreScreenMessagesEnabled) != 0x96)
        return Fail("raw ABI byte");
    if (second.m_Before != 0xC7 || second.m_After != 0x7C)
        return Fail("adjacent bytes");

    std::printf("FSE2_008922d0_TEST PASS\n");
    return 0;
}
