// CHitLocationDef::Transfer -- serialize hit-location scalars in the retail persistence order
// Retail: Fable.exe @ 0x004585FE, 237 bytes. Size-oriented definition TU; typed helpers are called at the addresses noted below.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct SLocation { unsigned char data[4]; };
struct SFloat { float value; };
struct SLong { long value; };
struct SBool { bool value; };
struct CPersistContext {
 void Transfer(SLocation*); // 0x00458833
 void Transfer(SLong*); // 0x00431102
 void Transfer(SFloat*); // 0x004586EB
 void Transfer(SBool*); // 0x0043314A
 void TransferTail(unsigned char*); // 0x0045878F
};
struct CHitLocationDef {
 virtual void v00();
 unsigned char base[0x38];
 SLocation loc; // +3c
 unsigned char pad40[4];
 SLong l44; SFloat f48; SFloat f4c; SFloat f50; SFloat f54; SFloat f58;
 SLong l5c; SLong l60; SLong l64; SLong l68;
 unsigned char b6c; unsigned char b6d; unsigned char b6e; unsigned char b6f; SLong l70; SBool b74; unsigned char b75; unsigned char pad76[2]; unsigned char tail[4];
 void Transfer(CPersistContext& context);
};
void CHitLocationDef::Transfer(CPersistContext& context) {
 context.Transfer(&loc); context.Transfer(&l44); context.Transfer(&l5c); context.Transfer(&l60); context.Transfer(&l64);
 context.Transfer(&f48); context.Transfer(&f4c); context.Transfer(&f50); context.Transfer(&f54); context.Transfer(&f58);
 long transferred = l68.value; context.Transfer((SLong*)&transferred); l68.value = transferred; context.Transfer((SBool*)&b6e); context.Transfer((SBool*)&b6f); context.Transfer(&l70);
 context.Transfer(&b74); context.Transfer((SBool*)&b6d); context.Transfer((SBool*)&b6c); context.Transfer((SBool*)&b75);
 context.TransferTail(tail);
}
