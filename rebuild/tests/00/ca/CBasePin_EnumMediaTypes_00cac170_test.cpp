// Behaviour test for 0x00CAC170 (CBasePin::EnumMediaTypes): the strmbase body and the inlined CEnumMediaTypes
// constructor over a fake pin. Checks: E_POINTER for null; a new enumerator at position 0 with ref count 1 holding
// the pin (AddRef'd once) and the pin's media-type version; a clone constructor copies position and version.
#include <cstdio>
typedef int HRESULT;
class CBasePin;
struct IEnumMediaTypes { virtual ~IEnumMediaTypes() { } };
class CEnumMediaTypes : public IEnumMediaTypes {
public:
    int m_Position;
    CBasePin* m_pPin;
    int m_Version;
    int m_cRef;
    CEnumMediaTypes(CBasePin* pPin, CEnumMediaTypes* pEnumMediaTypes);
};
class CBasePin {
public:
    int m_cRef;
    int m_TypeVersion;
    CBasePin() : m_cRef(0), m_TypeVersion(0) { }
    int AddRef() { return ++m_cRef; }
    int GetMediaTypeVersion() { return m_TypeVersion; }
    HRESULT EnumMediaTypes(IEnumMediaTypes** ppEnum);
};
CEnumMediaTypes::CEnumMediaTypes(CBasePin* pPin, CEnumMediaTypes* pEnumMediaTypes)
    : m_Position(0), m_pPin(pPin), m_cRef(1)
{
    m_pPin->AddRef();
    if (pEnumMediaTypes == 0) {
        m_Version = m_pPin->GetMediaTypeVersion();
        return;
    }
    m_Position = pEnumMediaTypes->m_Position;
    m_Version = pEnumMediaTypes->m_Version;
}
HRESULT CBasePin::EnumMediaTypes(IEnumMediaTypes** ppEnum)
{
    if (ppEnum == 0) {
        return (HRESULT)0x80004003;
    }
    *ppEnum = new CEnumMediaTypes(this, 0);
    if (*ppEnum == 0) {
        return (HRESULT)0x8007000E;
    }
    return 0;
}
static int fail(int line) { std::printf("00cac170_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    CBasePin pin;
    pin.m_TypeVersion = 5;
    CHECK(pin.EnumMediaTypes(0) == (HRESULT)0x80004003 && pin.m_cRef == 0);
    IEnumMediaTypes* e = 0;
    CHECK(pin.EnumMediaTypes(&e) == 0 && e != 0);
    CEnumMediaTypes* m = static_cast<CEnumMediaTypes*>(e);
    CHECK(m->m_Position == 0 && m->m_pPin == &pin && m->m_Version == 5 && m->m_cRef == 1 && pin.m_cRef == 1);
    m->m_Position = 3; pin.m_TypeVersion = 9;
    CEnumMediaTypes clone(&pin, m);
    CHECK(clone.m_Position == 3 && clone.m_Version == 5 && clone.m_cRef == 1 && pin.m_cRef == 2);
    delete e;
    std::printf("00cac170_TEST PASS\n");
    return 0;
}