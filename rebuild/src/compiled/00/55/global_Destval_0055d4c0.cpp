class CPlayerGui {
public:
    void Open();   // 0x004374B0
};
extern void __fastcall dv_h1_0055d180(void*);
extern void* g_glob;
void __fastcall Dest_val(void* self){ dv_h1_0055d180(self); ((CPlayerGui*)(g_glob))->Open(); }