struct S { char pad[0xD8]; unsigned char f; };
unsigned char __fastcall PopDontPopulateNextLoadedRegion(S* world)
{
    unsigned char v = world->f;
    world->f = 0;
    return v;
}