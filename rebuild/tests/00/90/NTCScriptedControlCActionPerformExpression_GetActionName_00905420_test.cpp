// Behaviour test for 0x00905420 NTCScriptedControl::CActionPerformExpression::GetActionName() const (agentFAM05 respell; the
// catalog test linked the old symbol FableGetActionName_00905420). VC7.1: links the row object, whose body builds the result in
// the caller's return slot with CCharString(const char*, long) (defined here to record its arguments). g++: the same body as a
// portable twin (no row object on a 64-bit host).
#include <cstdio>
#include <cstring>
class CCharString {
public:
    CCharString(const char* text, long length = -1);
    const char* Text;
    long Length;
};
CCharString::CCharString(const char* text, long length) : Text(text), Length(length) {}
namespace NTCScriptedControl {
class CActionPerformExpression {
public:
    virtual CCharString GetActionName() const;
};
#ifndef _MSC_VER
CCharString CActionPerformExpression::GetActionName() const { return CCharString("CActionPerformExpression"); }
#endif
}
int main()
{
    NTCScriptedControl::CActionPerformExpression action;
    CCharString name = action.CActionPerformExpression::GetActionName();
    if (name.Length != -1 || !name.Text || std::strcmp(name.Text, "CActionPerformExpression") != 0) {
        std::printf("00905420_TEST FAIL\n");
        return 1;
    }
    std::printf("00905420_TEST PASS\n");
    return 0;
}
