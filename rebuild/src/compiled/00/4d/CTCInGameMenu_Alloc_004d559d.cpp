#pragma optimize("s",on)

class CTCInGameMenu { public:
    CTCInGameMenu* Ctor(void* parent);   // this-call member; returns this
};
CTCInGameMenu* __fastcall CTCInGameMenu_Alloc(void* parent /*ecx*/)
{
    CTCInGameMenu* p = (CTCInGameMenu*)::operator new(0xF8);
    if (p != 0)
        return p->Ctor(parent);
    return 0;
}