#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
struct Member { int pad[16]; };
class COpinionReactionMaskDef { public:
    char pad[0x3c];
    Member m;
};

extern "C" void __fastcall Member_dtor(Member* def);

void* __fastcall COpinionReactionMaskDef_scalar_deleting_destructor(COpinionReactionMaskDef* def, int, unsigned int flags)
{
    Member_dtor(&def->m);
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}