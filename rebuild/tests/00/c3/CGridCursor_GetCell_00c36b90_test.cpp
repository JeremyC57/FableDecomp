// Behaviour test for CGridCursor::GetCell (0x00C36B90): cell origin = floor(p / size) * size per axis (returned), fraction = (p - cell) / size.
#include <stdio.h>
#include <math.h>
struct V { float m_x, m_y, m_z; };
static V GetCell(const V& position, float size, V* fraction)
{
    float row = (float)floor((1.0f / size) * position.m_y);
    V cell;
    cell.m_x = (float)floor((1.0f / size) * position.m_x) * size;
    cell.m_y = row * size;
    float layer = (float)floor((1.0f / size) * position.m_z);
    cell.m_z = layer * size;
    fraction->m_x = (position.m_x - cell.m_x) * (1.0f / size);
    fraction->m_y = (position.m_y - cell.m_y) * (1.0f / size);
    fraction->m_z = (position.m_z - cell.m_z) * (1.0f / size);
    return cell;
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    V p = { 5.0f, -3.0f, 9.5f }, f;
    V c = GetCell(p, 4.0f, &f);
    if (!Near(c.m_x, 4.0f) || !Near(c.m_y, -4.0f) || !Near(c.m_z, 8.0f) || !Near(f.m_x, 0.25f) || !Near(f.m_y, 0.25f) || !Near(f.m_z, 0.375f))
    { printf("00c36b90_TEST FAIL\n"); return 1; }
    printf("00c36b90_TEST PASS\n"); return 0;
}