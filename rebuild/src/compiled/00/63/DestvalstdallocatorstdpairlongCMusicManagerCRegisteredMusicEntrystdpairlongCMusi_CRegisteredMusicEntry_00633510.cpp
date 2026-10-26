// Retail 0x006334f0
// void __fastcall _Dest_val(Owner* this, allocator*, pair*)
// push esi; mov esi,ecx; mov ecx,[esi+0x140]; call 0xa0d2d0;
// mov ecx,[esi+0x1a4]; call 0xa0d2d0; pop esi; ret 8

class CAInputProcess { public:
    int pad;
public:
    void RemoveFromInterface();   // 0x00A0D2D0
};

struct Owner {
    char pad[0x140];
    CAInputProcess* m_a;        // +0x140
    char pad2[0x1a4 - 0x140 - 4];
    CAInputProcess* m_b;        // +0x1a4
    void _Dest_val_helper(void* al, void* pr);
};

void Owner::_Dest_val_helper(void* al, void* pr)
{
    m_a->RemoveFromInterface();
    m_b->RemoveFromInterface();
}