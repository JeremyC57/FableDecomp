struct CScriptModuleSelfTerminateBase {
    char pad[0x50];
    int m_threshold; /* +0x50 */
};

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

bool __fastcall IsFinished(CScriptModuleSelfTerminateBase* self)
{
    return CWorld::GetFrame() > self->m_threshold;
}