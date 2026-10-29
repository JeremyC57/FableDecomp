class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
struct CAIStateGroup_BuyFromSeller {
    char pad[0x8c];
    int result;
    void BoughtSomething();
};

void CAIStateGroup_BuyFromSeller::BoughtSomething() {
    this->result = CWorld::GetFrame();
}