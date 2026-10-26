class CTCMapwho;
// Leaf: OnDie
extern void __fastcall CTCMapwho_helper(CTCMapwho* self);
void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_helper(self);
        operator delete(self);
    }
}