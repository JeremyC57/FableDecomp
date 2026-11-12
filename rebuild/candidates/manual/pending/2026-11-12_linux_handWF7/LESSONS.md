## 0x008B0B60

a called-but-visible CCountedPointer(T*) ctor (FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR) keeps Data/Info in a register across later calls; declaration-only reloads the slot (0x008B0B60).
CDefString -> CCharString through the conversion operator costs a copy ctor + dtor; retail's direct build is `def->GetInstantiationName().GetString()` (0x008B0B60).

## 0x0094F480

- A Creature load that retail repeats from one PBrain register (`mov ecx,[this+4] ; mov eax,[ecx+18h] ... mov ebx,[ecx+18h]`) while a `creature` local shares it: spell every use as the inline `GetCreature()` (CAIStateGroupBase / CAIBrain accessors), no local (0x0094F480).
- A first early-out with its own epilogue and a late `push edi` = separate `return false`s where retail has one nested condition with a single `return false` (0x0094F480).
- Targeting component slot 29 (+0x74) is GetPBestEnemyCreatureForAttack (CThingCreatureBase*), slot 33 (+0x84) is GetPTargetedThing: count the slots in engine/CTCTargetingBase.h, do not trust "slot 29 = targeted thing" comments in older rows.

## 0x005F6793

A size() hoisted out of a loop that makes calls means the vector-filling callee body (and its push_back body, NOINLINE opt-in) was visible to the TU: put the real landed callee body in the row, do not hand-hoist (hand hoist flips the register pick).

## 0x00847DF0

CAnimFilterOfGroup has no user-declared dtor: a stack filter's exit is ~Group then the BASE vptr store only; an inline virtual ~CAnimFilterOfGroup(){} adds a derived vptr store (0x00847DF0).
CIntelligentPointer operator* reference arguments to a new-expression ctor are evaluated left-to-right-ish into registers and pushed afterwards; raw GetPItem() arguments go right to left (0x00847DF0).

## 0x0097D6F0

thing-vs-creature self tests: put the intelligent pointer on the LEFT (`thing.Get() == &GetCreature()`) for `cmp eax,esi`; the creature still loads first (row 0x0097D6F0).

## 0x007735D0

- A named `const CThing& x = *smartPtr.Get();` local declared before a call (after a named manager reference) changes VC7.1's callee-saved register roles and def-slot order even though the bytes of the call sequence are the same; when a row's residue is a whole-function register rotation, try naming the call's object and argument as reference locals in evaluation order (0x007735D0: 430v448 -> match).

## 0x00773790

CVectorMap LowerBound: both bodies are visible in the retail TU (hiding the const one makes the house key reload instead of folding to cmp [eax],0D5h) -- do not hide them to chase a register residue.
A zero register (xor reg,reg for two CDefPointer ctors) is opportunistic: VC7.1 only makes one when a callee-saved register is free there; where it lands tells which registers the neighbours hold, it is not a source fact.
A callee-saved register rotation across a whole function (this / member-address CSE / def pointer in the wrong registers, every instruction shape right) can hinge on one call statement: name the object and the argument of a `a().b().f(*p.Get())` call as two locals in evaluation order (0x00773790: `reactions` reference, then `keeper` pointer) before trying anything about the variables that look wrong; either local alone does nothing.
A def pointer load (CDefPointer Object after GetDef with a visible body) is allocated last and takes the register left over: adding uses to it never moves it; fix the candidates that are placed before it.

## 0x007FD650

A debug lift written Pos + C3DVector(0,0,k) through C3DVector::operator+ folds the +0.0f of X/Y and keeps &Pos in a callee-saved register; member-wise C3DVector(P.X,P.Y,P.Z+k) reads off this and shifts the register plan (0x007FD650).

