// behaviour test for 0x0066F360
#include <stdio.h>
class CInputTypeBase {
public:
    virtual ~CInputTypeBase() {}
    virtual long GetType() const = 0;                    // slot 1 (+4)
    virtual bool operator==(const CInputTypeBase& other) const = 0;   // slot 2 (+8)
};
class CInputTypeXboxPadRightStickEvent : public CInputTypeBase {
public:
    long Id;
    virtual long GetType() const { return Id; }
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeXboxPadRightStickEvent::operator==(const CInputTypeBase& other) const
{
    return GetType() == other.GetType();
}

int main()
{
    CInputTypeXboxPadRightStickEvent a, b, c; a.Id = 3; b.Id = 3; c.Id = 4;
    const CInputTypeBase& ra = a;
    int ok = (ra == b) && !(ra == c) && (c == c);
    printf(ok ? "0066f360_TEST PASS\n" : "0066f360_TEST FAIL\n");
    return ok ? 0 : 1;
}