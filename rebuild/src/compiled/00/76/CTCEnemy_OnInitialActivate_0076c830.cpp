// CTCEnemy::OnInitialActivate @ 0076c830
class CTCEnemy;

// g_global singleton at 0x13b8790; its method is a real __fastcall member
struct GlobalThing {
    void Register(int arg);   // __fastcall, stack arg -> call 0x4378c0
};
extern GlobalThing* g_electroGlobal;

// base OnInitialActivate (tail jmp 0x686870), __fastcall(this)
void __fastcall CTCEnemy_base_OnInitialActivate(CTCEnemy* enemy);

void __fastcall OnInitialActivate(CTCEnemy* enemy)
{
    g_electroGlobal->Register(0);
    CTCEnemy_base_OnInitialActivate(enemy);
}