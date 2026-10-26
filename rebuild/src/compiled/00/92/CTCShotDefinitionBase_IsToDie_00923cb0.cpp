// Byte-exact reconstruction of CTCShotDefinitionBase::IsToDie @ 0x00923cb0
struct CTCShotDefinitionBase {
    char pad[0x34];
    int m_dieTime; // +0x34
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870

int __fastcall IsToDie(CTCShotDefinitionBase* self)
{
    int t = self->m_dieTime;
    if (t != 0) {
        if (CWorld::GetFrame() > self->m_dieTime)
            return 1;
    }
    return 0;
}