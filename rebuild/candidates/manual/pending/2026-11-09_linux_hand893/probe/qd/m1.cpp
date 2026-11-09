typedef struct HKEY__* HKEY;
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(HKEY key, const char* name, unsigned long* reserved,
    unsigned long* type, unsigned char* data, unsigned long* size);
struct CRegistryKey {
    virtual void Dummy();
    HKEY m_hKey;
    int Q01(const char* name, unsigned long* value);
    int Q02(const char* name, unsigned long* value);
    int Q03(const char* name, unsigned long* value);
    int Q04(const char* name, unsigned long* value);
    int Q05(const char* name, unsigned long* value);
    int Q06(const char* name, unsigned long* value);
    int Q07(const char* name, unsigned long* value);
    int Q08(const char* name, unsigned long* value);
    int Q09(const char* name, unsigned long* value);
    int Q10(const char* name, unsigned long* value);
    int Q11(const char* name, unsigned long* value);
    int Q12(const char* name, unsigned long* value);
    bool Q13(const char* name, unsigned long* value);
    int Q14(const char* name, unsigned long* value);
};
// baseline: two early returns
int CRegistryKey::Q01(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0) return 0;
    if (type != 4) return 0;
    return size == 4;
}
// nested positive
int CRegistryKey::Q02(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) == 0) {
        if (type == 4) return size == 4;
    }
    return 0;
}
// first early return, then positive type with fallthrough return 0
int CRegistryKey::Q03(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0) return 0;
    if (type == 4) return size == 4;
    return 0;
}
// result local
int CRegistryKey::Q04(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    long result = RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size);
    if (result != 0) return 0;
    if (type != 4) return 0;
    return size == 4;
}
// else-if chain
int CRegistryKey::Q05(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0) return 0;
    else if (type != 4) return 0;
    else return size == 4;
}
// ternary
int CRegistryKey::Q06(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0) return 0;
    return type == 4 ? size == 4 : 0;
}
// positive first test, negative second
int CRegistryKey::Q07(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) == 0) {
        if (type != 4) return 0;
        return size == 4;
    }
    return 0;
}
// do-while(0) break
int CRegistryKey::Q08(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    do {
        if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0) return 0;
        if (type != 4) break;
        return size == 4;
    } while (0);
    return 0;
}
// break first, return second
int CRegistryKey::Q09(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    do {
        if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0) break;
        if (type != 4) return 0;
        return size == 4;
    } while (0);
    return 0;
}
// switch on result
int CRegistryKey::Q10(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    switch (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size)) {
    case 0:
        if (type != 4) return 0;
        return size == 4;
    }
    return 0;
}
// bool ok
int CRegistryKey::Q11(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    bool ok = RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) == 0;
    if (!ok) return 0;
    if (type != 4) return 0;
    return size == 4;
}
// || early
int CRegistryKey::Q12(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0 || type != 4) return 0;
    return size == 4;
}
bool CRegistryKey::Q13(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size) != 0) return false;
    if (type != 4) return false;
    return size == 4;
}
int CRegistryKey::Q14(const char* name, unsigned long* value)
{
    unsigned long type; unsigned long size = 4;
    if (RegQueryValueExA(m_hKey, name, 0, &type, (unsigned char*)value, &size)) return 0;
    if (type == 4) { if (size == 4) return 1; return 0; }
    return 0;
}
