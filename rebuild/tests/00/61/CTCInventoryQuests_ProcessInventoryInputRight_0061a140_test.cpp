// Behaviour test for CTCInventoryQuests::ProcessInventoryInputRight @ 0x0061A140: the method comes from the source object.
#include <cstdio>
namespace NInventory {

class CTCInventoryBase {
public:
    CTCInventoryBase() : LeftCalls(0), RightCalls(0), Count(0) {}
    virtual ~CTCInventoryBase() {}                          // slot 0
    virtual void _v01() {}
    virtual void _v02() {}
    virtual void _v03() {}
    virtual void _v04() {}
    virtual void _v05() {}
    virtual void _v06() {}
    virtual void _v07() {}
    virtual void _v08() {}
    virtual void _v09() {}
    virtual void _v10() {}
    virtual void _v11() {}
    virtual void _v12() {}
    virtual void _v13() {}
    virtual void _v14() {}
    virtual void _v15() {}
    virtual void _v16() {}
    virtual void _v17() {}
    virtual void _v18() {}
    virtual void _v19() {}
    virtual void _v20() {}
    virtual void _v21() {}
    virtual void _v22() {}
    virtual void _v23() {}
    virtual void _v24() {}
    virtual void _v25() {}
    virtual void _v26() {}
    virtual void _v27() {}
    virtual void _v28() {}
    virtual void _v29() {}
    virtual void _v30() {}
    virtual void _v31() {}
    virtual void _v32() {}
    virtual void _v33() {}
    virtual void _v34() {}
    virtual void _v35() {}
    virtual void _v36() {}
    virtual void _v37() {}
    virtual void _v38() {}
    virtual void _v39() {}
    virtual void ProcessInventoryInputLeft() { ++LeftCalls; }             // slot 40 (0x005BC638)
    virtual void ProcessInventoryInputRight() { ++RightCalls; }            // slot 41 (0x005BC649)
    int LeftCalls; int RightCalls; long Count; unsigned char _pad_0x10[0x130 - 0x10];
    long GetCategoryCount() const;                      // 0x005BCE2F
};

} // namespace NInventory

class CTCInventoryQuests : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryQuests@@, vtable 0x012585B4
public:
    virtual void ProcessInventoryInputLeft();           // slot 40 (0x0061A120)
    virtual void ProcessInventoryInputRight();          // slot 41 (0x0061A140)
    unsigned char _pad_0x130[0x157 - 0x130];
    bool          ActiveTestQuestMode;                  // +0x157 (Ego_r.pdb +0x177)
};
long NInventory::CTCInventoryBase::GetCategoryCount() const { return Count; }
void CTCInventoryQuests::ProcessInventoryInputLeft() { }
int main()
{
    CTCInventoryQuests q; q.Count = 2; q.ActiveTestQuestMode = false;
    q.ProcessInventoryInputRight();
    int bad = q.RightCalls != 1;
    q.ActiveTestQuestMode = true; q.ProcessInventoryInputRight(); bad |= q.RightCalls != 1;
    q.ActiveTestQuestMode = false; q.Count = 0; q.ProcessInventoryInputRight(); bad |= q.RightCalls != 1;
    q.Count = -1; q.ProcessInventoryInputRight(); bad |= q.RightCalls != 1;
    q.Count = 3; q.ProcessInventoryInputRight(); bad |= q.RightCalls != 2;
    bad |= (q.LeftCalls + q.RightCalls) != 2;
    if (bad) { std::printf("0061a140_TEST FAIL\n"); return 1; }
    std::printf("0061a140_TEST PASS\n");
    return 0;
}
