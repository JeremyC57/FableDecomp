# Owner exception granted — pass17 row009d5d30 only

The owner replied: "if it is absolutely proven, you may use it this time", followed by "with evidence line in file".

Scope: retain the original standard C++ qualifier on CBankFileAsyncData::Finished (+0x44), BufferLocked (+0x45), and AllowRestart (+0x46), in the shared class header. Aborted (+0x47) stays ordinary bool. No other field, local, parameter or arbitrary code-generation usage is authorized by this exception.

Direct re-read of both original PDB TPI streams confirms LF_MODIFIER over T_BOOL, flags exactly0x2, at Ego_r type0xc09d and FableWin type0xbd67. Owning complete class records are0xc0a1 and0xbd6b respectively. The four member offsets agree across both builds even though their total complete class sizes differ (72 and92). This distinction is preserved; the retail179-byte constructor and native offsets determine the retail layout.

Native evidence: constructor009d5d9b/009d5d9e/009d5da6 writes the three fields at+44/+45/+46; completion callback009d54c0 sets Finished and009d54cc sets AllowRestart under the bank lock;009d5559 reads BufferLocked and009d5566 sets it;009d5b87 reads Finished in WaitUntilFinished,009d5bae clears AllowRestart before restarting reads. These are read-only evidence inspections of neighbouring functions, not new out-of-scope implementations.

The header must carry a Retail-volatile evidence line. Standard C++03 compilation portability is retained. This exception and the constructor behavior test do not claim portable cross-thread synchronization: volatile is neither an atomic nor a memory-ordering primitive.
