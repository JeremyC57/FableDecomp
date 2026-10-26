// Behaviour test for GetNumberFromDataMarker (0x00CD5090): every element whose VariableName equals the marker
// (same string block, or equal length and equal text) stores its value into out; the last match wins and no match
// leaves out unchanged.
#include <cstdio>
#include <cstring>

class CCharStringData { public:
    const char*   m_pData;
    unsigned long m_length;
};
long Compare_00411570(const char* a, const char* b) { return std::strcmp(a, b); }
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare_00411570(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
class CDataMarkerOutput {
public:
    class CElement {
    public:
        CCharString   VariableName;
        CCharString   DataAsString;
        unsigned long DataAsULONG;
    };
    CElement* begin;
    CElement* end;
    unsigned int size() const { return (unsigned int)(end - begin); }
    const CElement& operator[](unsigned int n) const { return *(begin + n); }
};

void GetNumberFromDataMarker(const CCharString& marker, unsigned long& out, const CDataMarkerOutput& data)
{
    for (unsigned int i = 0; i < data.size(); ++i) {
        const CDataMarkerOutput::CElement& element = data[i];
        if (element.VariableName == marker)
            out = element.DataAsULONG;
    }
}

int main()
{
    CCharStringData radius = { "Radius", 6 }, radius2 = { "Radius", 6 }, colour = { "Colour", 6 }, rad = { "Rad", 3 };
    CCharStringData d0 = { "a", 1 }, d1 = { "b", 1 }, d2 = { "c", 1 }, none = { "", 0 };
    CDataMarkerOutput::CElement e[3];
    e[0].VariableName.PStringData = &radius;  e[0].DataAsString.PStringData = &d0; e[0].DataAsULONG = 0;
    e[1].VariableName.PStringData = &colour;  e[1].DataAsString.PStringData = &d1; e[1].DataAsULONG = 1;
    e[2].VariableName.PStringData = &radius2; e[2].DataAsString.PStringData = &d2; e[2].DataAsULONG = 2;
    CDataMarkerOutput data; data.begin = e; data.end = e + 3;
    CCharString marker; marker.PStringData = &radius;
    unsigned long out = 77;
    GetNumberFromDataMarker(marker, out, data);
    bool found = out == 2;
    CCharString missing; missing.PStringData = &rad;
    out = 77;
    GetNumberFromDataMarker(missing, out, data);
    if (!found || !(out == 77)) {
        std::printf("00cd5090_TEST FAIL\n");
        return 1;
    }
    std::printf("00cd5090_TEST PASS\n");
    return 0;
}