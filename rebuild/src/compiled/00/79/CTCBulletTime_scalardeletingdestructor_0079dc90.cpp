class CTCBulletTime { public:
    void* vftable;
};

void __fastcall CTCBulletTime_dtor(CTCBulletTime* time);
void operator delete(void*);

void* __fastcall CTCBulletTime_scalar_deleting_destructor(CTCBulletTime* time, int edx_unused, unsigned int flags)
{
    CTCBulletTime_dtor(time);
    if (flags & 1)
        operator delete(time);
    return time;
}