// CTCRegionFollower::GetClassTypeName @ 006af060
// __fastcall, returns CCharString BY VALUE (hidden return pointer).
// Retail: push esi; esi=[esp+8]=hidden ret buf; push -1; push 0x123c110;
//         mov ecx,esi; call CCharString(const char*,long); mov eax,esi; pop esi; ret 4.

class CCharString { public:
    char* p;
    // this(ecx)=self, stack args (const char*, int)
    CCharString(const char* s, long len);
};

class CTCRegionFollower {
    int dummy;
public:
    CCharString GetClassTypeName();
};

CCharString CTCRegionFollower::GetClassTypeName()
{
    return CCharString((const char*)0x123c110, -1);
}