#pragma optimize("s",on)
class CCombatAbilityBlockingData { public:
    int f0;
    int Parent;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
    ~CCombatAbilityBlockingData();
};
extern "C" void __cdecl fable_operator_delete(void*);

void* __fastcall CCombatAbilityBlockingData_scalar_deleting_destructor(CCombatAbilityBlockingData* blocking, int edx, unsigned int flags) {
    blocking->~CCombatAbilityBlockingData();
    if (flags & 1) {
        fable_operator_delete(blocking);
    }
    return blocking;
}