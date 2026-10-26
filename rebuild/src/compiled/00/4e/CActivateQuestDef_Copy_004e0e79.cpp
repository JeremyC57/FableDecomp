#pragma optimize("s",on)

class CDefClassBase { public:
    char pad[0x28];
    CDefClassBase& operator=(const CDefClassBase& other);  // thiscall: ecx=this, stack=src
};

class CActivateQuestDef : public CDefClassBase { public:
    int  field28;      // +0x28
    char LoadResources;      // +0x2c
};

void __fastcall Copy(CActivateQuestDef* self, void* edx, CDefClassBase const* src)
{
    self->CDefClassBase::operator=(*(const CDefClassBase*)(src));
    CActivateQuestDef const* s = (CActivateQuestDef const*)src;
    self->field28 = s->field28;
    self->LoadResources = s->LoadResources;
}