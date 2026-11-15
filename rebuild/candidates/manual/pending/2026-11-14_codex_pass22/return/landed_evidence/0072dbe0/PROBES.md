# Attempts

| Attempt | Strategy | Result |
|---|---|---|
|1|Original empty user destructor with real shared bases and vector member; new base destructor declarations|91/91 masked bytes; tests pass, but emitted vtables expose incomplete old shared declarations|
|1 review correction|Complete actual base overrides, pure secondary methods and reverse overload declaration order under new opt-ins|91/91 masked bytes retained; correct86/4-slot derived tables|

One destructor source strategy, two grades. The vtable audit is part of correctness, not a second arbitrary source experiment. VC71 /Od /GR- and g++ C++03 behavior tests pass24 scenarios. No unusual inline controls or manual destructor calls were necessary.
