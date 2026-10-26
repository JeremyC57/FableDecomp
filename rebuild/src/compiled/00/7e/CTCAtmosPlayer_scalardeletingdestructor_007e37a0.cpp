class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
struct CTCAtmosPlayer {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCAtmosPlayer_scalar_deleting_destructor(CTCAtmosPlayer* player, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(player))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(player);
    return player;
}