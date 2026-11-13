Many CText* text-line pointers in one UI builder: one reused `line` local, not one named local per line (each named local is a frame node; 0x005A9CBE).
A brief early return in a big UI builder with destructible locals: if/else with one return, else the dtor tail is duplicated (0x005A9CBE).
