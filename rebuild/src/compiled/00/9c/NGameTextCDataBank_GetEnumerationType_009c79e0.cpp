// NGameText::CDataBank::GetEnumerationType(unsigned long type) const -- name of a bank enumeration type: 0 "EGameText", 1 "ETextGroup", else ""
// Leaf: ?GetEnumerationType@CDataBank@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C79E0, 82 bytes (+14 int3 padding), `ret 8` (hidden CCharString return slot + type).
// Name: FableWin.pdb ?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z (protected virtual, catalog vfunc_19).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (three times, one per arm).
// Literals (.rdata): 0x0129B464 "EGameText", 0x0129B458 "ETextGroup", 0x0122D70E "" (shared empty string).
// Shape: a two-test if-chain, each arm returning a fresh CCharString into the hidden slot; the common `push -1` length
// argument is hoisted above the first branch by the optimiser.
class CCharStringData;
class CCharString {
public:
    CCharString(const char* s, long n);                  // 0x0099EBF0
private:
    CCharStringData* PStringData;
};

namespace NGameText {
class CDataBank {
protected:
    virtual CCharString GetEnumerationType(unsigned long type) const;
};

CCharString CDataBank::GetEnumerationType(unsigned long type) const
{
    if (type == 0)
        return CCharString("EGameText", -1);
    if (type == 1)
        return CCharString("ETextGroup", -1);
    return CCharString("", -1);
}
} // namespace NGameText