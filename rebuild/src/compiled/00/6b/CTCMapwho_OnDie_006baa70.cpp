#pragma optimize("t",on)
// Leaf: OnDie

class CTCMapwho { public: int dummy; };

class CFaction { public:
    ~CFaction();   // 0x006BA6D0 (Ego_r.pdb ??1CFaction@@QAE@XZ)
};
void operator delete(void*);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        ((CFaction*)self)->~CFaction();
        operator delete(self);
    }
}