struct CPlayerGui {
    int   f0;
    int   World;
    float f8;
    char  fc;
    char  fd;
};

CPlayerGui* __fastcall ForceOpen(CPlayerGui* self)
{
    self->fd = 0;
    self->fc = 0;
    self->World = 0;
    self->f0 = 0;
    self->f8 = 1.0f;
    return self;
}