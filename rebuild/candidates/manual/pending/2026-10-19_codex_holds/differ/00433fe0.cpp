// Grade: HELD(bytes match retail) -- wrong callee: CPersistContext::Transfer float/long overloads swapped (0x431061 float, 0x431102 long) | Codex source 00/43/CEngineDef_Transfer_00433fe0.cpp, flags , pass pattern 00433fe0_TEST PASS
// CEngineDef::Transfer(CPersistContext&) -- transfer the engine definition's scalar fields in declaration order
// Retail 0x00433FE0, 208 bytes. Integer fields use CPersistContext::Transfer(long&) 0x00431061; packed/float fields use Transfer(float&) 0x00431102.
struct CPersistContext {
 void Transfer(long&); // 0x00431061
 void Transfer(float&); // 0x00431102
};
struct CEngineDef {
 unsigned char pad[0x3c]; long f3c,f40,f44,f48; float f4c,f50,f54,f58; long f5c; float f60,f64; long f68; float f6c,f70,f74; long pad78; long f7c;
 void Transfer(CPersistContext&);
};
void CEngineDef::Transfer(CPersistContext& c) {
 c.Transfer(f3c); c.Transfer(f40); c.Transfer(f44); c.Transfer(f48);
 c.Transfer(f60); c.Transfer(f64); c.Transfer(f4c); c.Transfer(f50); c.Transfer(f54); c.Transfer(f58);
 c.Transfer(f68); c.Transfer(f6c); c.Transfer(f70); c.Transfer(f74); c.Transfer(f5c); c.Transfer(pad78); c.Transfer(f7c);
}