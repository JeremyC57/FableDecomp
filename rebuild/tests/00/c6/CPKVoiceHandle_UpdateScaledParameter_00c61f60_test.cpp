// behaviour test for 0x00C61F60 CPKVoiceHandle::UpdateScaledParameter: value * scale (unless unscaled) sent for a live handle
#include <cstdio>
#include <cmath>
struct CPKVoiceTable {
    int m_calls; int m_handle; float m_value;
    void SetValue(int handle, float value) { ++m_calls; m_handle = handle; m_value = value; }
};
struct CPKScaleSource { float m_scale; float GetScale() { return m_scale; } };
struct CPKEmitterOwner { CPKScaleSource m_scale; };
struct CPKEmitter {
    CPKEmitterOwner* m_pOwner; float m_value; CPKVoiceTable* m_pTable;
    float GetValue() { return m_value; }
};
struct CPKVoiceHandle {
    bool m_bUnscaled; int m_handle;
    CPKVoiceTable* GetTable(CPKEmitter* e) { return e->m_pTable; }
    void UpdateScaledParameter(CPKEmitter* emitter)
    {
        if (m_handle >= 0) {
            float value = emitter->GetValue();
            if (!m_bUnscaled) value *= emitter->m_pOwner->m_scale.GetScale();
            GetTable(emitter)->SetValue(m_handle, value);
        }
    }
};
int main()
{
    CPKVoiceTable table = { 0, 0, 0.0f };
    CPKEmitterOwner owner = { { 0.5f } };
    CPKEmitter e = { &owner, 0.8f, &table };
    CPKVoiceHandle h = { false, -1 };
    h.UpdateScaledParameter(&e);
    if (table.m_calls != 0) { std::printf("00c61f60_TEST FAIL\n"); return 1; }
    h.m_handle = 7; h.UpdateScaledParameter(&e);
    if (table.m_calls != 1 || table.m_handle != 7 || std::fabs(table.m_value - 0.4f) > 1e-6f) { std::printf("00c61f60_TEST FAIL\n"); return 1; }
    h.m_bUnscaled = true; h.UpdateScaledParameter(&e);
    if (table.m_calls != 2 || std::fabs(table.m_value - 0.8f) > 1e-6f) { std::printf("00c61f60_TEST FAIL\n"); return 1; }
    std::printf("00c61f60_TEST PASS\n");
    return 0;
}