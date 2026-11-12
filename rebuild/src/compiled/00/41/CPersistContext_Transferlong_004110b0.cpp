// CPersistContext::Transfer<long>  0x004110b0
class CPersistContext { public:
    // Callee: CPersistContext::Transfer<long>(const char*, long&, const long&) @ 0x00410BE0
    // (agentNF19: Ego_r/FableWin ??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z)
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    void Transfer_long_(char* pName, long* pData);
};

void CPersistContext::Transfer_long_(char* pName, long* pData)
{
    long dflt = 0;
    Transfer<long>(pName, *pData, dflt);
}