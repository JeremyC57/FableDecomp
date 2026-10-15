// Behaviour test for CScriptedMapBrush::SetThingToMove (0x0082EA50): the map entry for the id gets both the position and the facing angles.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct C3DAngles { float X, Y, Z; };
struct CThingPosAndOrientation { C3DVector Position; C3DAngles FacingAngles; };
struct CThingsToMoveMap {
    unsigned long long m_keys[4];
    CThingPosAndOrientation m_values[4];
    int m_count;
    int m_lookups;
    CThingPosAndOrientation& operator[](const unsigned long long& key)
    {
        ++m_lookups;
        for (int i = 0; i < m_count; ++i) if (m_keys[i] == key) return m_values[i];
        m_keys[m_count] = key;
        CThingPosAndOrientation zero = { { 0, 0, 0 }, { 0, 0, 0 } };
        m_values[m_count] = zero;
        return m_values[m_count++];
    }
};
struct CScriptedMapBrush {
    CThingsToMoveMap ThingsToMove;
    void SetThingToMove(unsigned long long thingID, const C3DVector& position, const C3DAngles& facingAngles)
    {
        ThingsToMove[thingID].Position = position;
        ThingsToMove[thingID].FacingAngles = facingAngles;
    }
};
int main()
{
    CScriptedMapBrush b;
    b.ThingsToMove.m_count = 0; b.ThingsToMove.m_lookups = 0;
    C3DVector p = { 1.0f, 2.0f, 3.0f };
    C3DAngles a = { 0.5f, 0.25f, 0.125f };
    b.SetThingToMove(0x100000002ULL, p, a);
    if (b.ThingsToMove.m_count != 1 || b.ThingsToMove.m_lookups != 2) { std::printf("0082ea50_TEST FAIL\n"); return 1; }
    const CThingPosAndOrientation& v = b.ThingsToMove.m_values[0];
    if (b.ThingsToMove.m_keys[0] != 0x100000002ULL || v.Position.X != 1.0f || v.Position.Z != 3.0f || v.FacingAngles.Y != 0.25f || v.FacingAngles.Z != 0.125f) { std::printf("0082ea50_TEST FAIL\n"); return 1; }
    std::printf("0082ea50_TEST PASS\n");
    return 0;
}
