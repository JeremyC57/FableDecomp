#include <cstdio>
// Behaviour test for 0x00A24290 CMeshDataBank::PeekRepeatedMesh: the entry pointer at the given index of the bank's
// MeshesAndAnimations array (+0x380), unchanged. Links against the source object.
class CMeshDataBank;
class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry() { }
    CMeshDataBank* Bank;
    unsigned long  Index;
};
class C3DMeshInfo : public CMeshDataBankEntry { };
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
class CMeshDataBank {
public:
    CMeshDataBank() { }
    const C3DMeshInfo* PeekRepeatedMesh(unsigned long index) const;
    unsigned char _pad_0x04[0x37c];
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;
private:
    virtual void Vptr() { }
};
int main()
{
    C3DMeshInfo e0, e1, e2;
    CMeshDataBankEntry* entries[3] = { &e0, &e1, &e2 };
    CMeshDataBank bank;
    if ((char*)&bank.MeshesAndAnimations - (char*)&bank != 0x380) { std::printf("00a24290_TEST FAIL layout\n"); return 1; }
    bank.MeshesAndAnimations._M_start = entries; bank.MeshesAndAnimations._M_finish = entries + 3; bank.MeshesAndAnimations._M_end_of_storage = entries + 3;
    if (bank.PeekRepeatedMesh(0) != &e0 || bank.PeekRepeatedMesh(2) != &e2 || bank.PeekRepeatedMesh(1) != &e1) { std::printf("00a24290_TEST FAIL\n"); return 1; }
    entries[1] = 0;
    if (bank.PeekRepeatedMesh(1) != 0) { std::printf("00a24290_TEST FAIL null\n"); return 1; }
    std::printf("00a24290_TEST PASS\n");
    return 0;
}
