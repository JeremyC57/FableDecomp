// CTCMapwho::OnDie  retail 0x006b5870  __fastcall void(CTCMapwho*)

struct SubObjAt90 {
    void Release();  // thiscall at [esi+0x90], call 0x8814c0
};

// cdecl free function, takes the object pointer, cleans up 4 bytes
void operator delete(void*);   // call 0xbfe9bc

class CTCMapwho { public:
    char pad[0x90];
    SubObjAt90 sub;   // at +0x90
    void OnDie();
};

void CTCMapwho::OnDie()
{
    if (this) {
        this->sub.Release();
        operator delete(this);
    }
}