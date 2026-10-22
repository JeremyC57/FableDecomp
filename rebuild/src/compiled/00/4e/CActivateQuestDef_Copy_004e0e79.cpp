#pragma optimize("s",on)

struct CDefClassBase {
    char pad[0x28];
    void Copy(CDefClassBase const* src);  // thiscall: ecx=this, stack=src
};

struct CActivateQuestDef : CDefClassBase {
    int  field28;      // +0x28
    char LoadResources;      // +0x2c
};

void __fastcall Copy(CActivateQuestDef* self, void* edx, CDefClassBase const* src)
{
    self->CDefClassBase::Copy(src);
    CActivateQuestDef const* s = (CActivateQuestDef const*)src;
    self->field28 = s->field28;
    self->LoadResources = s->LoadResources;
}