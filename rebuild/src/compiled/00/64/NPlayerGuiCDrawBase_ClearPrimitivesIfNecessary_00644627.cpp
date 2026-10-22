#pragma optimize("s",on)
// NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary  @ 00644627  (VC7.1 /O2 /Oy)

struct Temp {
    int slot;
};

// callee at 0x43f360 : __fastcall(this=temp, edx=arg, stackarg0, stackarg1)
void __fastcall Temp_helper(Temp* draw, int edx, int a0, int a1);

struct CDrawBase {
    char pad0[0x18];
    unsigned char Drawing;   // +0x18
    int   field1c;          // +0x1c
    int   field20;          // +0x20
    void ClearPrimitivesIfNecessary();
};

void CDrawBase::ClearPrimitivesIfNecessary()
{
    Temp t;
    if (this->Drawing == 0) {
        Temp_helper(&t, this->field1c, this->field20, 0x41be70);
    }
}