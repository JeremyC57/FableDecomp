#pragma optimize("s",on)
struct CHeroPostcardGeneratorDef {
    int f0;
    int Parent;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
};

extern void __fastcall CHeroPostcardGeneratorDef_dtor(CHeroPostcardGeneratorDef* def);
extern void __cdecl eng_operator_delete(void* p);

void* __fastcall CHeroPostcardGeneratorDef_scalar_deleting_destructor(CHeroPostcardGeneratorDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CHeroPostcardGeneratorDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}