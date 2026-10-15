// Behaviour test for 0x0098B390 CShaderRenderManager::SetPixelShaderConstant: known name, unknown name and a
// name whose index is -1 (host name map + constant table).
#include <cstdio>
struct C4DVector { float X, Y, Z, W; };
struct NameMap { int ids[8]; long idx[8]; int n; long* find(int id) { for (int i = 0; i < n; ++i) if (ids[i] == id) return &idx[i]; return 0; } };
struct Mgr {
    NameMap names; C4DVector table[8]; long tableSize; unsigned int UpdateFlags;
    void SetPixelShaderConstant(int name, const C4DVector& value)
    {
        long index;
        long* it = names.find(name);
        if (it == 0 || (index = *it) == -1) {
            index = tableSize;
            names.ids[names.n] = name; names.idx[names.n] = index; ++names.n;   // Add(name)
            table[tableSize++] = value;                                       // push_back(value)
        }
        table[index] = value;
        UpdateFlags |= 2u;
    }
};
int main()
{
    Mgr m; m.names.n = 1; m.names.ids[0] = 5; m.names.idx[0] = 0; m.tableSize = 1; m.UpdateFlags = 0;
    C4DVector v = { 1, 2, 3, 4 };
    m.SetPixelShaderConstant(5, v);
    if (m.tableSize != 1 || m.table[0].W != 4 || m.UpdateFlags != 2) { std::printf("0098b390_TEST FAIL\n"); return 1; }
    C4DVector w = { 5, 6, 7, 8 };
    m.SetPixelShaderConstant(9, w);
    if (m.tableSize != 2 || m.table[1].X != 5 || m.names.n != 2) { std::printf("0098b390_TEST FAIL\n"); return 1; }
    m.names.idx[0] = -1; m.SetPixelShaderConstant(5, w);
    if (m.tableSize != 3 || m.table[2].Y != 6 || m.names.n != 3) { std::printf("0098b390_TEST FAIL\n"); return 1; }
    std::printf("0098b390_TEST PASS\n");
    return 0;
}
