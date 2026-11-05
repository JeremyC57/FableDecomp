// Leaf: ?CPlayerGui_SetCrimeAsDisplaying@@YIXPAVCPlayerGui@@@Z
struct Crime { char pad24[0x24]; int m24; int m28; };
class CPlayerGui { public: char pad[0x2cc]; Crime* pCrime; };
void __fastcall CPlayerGui_SetCrimeAsDisplaying(CPlayerGui* this_)
{
    this_->pCrime->m24 = 0;
    if (this_->pCrime->m28 != 0)
        this_->pCrime->m28 = 2;
}