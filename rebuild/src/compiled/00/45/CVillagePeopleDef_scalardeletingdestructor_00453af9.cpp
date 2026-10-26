#pragma optimize("s",on)
class CVillagePeopleDef { public: int f0; };
extern void __fastcall CVillagePeopleDef_dtor(CVillagePeopleDef* def);
extern void __cdecl op_delete(void* p);

struct CVillagePeopleDef_shim {
    void* CVillagePeopleDef_vecdel(unsigned int flags);
};

void* CVillagePeopleDef_shim::CVillagePeopleDef_vecdel(unsigned int flags)
{
    CVillagePeopleDef_dtor((CVillagePeopleDef*)this);
    if (flags & 1)
        op_delete(this);
    return this;
}