# 009d5d30 identity, layout and bindings

Exact emitted constructor: `??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z`, private thiscall,179 bytes, ret12. Ego_r name-set entry00b29d50 and FableWin02ff9060 give that spelling. Native caller009d6084 passes the caller-owned buffer, length and bank; constructor member stores corroborate the overload. Main objdump symbol review confirmed the complete emitted Leaf.

## Original layout and narrowly authorized qualifiers

Ego_r complete class type0xc0a1 has size72 and FableWin complete type0xbd6b size92; their original core fields agree through+0x47. They prove CThreadedFileCallback at0, CMovableResource at4, Data8, Buffer0xc, Length0x10, SkipBytes0x14, ThreadedFileUID0x18, MemoryPool0x1c, MemoryHandle0x20, InternalAllocatedBufferSize0x24, BankFile0x28 as a reference, and LoadDesc0x2c. Native constructor offsets and vtable stores independently agree. Retail-size assertions compile under RTM; the larger debug-build tail is not copied into retail.

Direct original TPI-stream re-read confirms bool LF_MODIFIER flags exactly2 in Ego_r0xc09d and FableWin0xbd67 for Finished+0x44, BufferLocked+0x45, AllowRestart+0x46; Aborted+0x47 is primitive bool. See QUALIFIER_RECHECK.json and OWNER_EXCEPTION_GRANTED.md. The owner expressly authorized these three only and required the evidence line; both row and shared header carry Retail-volatile comments. No asm or custom reload trick is introduced. Standard C++ qualifier portability does not imply portable cross-thread synchronization.

## Supporting identities

CCriticalSectionLock is class V, with protected SDK _RTL_CRITICAL_SECTION reference at0 and Locked bool at4; original public constructor takes `_RTL_CRITICAL_SECTION&, bool`, destructor public nonvirtual. Both PDBs prove the declarations; native calls009d5dac and009d5dd4 target00a62920 and00a629a0. The native lock address is bank+0x14c, matching the original bank field. The base call009d5d37 targets the protected CBaseClassNonCopyable constructor0099a2f0.

The native set is bank+0x128; it contains CBankFileAsyncData pointers. The insert call009d5dc8 takes `this` by const reference with pair result via hidden pointer. The shared real STLport4.6 set/_Rb_tree template definitions give the complete emitted instantiation recorded in RELOCATION_AUDIT.json. No fabricated tree class or address-specific helper is used.

EThreadedFilePriority has exactly seven original enumerators (both raw LF_ENUM/field lists), NORMAL=3. See ENUM_EVIDENCE.md. The prior draft's DEFAULT was invented and is removed. CThreadedFileObjectDesc's six original fields and native constructor stores009d5d7a..009d5d8a prove its default values0,0,0,null,true,NORMAL.

## Vtable identity review

Native table0129b860 belongs to CMovableResource. Native0129b8b4 is CBankFileAsyncData via CThreadedFileCallback, and0129b8a0 is via CMovableResource. Both PDB base records and the complete emitted/native virtual slot lists independently establish these names. VTABLE_REVIEW.json records every slot including the inherited Verify and adjusted second-base destructor. Existing binding labels naming CBase, CThreadedFileRequest and Native::Movable are incorrect; NAME_FIXES.tsv reports them without changing the shared binding database.

All seven actual constructor relocation operands were independently decoded and checked against native targets. Main RTM recompilation matches the complete179-byte extent, with only those COFF operands masked. Main reran the exact selected behavior fixture under VC7.1 /Od /W3 /GR- and64-bit g++ -std=c++03: both PASS. Native offset/byte checks are separate from the host-layout behavior fixture.
