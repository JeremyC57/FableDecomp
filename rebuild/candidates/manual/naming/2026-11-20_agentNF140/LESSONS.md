# NF140 lessons (one line each)
- A nested-type respell (enum / struct moved into the class) changes every leaf that mentions the type: census the own leaves of the binders too (SetDisplayMode / SetExclusiveMode / the CCreateDeviceParameters copy), not only the entry address.
- An out-of-line "copy constructor" of a struct whose member has a user copy constructor is the compiler-generated one: emit it pick-any with the nf86 anonymous-namespace scaffolding instead of a user-declared header opt-in (MATCH first time, and it ends the HEAD selection-1 vs pick-any clash).
- Protected statics (KIXXZ) reached from non-members are retail header inlines: port the callers onto the inline (Allocate / Deallocate), never declare the static public in a local model.
- A FABLE_FORCEINLINE helper in a landed row is the hint that the header inline needs the same control: Destroy and Deallocate both had to be forced (two DIFFERs).
- Insert a header include after the leading preprocessor block, never after the last `#` line of the file: a `#pragma auto_inline(off)` region turned an inline into a call (DIFFER 132v167).
- `objdump ... | head` inside a for loop kills the loop on SIGPIPE: write each file first, print afterwards.
- A `// Leaf:` line must hold the bare mangled name: a trailing comment makes q.py grade the largest function.
