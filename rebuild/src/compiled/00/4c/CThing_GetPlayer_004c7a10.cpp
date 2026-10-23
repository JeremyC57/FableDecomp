struct CPlayer;
struct CThing { char pad[0x90]; signed char idx; };
struct CInner { CPlayer* Get_00449910(int idx); };
struct COuter { char pad[0x30]; CInner* inner; };
extern COuter* g_outer;

CPlayer* __fastcall CThing_GetPlayer(CThing* self)
{
    CInner* p = g_outer->inner;
    return p->Get_00449910(self->idx);
}