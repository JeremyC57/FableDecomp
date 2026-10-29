struct CTCShotDefinitionBase {
    char pad[0x60];
    int TimeToDie;   // +0x60
};

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

int __fastcall IsToDie(CTCShotDefinitionBase *self)
{
    int t = self->TimeToDie;
    if (t > 0) {
        if (CWorld::GetFrame() > self->TimeToDie)
            return 1;
    }
    return 0;
}