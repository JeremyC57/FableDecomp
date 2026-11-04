// Behaviour test for std::pair<const CCharString, CBankFileManager::CBankInfo>::~pair (0x009AA440): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_key { ~M_key() { Log("key"); } };
struct M_dev { ~M_dev() { Log("dev"); } };
struct M_retail { ~M_retail() { Log("retail"); } };
struct M_devhdr { ~M_devhdr() { Log("devhdr"); } };
struct M_retailhdr { ~M_retailhdr() { Log("retailhdr"); } };
struct T : Base {
    M_key m_key;
    M_dev m_dev;
    M_retail m_retail;
    M_devhdr m_devhdr;
    M_retailhdr m_retailhdr;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "retailhdr,devhdr,retail,dev,key,base,") == 0;
    printf(ok ? "009aa440_TEST PASS\n" : "009aa440_TEST FAIL\n");
    return ok ? 0 : 1;
}
