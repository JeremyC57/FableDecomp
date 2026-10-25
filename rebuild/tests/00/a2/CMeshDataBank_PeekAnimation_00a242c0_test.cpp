#include <cstdio>
// Behaviour test for 0x00A242C0 CMeshDataBank::PeekAnimation: the entry pointer at the given index of the bank's
// MeshesAndAnimations array (+0x380), unchanged. Links against the source object.
class CMeshDataBank;
class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry() { }
    CMeshDataBank* Bank;
    unsigned long  Index;
};
class C3DAnimationInfo : public CMeshDataBankEntry { };
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
class CMeshDataBank {
public:
    CMeshDataBank() { }
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;
    unsigned char _pad_0x04[0x37c];
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;
private:
    virtual void Vptr() { }
};
int main()
{
    C3DAnimationInfo e0, e1, e2;
    CMeshDataBankEntry* entries[3] = { &e0, &e1, &e2 };
    CMeshDataBank bank;
    if ((char*)&bank.MeshesAndAnimations - (char*)&bank != 0x380) { std::printf("00a242c0_TEST FAIL layout\n"); return 1; }
    bank.MeshesAndAnimations._M_start = entries; bank.MeshesAndAnimations._M_finish = entries + 3; bank.MeshesAndAnimations._M_end_of_storage = entries + 3;
    if (bank.PeekAnimation(0) != &e0 || bank.PeekAnimation(2) != &e2 || bank.PeekAnimation(1) != &e1) { std::printf("00a242c0_TEST FAIL\n"); return 1; }
    entries[1] = 0;
    if (bank.PeekAnimation(1) != 0) { std::printf("00a242c0_TEST FAIL null\n"); return 1; }
    std::printf("00a242c0_TEST PASS\n");
    return 0;
}
