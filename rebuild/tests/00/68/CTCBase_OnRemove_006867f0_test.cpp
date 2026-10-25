// Behaviour test for CTCBase::OnRemove (0x006867F0): links the reconstructed object; the call (direct and virtual) must return
// with the object untouched.
#include <cstdio>
#include <cstring>
class CTCBase {
public:
    CTCBase() : Marker(0x1234) {}
    virtual ~CTCBase();
    virtual void OnRemove();
    virtual void OnKill();
    long Marker;
};
CTCBase::~CTCBase() {}
void CTCBase::OnKill() {}
int main()
{
    CTCBase b;
    unsigned char before[sizeof(CTCBase)]; std::memcpy(before, &b, sizeof b);
    b.CTCBase::OnRemove();
    CTCBase* p = &b; p->OnRemove();
    if (std::memcmp(before, &b, sizeof b) != 0 || b.Marker != 0x1234) { std::printf("006867f0_TEST FAIL\n"); return 1; }
    std::printf("006867f0_TEST PASS\n");
    return 0;
}
