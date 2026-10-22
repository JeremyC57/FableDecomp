// Copy the members at this+0xcc / this+0xd0 into the caller's 2-dword struct.
// __fastcall this=ecx, out=stack (ret 4).
#pragma pack(push,1)
struct Out { int a; int b; };
struct CIconText {
    char pad_0[0xcc];
    int first;
    int second;
    void GetSize(Out* out);
};
#pragma pack(pop)
void CIconText::GetSize(Out* out) {
    out->a = this->first;
    out->b = this->second;
}