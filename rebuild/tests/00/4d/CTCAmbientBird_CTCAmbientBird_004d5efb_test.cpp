#include <cstdio>
#include <cstring>

class CThing { public: int id; };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing& o) : ParentThing(&o) { } };
struct CTCAmbientBird : CTCBase { bool Flying; float InitialHeight; CTCAmbientBird(CThing& o) : CTCBase(o), Flying(false), InitialHeight(0.0f) { } };
int main()
{
    CThing t; t.id = 2;
    CTCAmbientBird b(t);
    if (b.ParentThing != &t || b.Flying || b.InitialHeight != 0.0f) { std::printf("004d5efb_TEST FAIL\n"); return 1; }
    std::printf("004d5efb_TEST PASS\n");
    return 0;
}