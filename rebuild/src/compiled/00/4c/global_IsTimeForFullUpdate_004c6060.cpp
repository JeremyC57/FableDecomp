// CBulletTimeManager::IsTimeForFullUpdate @ 0x004c6060
// __fastcall bool(CBulletTimeManager*)

extern "C" int __fastcall sub_4c5fa0(void* self);      // this->method, result unused
extern "C" int __cdecl   sub_bfea70(void);             // returns divisor -> esi
class CWorld { public: static long __fastcall GetFrame(); };             // returns dividend -> eax

class CBulletTimeManager;

extern "C" bool __fastcall IsTimeForFullUpdate(CBulletTimeManager* self)
{
    sub_4c5fa0(self);
    int divisor = sub_bfea70();
    int dividend = CWorld::GetFrame();
    return (dividend % divisor) == 0;
}