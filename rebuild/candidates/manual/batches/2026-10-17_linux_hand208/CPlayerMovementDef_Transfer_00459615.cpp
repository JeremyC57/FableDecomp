#pragma optimize("s", on)
// CPlayerMovementDef::Transfer(CPersistContext&) -- serializes sixteen contiguous 4-byte movement fields through the persistence context.
// Retail 0x00459615, 189 bytes. The first, second, third, fourth, fifth, sixth, ninth and final fields use 0x00431061;
// fields at +54, +58 and +60 use the alternate persistence overload at 0x00431102.
struct CPersistContext {
    void Transfer(void*);      // 0x00431061
    void TransferAlt(void*);   // 0x00431102
};
struct CPlayerMovementDef {
    unsigned char pad[0x3c];
    unsigned char f3c[4], f40[4], f44[4], f48[4], f4c[4], f50[4];
    unsigned char f54[4], f58[4], f5c[4], f60[4], f64[4], f68[4], f6c[4], f70[4], f74[4], f78[4];
    void Transfer(CPersistContext&);
};
void CPlayerMovementDef::Transfer(CPersistContext& ctx) {
    ctx.Transfer(f3c); ctx.Transfer(f40); ctx.Transfer(f44); ctx.Transfer(f48); ctx.Transfer(f4c); ctx.Transfer(f50);
    ctx.TransferAlt(f54); ctx.TransferAlt(f58); ctx.Transfer(f5c); ctx.TransferAlt(f60); ctx.Transfer(f64); ctx.Transfer(f68);
    ctx.Transfer(f6c); ctx.Transfer(f70); ctx.Transfer(f74); ctx.Transfer(f78);
}
