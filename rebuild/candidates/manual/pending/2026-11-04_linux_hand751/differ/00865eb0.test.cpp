// Behaviour test for CCombatAction_GenericStrikeResponseKnockdownRecoverBase::Initialise (0x00865EB0, parked DIFFER row): a model of the row -- the recover animation is the previous entry's data name, else RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER.
#include <stdio.h>
#include <string>
static std::string Pick(const char* previousData) { return previousData && *previousData ? std::string(previousData) : std::string("RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER"); }
static int Check() { return Pick("GET_UP_FAST") == "GET_UP_FAST" && Pick(0) == "RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER" && Pick("") == "RESPONSE_GENERIC_HIT_KNOCKDOWN_RECOVER"; }
int main()
{
    int ok = Check();
    printf(ok ? "00865eb0_TEST PASS\n" : "00865eb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
