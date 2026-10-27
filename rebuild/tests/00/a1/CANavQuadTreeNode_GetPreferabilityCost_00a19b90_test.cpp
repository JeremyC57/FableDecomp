// Test for 0x00A19B90 CANavQuadTreeNode::GetPreferabilityCost: the base virtual reports the maximum cost 255.
#include <stdio.h>
class CANavQuadTreeNode { public: virtual unsigned char GetPreferabilityCost() const { return 255; } };
class CNavQuadTreeNode : public CANavQuadTreeNode { };
int main()
{
    CNavQuadTreeNode n; const CANavQuadTreeNode* p = &n;
    bool ok = p->GetPreferabilityCost() == 255;
    puts(ok ? "00a19b90_TEST PASS" : "00a19b90_TEST FAIL");
    return ok ? 0 : 1;
}