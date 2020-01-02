#include <taihen.h>
#include <vitasdk.h>
#include <psp2/kernel/modulemgr.h> 

static SceUID g_hooks[1];
uint32_t text_addr, text_size, data_addr, data_size;

void _start() __attribute__ ((weak, alias ("module_start")));

static tai_hook_ref_t ref_hook0;
static int sub_8126A8CA_patched(int a1, int a2)
{
	//Returns > 1 if normal app
	//Returns 0 if NPXS10015 SceSettings (Launch in PHYMEMLOW mode)
	//??? Returns < 0 if NPXS10104 SceMiniSettings (Launch in special PHYMEMLOW mode) ???
	return 0;
}

int module_start(SceSize argc, const void *args)
	{
		tai_module_info_t info;
		info.size = sizeof(info);
		int ret = taiGetModuleInfo("SceShell", &info);
		if (ret < 0) {
			return SCE_KERNEL_START_FAILED;
		}

		// Modified from TheOfficialFloW/Adrenaline
		SceKernelModuleInfo mod_info;
		mod_info.size = sizeof(SceKernelModuleInfo);
		ret = sceKernelGetModuleInfo(info.modid, &mod_info);
		if (ret < 0) {
			return SCE_KERNEL_START_FAILED;
		}
		text_addr = (uint32_t)mod_info.segments[0].vaddr;
		text_size = (uint32_t)mod_info.segments[0].memsz;
		data_addr = (uint32_t)mod_info.segments[1].vaddr;
		data_size = (uint32_t)mod_info.segments[1].memsz;

		int offset;

		switch (info.module_nid) {

		case 0x0552F692: // retail 3.60 SceShell
			offset = 0x27246E;
			break;

		case 0x5549BF1F: // retail 3.65 SceShell
		case 0x34B4D82E: // retail 3.67 SceShell
		case 0x12DAC0F3: // retail 3.68 SceShell
			offset = 0x272526;
			break;

			//For testing
		case 0x587F9CED: // PTEL 3.65 SceShell
			offset = 0x26A8CA;
			break;

		default:
			return SCE_KERNEL_START_FAILED;
		}
  g_hooks[0] = taiHookFunctionOffset(&ref_hook0,
	  info.modid,
	  0,          // segidx
	  offset,     // offset
	  1,          // thumb
	  sub_8126A8CA_patched);

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	if (g_hooks[0] >= 0) taiHookRelease(g_hooks[0], ref_hook0);
  return SCE_KERNEL_STOP_SUCCESS;
}
