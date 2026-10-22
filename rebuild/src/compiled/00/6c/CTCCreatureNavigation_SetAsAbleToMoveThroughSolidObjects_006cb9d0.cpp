struct CObj;
struct CObjVtbl {
    void (__fastcall *slots[32])(CObj*);
};
struct CObj {
    CObjVtbl* vtbl;
};
struct CTCCreatureNavigation {
    char pad0[0x0c];
    CObj* obj;
    char pad1[0x55 - 0x10];
    unsigned char flag;
};

void __fastcall CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects(CTCCreatureNavigation* navigation, void* edx, unsigned char b)
{
    if (b != navigation->flag) {
        CObj* o = navigation->obj;
        o->vtbl->slots[0x48/4](o);
    }
    navigation->flag = b;
}