// Behaviour test for 0x007B47B0 CPersistTraits<CCrimeWaitingForBribe>::TransferIn: "(" handle "," type ")" fills both
// fields; a missing "(" leaves the record untouched, a missing "," stops after the handle.
#include <cstdio>
#include <cstring>
struct Parser { const char* Tokens[8]; int Pos; const char* Next() { return Tokens[Pos++]; } };
long ReadLong(Parser& p) { long v = 0; const char* t = p.Next(); while (*t) v = v * 10 + (*t++ - '0'); return v; }
struct Bribe { long DeedHandle; int Type; };
void TransferIn(Parser& parser, Bribe& value)
{
    if (std::strcmp(parser.Next(), "(") == 0) {
        value.DeedHandle = ReadLong(parser);
        if (std::strcmp(parser.Next(), ",") == 0) {
            value.Type = (int)ReadLong(parser);
            parser.Next();
        }
    }
}
int main()
{
    Parser a = { { "(", "42", ",", "7", ")" }, 0 };
    Bribe b = { -1, -1 };
    TransferIn(a, b);
    bool ok = b.DeedHandle == 42 && b.Type == 7 && a.Pos == 5;
    Parser c = { { "[", "1" }, 0 };
    Bribe d = { -1, -1 };
    TransferIn(c, d);
    ok = ok && d.DeedHandle == -1 && d.Type == -1 && c.Pos == 1;
    Parser e = { { "(", "5", ";", "9" }, 0 };
    Bribe f = { -1, -1 };
    TransferIn(e, f);
    ok = ok && f.DeedHandle == 5 && f.Type == -1 && e.Pos == 3;
    if (!ok) { std::printf("007b47b0_TEST FAIL\n"); return 1; }
    std::printf("007b47b0_TEST PASS\n");
    return 0;
}