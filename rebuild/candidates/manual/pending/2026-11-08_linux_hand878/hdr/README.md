# Chunk 878 header overlay

No header changes. 0x00C4F600 uses the shared `engine/EAXNativeTypes.h` (EAX30LISTENERPROPERTIES / EAX30BUFFERPROPERTIES,
which pulls in EAX40.h for the FX-slot GUIDs) unchanged. The two resamplers model the Intel dvec.h classes (M128, I32vec4,
Is32vec4, I16vec8, Is16vec8) locally, exactly as the landed Is32vec4 operator rows 0x00C7B870 / 0x00C7B8B0 / 0x00C7B8F0 and
the dvec constructors 0x00C7B7D0..0x00C7B840 do; there is no shared dvec header in rebuild/include (RTM VC7.1 ships none).
A shared `dvec` header for the mixer kernels would be a landing-time decision.
