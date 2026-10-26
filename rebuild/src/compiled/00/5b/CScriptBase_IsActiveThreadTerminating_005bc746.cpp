
struct Thread { char pad[0x18]; char terminating; };
class CScriptBase { public: char pad[0x8]; Thread* activeThread; };

char __fastcall IsActiveThreadTerminating(CScriptBase* self)
{
    Thread* t = self->activeThread;
    if (t)
        return t->terminating;
    return 1;
}