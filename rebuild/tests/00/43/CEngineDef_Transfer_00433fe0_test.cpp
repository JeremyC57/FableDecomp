// Behaviour test for retail 0x00433fe0: the candidate source below is embedded verbatim (from src/00433fe0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CEngineDef::Transfer(CPersistContext&) -- transfer the engine definition's scalar fields in the retail order
// Retail: Fable.exe @ 0x00433FE0, 208 bytes. Float fields go through CPersistContext::Transfer(float&) @ 0x00431061
// (fldz/fld/fstp body), long fields through CPersistContext::Transfer(long&) @ 0x00431102 (calls WriteLong 0x993eb0).
// Local model: no engine/CEngineDef.h exists. Field names/types are the PDB's (fable_types.h), but the retail object
// places them 4 bytes earlier than the Ego_r layout (floats 0x3C..0x5C, longs 0x60..0x7C), as the lea offsets show.
class CPersistContext { public:
 void Transfer(float&); // 0x00431061
 void Transfer(long&);  // 0x00431102
};
class CEngineDef { public:
 unsigned char _base[0x3c];
 float LODErrorTolerance, CharacterLODErrorTolerance, LODErrorFactor, SeaHeight;              // 0x3c..0x48
 float GamePrimitiveDefaultFadeStart, GamePrimitiveDefaultFadeRangeRatio;                     // 0x4c, 0x50
 float LocalDetailDefaultFadeStart, LocalDetailDefaultFadeRangeRatio, FOV_2D;                 // 0x54, 0x58, 0x5c
 long LocalDetailBooleanAlphaDefaultAlphaRef, DefaultPrimitiveAlphaRef;                       // 0x60, 0x64
 long TestStaticMesh, TestAnimatedMesh, TestAnim, TestGraphic;                                // 0x68..0x74
 long InvalidTextureStandin, InvalidThemeStandin;                                             // 0x78, 0x7c
 void Transfer(CPersistContext&);
};
void CEngineDef::Transfer(CPersistContext& c) {
 c.Transfer(LODErrorTolerance); c.Transfer(CharacterLODErrorTolerance); c.Transfer(LODErrorFactor); c.Transfer(SeaHeight);
 c.Transfer(LocalDetailBooleanAlphaDefaultAlphaRef); c.Transfer(DefaultPrimitiveAlphaRef);
 c.Transfer(GamePrimitiveDefaultFadeStart); c.Transfer(GamePrimitiveDefaultFadeRangeRatio);
 c.Transfer(LocalDetailDefaultFadeStart); c.Transfer(LocalDetailDefaultFadeRangeRatio);
 c.Transfer(TestStaticMesh); c.Transfer(TestAnimatedMesh); c.Transfer(TestAnim); c.Transfer(TestGraphic);
 c.Transfer(FOV_2D); c.Transfer(InvalidTextureStandin); c.Transfer(InvalidThemeStandin);
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_n; static int g_off[32]; static char g_kind[32]; static unsigned char* g_base;
void CPersistContext::Transfer(float& f) { g_off[g_n] = (int)((unsigned char*)&f - g_base); g_kind[g_n++] = 'f'; f += 1.0f; }
void CPersistContext::Transfer(long& l) { g_off[g_n] = (int)((unsigned char*)&l - g_base); g_kind[g_n++] = 'l'; l += 1; }
int main() {
 static const int eo[17] = {0x3c,0x40,0x44,0x48,0x60,0x64,0x4c,0x50,0x54,0x58,0x68,0x6c,0x70,0x74,0x5c,0x78,0x7c};
 CEngineDef d; memset(&d, 0, sizeof d); CPersistContext c; g_base = (unsigned char*)&d;
 int ok = sizeof(CEngineDef) == 0x80;
 d.Transfer(c);
 ok = ok && g_n == 17;
 for (int i = 0; ok && i < 17; ++i) ok = g_off[i] == eo[i] && g_kind[i] == (eo[i] < 0x60 ? 'f' : 'l');
 ok = ok && d.FOV_2D == 1.0f && d.InvalidThemeStandin == 1 && d.LODErrorTolerance == 1.0f;
 printf(ok ? "00433fe0_TEST PASS\n" : "00433fe0_TEST FAIL\n"); return ok ? 0 : 1;
}