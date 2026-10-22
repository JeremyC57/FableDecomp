struct Vec3 { float x, y, z; };
extern int __fastcall CTCPhysicsNavigator_Notify(void* navigator, int edxpass, const Vec3* v);
void __fastcall CTCPhysicsNavigator_SetPosition(void* navigator, int edxpass, const Vec3* v)
{
    CTCPhysicsNavigator_Notify(navigator, edxpass, v);
    *(Vec3*)((char*)navigator + 0x89) = *v;
}