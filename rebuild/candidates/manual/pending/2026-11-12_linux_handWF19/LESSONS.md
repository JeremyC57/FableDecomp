## 0x006BFCD0

- (wf19, 2026-11-13) VC7.1 keeps the RIGHT operand of a vector add on the x87 stack (`fxch st(2) ; fadd st,st(2) ; fstp st(1)`) only when the left operand is a by-value copy evaluated first and the right is bound by const reference; named locals, member/free operator+ in either order and reversed bodies all give top-of-stack-destination (0x006BFCD0).

## 0x00B206B0

VC7.1 reorders a one-line float sum of equal-weight terms canonically (ignores source order); running statements d=a; d+=b; pin the order (0x00B206B0).

## 0x0094A3B0

VC7.1 tail-merge survivor / odd mov-ecx hoists: try a duplicated source tail (natural else-branch repeat) -- cross-jumping merges it and the hoists follow.

