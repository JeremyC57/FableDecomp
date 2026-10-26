struct Target;
struct TargetVtbl {
    char pad[0x98];
    bool (__fastcall *slot)(Target*);
};
struct Target {
    TargetVtbl* vtbl;
};
class CScriptThing { public:
    void* pad0;
    Target* m_target;
};

bool __fastcall CScriptThing_MsgPerformedAnySpecialAbility(CScriptThing* scriptThing)
{
    Target* t = scriptThing->m_target;
    if (t == 0)
        return false;
    return t->vtbl->slot(t);
}