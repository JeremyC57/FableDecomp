#include <cstdio>
enum EInputType {};
enum EInputKey {};
class C2DVector { public: float X; float Y; };
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual EInputType GetInputType() const = 0;
    virtual bool operator==(const CInputTypeBase& other) const = 0;
};
class CInputTypeKeyboardKeyEvent : public CInputTypeBase {
public:
    EInputKey KeyboardKey;                    // +0x04
    C2DVector ControlDirection;                          // +0x08
    virtual EInputType GetInputType() const;
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeKeyboardKeyEvent::operator==(const CInputTypeBase& other) const
{
    if (GetInputType() == other.GetInputType())
        return KeyboardKey == static_cast<const CInputTypeKeyboardKeyEvent&>(other).KeyboardKey;
    return false;
}

CInputTypeBase::~CInputTypeBase() {}
EInputType CInputTypeKeyboardKeyEvent::GetInputType() const { return (EInputType)7; }
struct COther : CInputTypeBase {
    long Pad;
    EInputType GetInputType() const { return (EInputType)8; }
    bool operator==(const CInputTypeBase&) const { return false; }
};
int main()
{
    CInputTypeKeyboardKeyEvent a, b; COther o; o.Pad = 5;
    a.KeyboardKey = (EInputKey)5; b.KeyboardKey = (EInputKey)5;
    bool ok = (a == b) && (b == a);
    b.KeyboardKey = (EInputKey)6;
    ok = ok && !(a == b);
    ok = ok && !(a == o);
    std::printf(ok ? "0066ede0_TEST PASS\n" : "FAIL\n");
    return 0;
}