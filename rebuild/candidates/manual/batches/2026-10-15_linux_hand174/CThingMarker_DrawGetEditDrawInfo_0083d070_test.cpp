#include <cstdio>
#include <cstring>
struct Colour { unsigned char B, G, R, A; void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct Info { char Fullname[64]; Colour C; };
struct Marker { bool baseOk; const char* ScriptName;
    bool Base(float, Info*) const { return baseOk; }
    bool Draw(float t, Info* info) const {
        if (Base(t, info)) {
            char tmp[64]; std::strcpy(tmp, ""); std::strcat(tmp, "Marker - "); std::strcat(tmp, ScriptName);
            std::strcpy(info->Fullname, tmp);
            info->C.Set(0xff, 0x80, 0xff, 0xff);
            return true;
        }
        return false; } };
int main()
{
    Marker m = { true, "M1" };
    Info i; i.C.B = i.C.G = i.C.R = i.C.A = 0;
    bool ok = m.Draw(0, &i) && std::strcmp(i.Fullname, "Marker - M1") == 0 && i.C.R == 0xff && i.C.G == 0x80 && i.C.B == 0xff && i.C.A == 0xff;
    m.baseOk = false; Info j; std::strcpy(j.Fullname, "x"); ok = ok && !m.Draw(0, &j) && std::strcmp(j.Fullname, "x") == 0;
    if (!ok) { std::printf("0083d070_TEST FAIL\n"); return 1; }
    std::printf("0083d070_TEST PASS\n");
    return 0;
}
