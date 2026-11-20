# NF135 lessons (one line each)
- A HEAD "Native" stand-in row (ThingNative / ModeStandNative / quarantine CThing under a renaming macro) moves onto the shared headers in one form when every callee is taken from a landed row that already binds its PDB spelling (0x008D8CE0 for ORIGINAL_VTABLE + DRAINED SuspendAI, 0x00956E70 for GetSubDef<T>).
- HEAD's names for a sub-def and its fields can be invented: read the GetSubDef<T> owner's self leaf and literal (0x008E7790 = CBriarRoseDef) and map the offsets onto the real def header before naming anything.
- A def read without AddRef (`mov ecx,[thing+70h]` straight into GetSubDef) is CThing::PeekBaseDef(), not GetPBaseDef() (which copies and releases).
- VC7.1 drops the CDefPointer release's null test after a member read through the pointer: an unconditional `dec [ebx+4]` at the end is the plain header dtor, not a hand-written release.
- A C3DVector built from an expression straight into its declaration differs from retail's declared-then-assigned temporary (DIFFER 478v502 -> match); keep HEAD's assignment form.
- CThingManager.h needs FABLE_RETAIL_CCLASSFACTORY and CWorldMap.h's IsPosInMap needs FABLE_RETAIL_CWORLDMAP; the cpp precheck passes without them, so grep a landed user's define set first.
- Before a self rename, xr.sh the address: 0x008DCB60 had one referencer (0x008DF310) that already used the namespaced spelling with the wrong access; an in-place `private:` closes the family.
- A sub-def class with only a flat header needs a retail branch over CSubDefClassBase (copy the CBriarRoseDef.h shape) before a row can hold CDefPointer<const T>.
