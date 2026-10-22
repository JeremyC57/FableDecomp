// CTCFish::GetClassTypeName @ 0x007dfc60
// Returns a CCharString constructed from a fixed string literal.

class CCharString {
public:
    CCharString(const char* s, int n);
    void* p;
};

// The ctor lives at 0x0099ebf0 in retail; declare it so the compiler emits a
// direct call. Address of the string literal is 0x0123d7b0.
class CTCFish {
public:
    CCharString GetClassTypeName();
};

CCharString CTCFish::GetClassTypeName()
{
    return CCharString((const char*)0x0123d7b0, -1);
}