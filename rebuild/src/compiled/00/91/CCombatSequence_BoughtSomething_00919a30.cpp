struct S;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
struct S { char pad[0x14]; int result; };
void __fastcall BoughtSomething(S* self)
{
    self->result = CWorld::GetFrame();
}