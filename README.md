# LowMemMode
Plugin for PS Vita that disables memory budget check during app launch in SceShell.

Allows you to launch other applications in LOWPHYMEM mode while enlarged memory apps (+29MiB and +77MiB) is in the background.

Boot param set in control information type 6 at offset 0x1D must be 0x80 (memory budget 30MiB) or lower, otherwise such action will not be possible (C1-2609-7 SCE_KERNEL_ERROR_CANNOT_ALLOC_PHYMEMLOW).

Only for 3.60-3.65 retail FW versions

Install under *main
