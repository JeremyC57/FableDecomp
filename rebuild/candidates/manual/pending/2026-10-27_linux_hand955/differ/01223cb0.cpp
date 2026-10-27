// `dynamic initializer for 'g_UnprovenFloat_013bc3ac''() -- store 5.0f into the .bss float 0x013BC3AC at start-up
// Grade: DIFFER(12v11) -- 12v11 compiler dynamic initializer (.text$yc) storing 5.0f into the .bss float 0x013BC3AC; global and initializer unrecovered; VC7.1 RTM folds every constant float initializer tried to static data, an invented initializer object reproduces the bytes
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x01223cb0, 11 bytes, in the .text$yc group (compiler dynamic initializers), no arguments.
// Identity (descriptive): a dynamic initializer that stores the constant 5.0f into the .bss float 0x013BC3AC (no code
//   reference to the thunk; it is reached through the .CRT$XCU table). The initialised global, its type and the initializer
//   expression are unrecovered.
// Residue: VC7.1 RTM folds every constant float initializer tried into static .data and emits no thunk (const float,
//   -k, -1.0f * 1.0f, casts, inline / static / __forceinline function calls, a converting constructor, a user copy
//   constructor); an initializer object whose constructor stores the global reproduces the bytes but is invented. Parked.
extern float UnprovenInitialValue_013bc3ac();
float g_UnprovenFloat_013bc3ac = UnprovenInitialValue_013bc3ac();
