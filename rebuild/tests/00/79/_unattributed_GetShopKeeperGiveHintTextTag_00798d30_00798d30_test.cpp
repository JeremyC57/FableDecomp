#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct _unattributed { CCharString GetShopKeeperGiveHintTextTag_00798d30(); };
CCharString _unattributed::GetShopKeeperGiveHintTextTag_00798d30() { return CCharString("TEXT_AI_SHOP_KEEPER_GIVE_HINT", -1); }
int main() { _unattributed o; bool ok = std::strcmp(o.GetShopKeeperGiveHintTextTag_00798d30().p, "TEXT_AI_SHOP_KEEPER_GIVE_HINT") == 0; std::printf(ok ? "00798d30_TEST PASS\n" : "00798d30_TEST FAIL\n"); return ok ? 0 : 1; }
