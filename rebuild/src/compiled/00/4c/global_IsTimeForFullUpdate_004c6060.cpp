// CBulletTimeManager::IsTimeForFullUpdate @ 0x004c6060
// __fastcall bool(CBulletTimeManager*)

extern "C" float __fastcall sub_4c5fa0(void* self);    // this->method returning a float (st0)
class CWorld { public: static long __fastcall GetFrame(); };             // returns dividend -> eax

class CBulletTimeManager;

extern "C" bool __fastcall IsTimeForFullUpdate(CBulletTimeManager* self)
{
    int divisor = (int)sub_4c5fa0(self);                  // (int) float: call __ftol2 0x00BFEA70
    int dividend = CWorld::GetFrame();
    return (dividend % divisor) == 0;
}