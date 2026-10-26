// CTCMapwho::OnDie @ 0x00c0ead0
// Static/free style: pointer param on stack, ret 4 (stdcall).
class CTCMapwho { public:
    void Cleanup();  // thiscall method @ 0xc1e260
};

// cdecl free function @ 0xbfe9bc

void __stdcall CTCMapwho_OnDie(CTCMapwho* p)
{
    if (p) {
        p->Cleanup();
        operator delete(p);
    }
}