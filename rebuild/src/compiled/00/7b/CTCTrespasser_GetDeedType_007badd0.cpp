// CTCTrespasser::GetDeedType(long) -- maps the trespass kind (0, 1, 2) to its deed type (10, 11, 2); anything else is 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BADD0, length 44 bytes (`ret 4`: a member whose `this` is unused).
// Shape: a three-case `switch` lowered to `sub eax,0 ; je / dec eax ; je / dec eax ; je` with the default
// (`xor eax,eax`) on the fall-through and one `ret 4` per case (the return values are the deed-type enum
// constants; names are descriptive).
class CTCTrespasser { public:
    long GetDeedType(long trespassType);
};

long CTCTrespasser::GetDeedType(long trespassType)
{
    switch (trespassType) {
    case 0: return 10;
    case 1: return 11;
    case 2: return 2;
    }
    return 0;
}