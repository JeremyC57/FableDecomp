# Retry200C lessons

- Named CCharString inside an inline query helper produces native LEA argument and loop layout; an unnamed conversion temporary forwards ctor EAX and misses14 bytes (00EF29E0).
- Use actual CResource/CIVCounted headers plus field-free method views for frame initialization; existing shared graphic destructor naturally matches native release+free chain (009FE550).
- Manifest CAICreatureWillPowerIndicatorDef on6CD2F0 is wrong; actual string names CCreatureNavigationDef and Radius+30. A69B20/A69AD0 labels likewise propagate from unrelated IV-owning types.
- Preserve the native dimension-comparison bug:009FC950 compares both frame dimensions against returned X, and never Y.
- The normalized section coordinate helper scales32767.0f and performs current-mode dword FISTP. Ordinary truncation fails nearest-even/down/up modes. Portable semantic source is retained; no assembly workaround (009FCA40).

Review at landing: independently audit every relocation and predicate vtable for both READY rows. Fixture assertions survive native compilation; full host PMF widths are retained. Parked sources006CD890/009FC950 exhausted six new + four UNCERTAIN reserved;009FCA40 preserves five unused new grades. No phase reset or additional grade is authorized.

- Root review rejected free-fastcall substitution for an IV virtual member even when ECX aligns. Genuine member-pointer dispatch reproduces the same009FE550 bytes atnew2; member ABI is part of source correctness.
