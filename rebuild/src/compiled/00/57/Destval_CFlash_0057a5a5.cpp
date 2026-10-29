extern int g_glob;
// Leaf: CFlash>
class CCharString { public: void* Data; const CCharString& operator=(const CCharString& other); };   // 0x0099EFB0
struct S { char pad[0x170]; void Dtor(); };
void S::Dtor(){ *(CCharString*)((char*)this+0x170) = *(const CCharString*)&g_glob; }